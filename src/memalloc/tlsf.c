#include "../../include/zlx/memalloc/tlsf.h"
#include "../../include/zlx/memalloc/nop.h"
#include "../../include/zlx/int/ops.h"
#include "../../include/zlx/int/array.h"
#include "../../include/zlx/dlist.h"
#include "../../include/zlx/debug.h"

/****************************************************************************/

ZLX_STATIC_ASSERT(sizeof(void *) == 4 || sizeof(void *) == 8);
#define ATOM_SIZE_LOG2 4
#define ATOM_SIZE ((size_t) 1 << ATOM_SIZE_LOG2)
#define COLUMN_COUNT_LOG2 5
#define COLUMN_COUNT (1 << COLUMN_COUNT_LOG2)
#define COLUMN_MASK (COLUMN_COUNT - 1)

#define SIZE_ALIGN_UP(z, a) (((z) + (a) - 1) & ~(size_t) ((a) - 1))
#define UPTR_ALIGN_UP(p, a) (((p) + (a) - 1) & ~(uintptr_t) ((a) - 1))
#define UPTR_ALIGN_DOWN(p, a) ((p) & ~(uintptr_t) ((a) - 1))

#define SEP_CTL_MASK 1
#define SEP_CTL_FREE 1
#define SEP_CTL_USED 0
#define SEP_SIZE_MASK (~(uintptr_t) SEP_CTL_MASK)

#define M ZLX_DMSG

/****************************************************************************/

typedef uint32_t column_mask_t;
#define cmask_lssb zlx_u32_lssb

typedef size_t row_mask_t;
#define rmask_lssb zlx_size_lssb

typedef struct tlsf_block tlsf_block_t;
struct tlsf_block
{
    uint8_t * data;
    size_t size;
};

typedef struct tlsf_sep tlsf_sep_t;
struct tlsf_sep
{
    uintptr_t left_size_ctl;
    uintptr_t right_size_ctl;
};
ZLX_STATIC_ASSERT(sizeof(tlsf_sep_t) <= ATOM_SIZE);

typedef struct tlsf_ma tlsf_ma_t;
struct tlsf_ma
{
    zlx_ma_t ma;
    column_mask_t * cmask_table;
    tlsf_block_t * block_table;
    row_mask_t row_mask;
    size_t max_block_size;
    size_t block_count;
    uint8_t row_count;
    uint8_t magic[7];
    zlx_np_t free_list_table[1]; // table of lists
};


/****************************************************************************/

static void * ZLX_CALL tlsf_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
);

static uint8_t ZLX_CALL compute_row_count
(
    size_t max_block_size
);

/*  free_cell_lookup  */
/**
 *  Searches for a cell of desired size.
 *  @returns cell number of 0 for none available.
 */
static unsigned int ZLX_CALL free_cell_lookup
(
    tlsf_ma_t const * ZLX_RESTRICT tma,
    unsigned int cell
);

/*  extract_free_chunk  */
/**
 *  Extracts a chunk from a non-empty cell and updates masks.
 */
static tlsf_sep_t * ZLX_CALL extract_free_chunk
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    unsigned int cell
);

/*  insert_free_chunk  */
/**
 *  Inserts a free chunk.
 */
static void ZLX_CALL insert_free_chunk
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    void * ptr,
    unsigned int cell
);

/*  alloc_chunk  */
/**
 *  Extracts chunk from non-empty cell, splits into used/free and
 *  reinserts the free portion in the appropriate cell.
 */
static void * ZLX_CALL alloc_chunk
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    unsigned int cell,
    size_t size
);

/* zlx_tlsf_create **********************************************************/
ZLX_API zlx_tlsf_status_t ZLX_CALL zlx_tlsf_create
(
    zlx_ma_t * * ZLX_RESTRICT ma_p,
    void * buffer,
    size_t size,
    size_t max_block_size
)
{
    uint8_t row_count;
    size_t size_needed, free_size;
    tlsf_ma_t * tma;
    tlsf_sep_t * btab_pre_sep;
    tlsf_sep_t * btab_post_sep;
    tlsf_sep_t * end_sep;
    zlx_np_t * free_list_entry;
    uintptr_t va, end_va;
    unsigned int i, init_cell, init_row;

    if (max_block_size == 0 ||
        max_block_size < size ||
        max_block_size > ZLX_TLSF_BLOCK_LIMIT)
        return ZLX_TLSF_BAD_MAX;

    row_count = compute_row_count(max_block_size);
    M("row_count = $i", row_count);

    va = (uintptr_t) buffer;
    end_va = va + size;
    M("original buffer: $p - $p", va, end_va);
    if (end_va < va) return ZLX_TLSF_BAD_BUFFER;

    end_va = UPTR_ALIGN_DOWN(end_va, ATOM_SIZE);
    if (va >= end_va) return ZLX_TLSF_BUFFER_TOO_SMALL;

    va = UPTR_ALIGN_UP(va, sizeof(void *));
    size = zlx_uptr_to_size(end_va - va);
    M("edge aligned buffer: $p - $p, size=$xz", va, end_va, size);

    tma = (tlsf_ma_t *) va;
    va += ZLX_FIELD_OFFSET(tlsf_ma_t, free_list_table);
    va += (size_t) row_count * COLUMN_COUNT * sizeof(tma->free_list_table[0]);

    tma->cmask_table = (column_mask_t *) va;
    va += sizeof(column_mask_t) * row_count;

    va = UPTR_ALIGN_UP(va, ATOM_SIZE);
    btab_pre_sep = (tlsf_sep_t *) va;
    va += ATOM_SIZE;

    tma->block_table = (tlsf_block_t *) va;
    tma->block_count = 1;
    va += ATOM_SIZE;

    btab_post_sep = (tlsf_sep_t *) va;
    va += ATOM_SIZE;

    free_list_entry = (zlx_np_t *) va;
    va += ATOM_SIZE;

    size_needed = va - (uintptr_t) tma + ATOM_SIZE;
    M("size_needed=$xz size=$xz", size_needed, size);
    if (size_needed > size)
    {
        *(uintptr_t *) ma_p = size_needed;
        return ZLX_TLSF_BUFFER_TOO_SMALL;
    }

    end_va -= ATOM_SIZE;
    end_sep = (tlsf_sep_t *) end_va;

    free_size = end_va - (uintptr_t) free_list_entry;
    init_cell = zlx_tlsf_size_to_cell(free_size);
    M("initial free block: size=$xz cell=$xz", free_size, init_cell);
    ZLX_ASSERT(init_cell < row_count * COLUMN_COUNT);
    init_row = init_cell >> COLUMN_COUNT_LOG2;

    *ma_p = &tma->ma;
    tma->ma.realloc = tlsf_realloc;
    tma->ma.contains = zlx_ma_nop_contains;
    tma->ma.info_set = zlx_ma_nop_info_set;
    tma->ma.check = zlx_ma_nop_check;
    tma->row_mask = (row_mask_t) 1 << init_row;
    tma->max_block_size = max_block_size;
    tma->row_count = row_count;

    for (i = 0; i < row_count; ++i)
    {
        tma->cmask_table[i] = 0;
    }
    tma->cmask_table[init_row] = (column_mask_t) 1 << (init_cell & COLUMN_MASK);

    for (i = 0; i < row_count * COLUMN_COUNT; ++i)
    {
        zlx_dlist_init(&tma->free_list_table[i]);
    }
    zlx_dlist_insert(&tma->free_list_table[init_cell], free_list_entry,
                     ZLX_PREV);

    btab_pre_sep->left_size_ctl = 0 | SEP_CTL_USED;
    btab_pre_sep->right_size_ctl = ATOM_SIZE | SEP_CTL_USED;
    btab_post_sep->left_size_ctl = ATOM_SIZE | SEP_CTL_USED;
    btab_post_sep->right_size_ctl = free_size | SEP_CTL_FREE;
    end_sep->left_size_ctl = free_size | SEP_CTL_FREE;
    end_sep->right_size_ctl = 0 | SEP_CTL_USED;

    zlx_u8a_copy(tma->magic, (uint8_t const *) "[tlsf!]", 7);

    M("returning successfully ma=$p", tma);
    return ZLX_TLSF_OK;
}

/* zlx_tlsf_add_block *******************************************************/
ZLX_API zlx_tlsf_status_t ZLX_CALL zlx_tlsf_add_block
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * buffer,
    size_t size
)
{
    (void) ma;
    (void) buffer;
    (void) size;
    M("ma=$p, buffer=$p, size=$xz", ma, buffer, size);
    return ZLX_TLSF_NO_SUP;
}

/* zlx_tlsf_size_to_cell ****************************************************/
ZLX_API unsigned int zlx_tlsf_size_to_cell
(
    size_t size
)
{
    size_t atom_count;
    unsigned int q;

    // size = 0x1357
    atom_count = (size + ATOM_SIZE - 1) >> ATOM_SIZE_LOG2;
    // atom_count = 0x136
    if (atom_count < COLUMN_COUNT) return zlx_size_to_uint(atom_count);

    q = zlx_size_mssb((atom_count >> COLUMN_COUNT_LOG2));
    // q = 9
    return zlx_size_to_uint((zlx_uint_to_size(q) << COLUMN_COUNT_LOG2) + (atom_count >> q));
}

/* zlx_tlsf_cell_to_size ****************************************************/
ZLX_API size_t zlx_tlsf_cell_to_size
(
    unsigned int cell
)
{
    unsigned int q;
    if (cell < COLUMN_COUNT) return zlx_uint_to_size(cell) << ATOM_SIZE_LOG2;
    q = (cell >> COLUMN_COUNT_LOG2) - 1;
    return zlx_uint_to_size(COLUMN_COUNT | (cell & COLUMN_MASK))
        << (q + ATOM_SIZE_LOG2);
}

/* tlsf_realloc *************************************************************/
static void * ZLX_CALL tlsf_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
)
{
    tlsf_ma_t * ZLX_RESTRICT tma = (tlsf_ma_t *) ma;

    if (!old_size)
    {
        unsigned int cell, free_cell;
        void * ptr;

        if (!new_size) return NULL;

        /* alloc */

        new_size = SIZE_ALIGN_UP(new_size, ATOM_SIZE);
        /* get the cell where the requested size falls into */
        cell = zlx_tlsf_size_to_cell(new_size);
        cell += (zlx_tlsf_cell_to_size(cell) < new_size);
        M("alloc: ma=$p size=$z cell=$xi", tma, new_size, cell);

        free_cell = free_cell_lookup(tma, cell);
        if (free_cell == 0) return NULL;

        ptr = alloc_chunk(tma, free_cell, new_size);
        M("size=$z -> ptr=$p", new_size, ptr);
        return ptr;
    }
    else if (!new_size)
    {
        /* free */
        tlsf_sep_t * lsep = old_ptr;
        lsep--;
    }
    else
    {
        /* realloc */
    }
    return NULL;
}

/* compute_row_count ********************************************************/
static uint8_t ZLX_CALL compute_row_count
(
    size_t max_block_size
)
{
    zlx_uint_t cell;

    max_block_size = UPTR_ALIGN_UP(max_block_size, ATOM_SIZE);
    ZLX_ASSERT(max_block_size != 0);
    cell = zlx_tlsf_size_to_cell(max_block_size);
    cell += (zlx_tlsf_cell_to_size(cell) != max_block_size);

    return zlx_uint_to_u8((cell >> COLUMN_COUNT_LOG2) + 1);
}

/* free_cell_lookup *********************************************************/
static unsigned int ZLX_CALL free_cell_lookup
(
    tlsf_ma_t const * ZLX_RESTRICT tma,
    unsigned int cell
)
{
    unsigned int start_row, start_column, free_cell;
    column_mask_t shifted_column_mask;

    start_row = cell >> COLUMN_COUNT_LOG2;
    ZLX_ASSERT(start_row < tma->row_count);

    start_column = cell & COLUMN_MASK;
    shifted_column_mask = tma->cmask_table[start_row] >> start_column;
    M("cell=$xi start_row=$xi start_column=$xi cmask=$xi "
      "shifted_column_mask=$xi",
      cell, start_row, start_column, tma->cmask_table[start_row],
      shifted_column_mask);
    ZLX_ASSERT(((tma->row_mask >> start_row) & 1) == !!shifted_column_mask);
    if (shifted_column_mask)
    {
        free_cell = (start_row << COLUMN_COUNT_LOG2) + start_column
            + cmask_lssb(shifted_column_mask);
    }
    else
    {
        row_mask_t shifted_row_mask;
        unsigned int row = start_row + 1;
        shifted_row_mask = tma->row_mask >> row;
        M("row_mask=$xz shifted_row_mask=$xz", tma->row_mask, shifted_row_mask);
        if (shifted_row_mask == 0) return 0;
        row += rmask_lssb(shifted_row_mask);
        M("row=$xi cmask=$xi", row, tma->cmask_table[row]);
        ZLX_ASSERT(tma->cmask_table[row] != 0);
        free_cell = (row << COLUMN_COUNT_LOG2)
            | cmask_lssb(tma->cmask_table[row]);
    }
    M("start_cell=$xi free_cell=$xi", cell, free_cell);

    return free_cell;
}

/* alloc_chunk **************************************************************/
static void * ZLX_CALL alloc_chunk
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    unsigned int cell,
    size_t size
)
{
    tlsf_sep_t * sep;
    tlsf_sep_t * rsep;
    size_t chunk_size;

    ZLX_ASSERT((size & (ATOM_SIZE - 1)) == 0);

    sep = extract_free_chunk(tma, cell);
    chunk_size = sep->right_size_ctl & SEP_SIZE_MASK;
    ZLX_ASSERT(size <= chunk_size);

    rsep = (tlsf_sep_t *) ((uint8_t *) (sep + 1) + chunk_size);
    ZLX_ASSERT((rsep->left_size_ctl & SEP_SIZE_MASK) == chunk_size);
    if (size == chunk_size)
    {
        sep->right_size_ctl = chunk_size | SEP_CTL_USED;
        rsep->left_size_ctl = chunk_size | SEP_CTL_USED;
    }
    else
    {
        tlsf_sep_t * split;
        size_t left_size;
        ZLX_ASSERT(chunk_size - size >= ATOM_SIZE);
        left_size = size - chunk_size - ATOM_SIZE;
        
        split = (tlsf_sep_t *) ((uint8_t *) sep + size);

        sep->right_size_ctl = size | SEP_CTL_USED;
        split->left_size_ctl = size | SEP_CTL_USED;
        split->right_size_ctl = left_size | SEP_CTL_FREE;
        rsep->left_size_ctl = left_size | SEP_CTL_FREE;

        insert_free_chunk(tma, split + 1, zlx_tlsf_size_to_cell(left_size));
    }

    return sep + 1;
}

/* extract_free_chunk *******************************************************/
static tlsf_sep_t * ZLX_CALL extract_free_chunk
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    unsigned int cell
)
{
    zlx_np_t * np;
    tlsf_sep_t * sep;

    ZLX_ASSERT((tma->row_mask >> (cell >> COLUMN_COUNT_LOG2)) & 1);
    np = tma->free_list_table[cell].next;
    ZLX_ASSERT(np != &tma->free_list_table[cell]);

    zlx_dlist_delete(np);

    sep = (tlsf_sep_t *) np - 1;
    ZLX_ASSERT((sep->right_size_ctl & SEP_CTL_MASK) == SEP_CTL_FREE);

    if (zlx_dlist_is_empty(&tma->free_list_table[cell]))
    {
        unsigned int row, col;

        row = cell >> COLUMN_COUNT_LOG2;
        col = cell & COLUMN_MASK;
        M("cell=$xi row=$xi col=$xi is left empty", cell, row, col);

        tma->cmask_table[row] &= ~(column_mask_t) 1 << col;
        M("cmask=$xi", tma->cmask_table[row]);
        if (tma->cmask_table[row])
        {
            tma->row_mask &= ~(row_mask_t) 1 << row;
            M("row_mask=$xz", tma->row_mask);
        }
    }

    return sep;
}

/* insert_free_chunk ********************************************************/
static void ZLX_CALL insert_free_chunk
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    void * ptr,
    unsigned int cell
)
{
    if (zlx_dlist_is_empty(&tma->free_list_table[cell]))
    {
        unsigned int row, col;

        row = cell >> COLUMN_COUNT_LOG2;
        col = cell & COLUMN_MASK;
        tma->row_mask |= (row_mask_t) 1 << row;
        tma->cmask_table[row] |= (column_mask_t) 1 << col;
    }

    zlx_dlist_insert(&tma->free_list_table[cell], ptr, ZLX_NEXT);
}

