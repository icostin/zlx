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
#define ATOM_OFS_MASK (ATOM_SIZE - 1)
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

static void * ZLX_CALL alloc
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    size_t size
);

/*  free  */
/**
 *  Free a non-zero sized previous allocation.
 */
static void ZLX_CALL free
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    void * ptr,
    size_t size
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

/*  insert_chunk_in_cell  */
/**
 *  Inserts a free chunk.
 */
static void ZLX_CALL insert_chunk_in_cell
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    void * ptr,
    unsigned int cell
);

/*  alloc_chunk_from_cell  */
/**
 *  Extracts chunk from non-empty cell, splits into used/free and
 *  reinserts the free portion in the appropriate cell.
 */
static void * ZLX_CALL alloc_chunk_from_cell
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    unsigned int cell,
    size_t size
);

/* ptr_delta ****************************************************************/
ZLX_INLINE  void * ptr_delta (void * ptr, intptr_t delta)
{
    return (uint8_t *) ptr + delta;
}

/* sep_size_left ************************************************************/
ZLX_INLINE size_t sep_size_left (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return sep->left_size_ctl & SEP_SIZE_MASK;
}

/* sep_size_right ***********************************************************/
ZLX_INLINE size_t sep_size_right (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return sep->right_size_ctl & SEP_SIZE_MASK;
}

/* sep_free_left ************************************************************/
ZLX_INLINE int sep_free_left (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return (sep->left_size_ctl & SEP_CTL_MASK) == SEP_CTL_FREE;
}

// ZLX_INLINE int sep_used_left (tlsf_sep_t * ZLX_RESTRICT sep)
// {
//     return (sep->left_size_ctl & SEP_CTL_MASK) == SEP_CTL_USED;
// }

/* sep_free_right ***********************************************************/
ZLX_INLINE int sep_free_right (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return (sep->right_size_ctl & SEP_CTL_MASK) == SEP_CTL_FREE;
}

#if ZLXOPT_ASSERT
/* sep_used_right ***********************************************************/
ZLX_INLINE int sep_used_right (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return (sep->right_size_ctl & SEP_CTL_MASK) == SEP_CTL_USED;
}
#endif


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
    tlsf_sep_t * lsep;

    if (new_size > tma->max_block_size)
    {
        M("requested $xz which is higher than max_block_size=$xz", new_size,
          tma->max_block_size);
        return NULL;
    }

    if (!old_size) return alloc(tma, new_size);
    if (!new_size)
    {
        free(tma, old_ptr, old_size);
        return NULL;
    }

    /* realloc */
    lsep = ptr_delta(old_ptr, -(ptrdiff_t) ATOM_SIZE);

    ZLX_ASSERT(((uintptr_t) old_ptr & ATOM_OFS_MASK) == 0);
    ZLX_ASSERT(sep_used_right(lsep));

    old_size = SIZE_ALIGN_UP(old_size, ATOM_SIZE);
    ZLX_ASSERT(sep_size_right(lsep) == old_size);

    new_size = SIZE_ALIGN_UP(new_size, ATOM_SIZE);
    if (new_size <= old_size)
    {
        tlsf_sep_t * rsep;
        tlsf_sep_t * sep;
        size_t free_size;

        if (new_size == old_size)
        {
            M("realloc to same atom count: return same $p", old_ptr);
            return old_ptr;
        }
        /* shrink */
        M("shrink $p from $xz to $xz", old_ptr, old_size, new_size);
        rsep = ptr_delta(old_ptr, zlx_size_to_sptr(old_size));
        ZLX_ASSERT(rsep->left_size_ctl == lsep->right_size_ctl);
        if (sep_free_right(rsep))
        {
            zlx_np_t * np = ptr_delta(rsep, ATOM_SIZE);
            size_t rsize = sep_size_right(rsep);
            if (rsize) zlx_dlist_delete(np);
            free_size = ATOM_SIZE + rsize;
            M("take over free block on the right - $xz", free_size);
            rsep = ptr_delta(np, zlx_size_to_sptr(rsize));
        }
        else free_size = 0;

        free_size += old_size - new_size - ATOM_SIZE;
        sep = ptr_delta(old_ptr, zlx_size_to_sptr(new_size));
        sep->left_size_ctl = lsep->right_size_ctl = new_size | SEP_CTL_USED;
        sep->right_size_ctl =
            rsep->left_size_ctl = free_size | SEP_CTL_FREE;
        insert_chunk_in_cell(tma, ptr_delta(sep, ATOM_SIZE),
                          zlx_tlsf_size_to_cell(free_size));
        return old_ptr;
    }
    else
    {
        /* realloc - enlarge allocation */
        tlsf_sep_t * sep = ptr_delta(old_ptr, zlx_size_to_sptr(old_size));
        size_t size = sep_size_right(sep);
        ZLX_ASSERT(sep->left_size_ctl == lsep->right_size_ctl);
        if (sep_free_right(sep)
            && new_size - old_size <= size + ATOM_SIZE)
        {
            /* can enlarge in-place */
            tlsf_sep_t * rsep;
            rsep = ptr_delta(sep, zlx_size_to_sptr(size + ATOM_SIZE));
            ZLX_ASSERT(sep->right_size_ctl == rsep->left_size_ctl);

            if (size) zlx_dlist_delete(ptr_delta(sep, ATOM_SIZE));

            if (new_size - old_size == size + ATOM_SIZE)
            {
                /* use the entire free buffer */
                rsep->left_size_ctl =
                    lsep->right_size_ctl = new_size | SEP_CTL_USED;
            }
            else
            {
                /* shrink the free block on the right */
                sep = ptr_delta(old_ptr, zlx_size_to_sptr(new_size));
                size -= new_size - old_size;
                sep->left_size_ctl =
                    lsep->right_size_ctl = new_size | SEP_CTL_USED;
                sep->right_size_ctl =
                    rsep->left_size_ctl = size | SEP_CTL_FREE;
                insert_chunk_in_cell(tma, ptr_delta(sep, ATOM_SIZE),
                                     zlx_tlsf_size_to_cell(size));
            }
            return old_ptr;
        }
        else
        {
            void * ptr;
            ptr = alloc(tma, new_size);
            if (ptr)
            {
                zlx_u8a_copy(ptr, old_ptr, old_size);
                free(tma, old_ptr, old_size);
            }
            return ptr;
        }
    }
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

/* alloc_chunk_from_cell ****************************************************/
static void * ZLX_CALL alloc_chunk_from_cell
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    unsigned int cell,
    size_t size
)
{
    tlsf_sep_t * sep;
    tlsf_sep_t * rsep;
    size_t chunk_size;
    void * chunk;

    ZLX_ASSERT((size & (ATOM_SIZE - 1)) == 0);

    sep = extract_free_chunk(tma, cell);
    chunk = ptr_delta(sep, ATOM_SIZE);
    chunk_size = sep->right_size_ctl & SEP_SIZE_MASK;
    M("got chunk of size $xz", chunk_size);
    ZLX_ASSERT(size <= chunk_size);

    rsep = ptr_delta(chunk, zlx_size_to_sptr(chunk_size));
    M("rsep=$p -> l=$xz r=$xz",
      rsep, rsep->left_size_ctl, rsep->right_size_ctl);
    ZLX_ASSERT((rsep->left_size_ctl & SEP_SIZE_MASK) == chunk_size);
    if (size == chunk_size)
    {
        sep->right_size_ctl = chunk_size | SEP_CTL_USED;
        rsep->left_size_ctl = chunk_size | SEP_CTL_USED;
    }
    else
    {
        tlsf_sep_t * split;
        zlx_np_t * list_entry;
        size_t leftover;

        ZLX_ASSERT(chunk_size - size >= ATOM_SIZE);
        leftover = chunk_size - size - ATOM_SIZE;

        split = ptr_delta(chunk, zlx_size_to_sptr(size));
        M("leftover=$xz split=$p", leftover, split);
        list_entry = ptr_delta(split, ATOM_SIZE);

        sep->right_size_ctl = size | SEP_CTL_USED;
        split->left_size_ctl = size | SEP_CTL_USED;
        split->right_size_ctl = leftover | SEP_CTL_FREE;
        rsep->left_size_ctl = leftover | SEP_CTL_FREE;

        insert_chunk_in_cell(tma, list_entry, zlx_tlsf_size_to_cell(leftover));
    }

    return chunk;
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

    ZLX_ASSERT(cell > 0);
    ZLX_ASSERT((tma->row_mask >> (cell >> COLUMN_COUNT_LOG2)) & 1);
    np = tma->free_list_table[cell].next;
    ZLX_ASSERT(np != &tma->free_list_table[cell]);

    zlx_dlist_delete(np);

    sep = ptr_delta(np, -(ptrdiff_t) ATOM_SIZE);
    ZLX_ASSERT((sep->right_size_ctl & SEP_CTL_MASK) == SEP_CTL_FREE);

    if (zlx_dlist_is_empty(&tma->free_list_table[cell]))
    {
        unsigned int row, col;

        row = cell >> COLUMN_COUNT_LOG2;
        col = cell & COLUMN_MASK;
        M("cell=$xi row=$xi col=$xi is left empty", cell, row, col);

        tma->cmask_table[row] &= ~(column_mask_t) 1 << col;
        M("cmask=$xi", tma->cmask_table[row]);
        if (tma->cmask_table[row] == 0)
        {
            tma->row_mask &= ~(row_mask_t) 1 << row;
            M("row_mask=$xz", tma->row_mask);
        }
    }
    M("extracted chunk $p: l=$p r=$p",
      sep, sep->left_size_ctl, sep->right_size_ctl);

    return sep;
}

/* insert_chunk_in_cell *****************************************************/
static void ZLX_CALL insert_chunk_in_cell
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    void * ptr,
    unsigned int cell
)
{
    if (cell == 0)
    {
        M("zero-sized free chunk at ptr=$p", ptr);
        return;
    }

    if (zlx_dlist_is_empty(&tma->free_list_table[cell]))
    {
        unsigned int row, col;

        row = cell >> COLUMN_COUNT_LOG2;
        col = cell & COLUMN_MASK;
        tma->row_mask |= (row_mask_t) 1 << row;
        tma->cmask_table[row] |= (column_mask_t) 1 << col;
        M("updating masks: rmask=$xz cmask[$i]=$xz",
          tma->row_mask, row, tma->cmask_table[row]);
    }

    zlx_dlist_insert(&tma->free_list_table[cell], ptr, ZLX_NEXT);
    M("inserted $p into free list for cell $xi", ptr, cell);
}

/* alloc ********************************************************************/
static void * ZLX_CALL alloc
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    size_t size
)
{
    unsigned int cell, free_cell;
    void * ptr;

    if (!size) return NULL;

    size = SIZE_ALIGN_UP(size, ATOM_SIZE);

    /* get the cell where the requested size falls into */
    cell = zlx_tlsf_size_to_cell(size);
    cell += (zlx_tlsf_cell_to_size(cell) < size);
    M("alloc: ma=$p size=$z cell=$xi", tma, size, cell);

    free_cell = free_cell_lookup(tma, cell);
    ptr = (free_cell == 0) ? NULL : alloc_chunk_from_cell(tma, free_cell, size);
    M("alloc size=$z -> ptr=$p", size, ptr);
    return ptr;
}

/* free *********************************************************************/
static void ZLX_CALL free
(
    tlsf_ma_t * ZLX_RESTRICT tma,
    void * ptr,
    size_t size
)
{
    tlsf_sep_t * sep = ptr_delta(ptr, -(ptrdiff_t) ATOM_SIZE);
    tlsf_sep_t * rsep;

    M("free ptr=$p: sep.left=$xz sep.right=$xz",
      ptr, sep->left_size_ctl, sep->right_size_ctl);
    ZLX_ASSERT(((uintptr_t) ptr & ATOM_OFS_MASK) == 0);
    ZLX_ASSERT(sep_used_right(sep));
    size = SIZE_ALIGN_UP(size, ATOM_SIZE);
    ZLX_ASSERT(sep_size_right(sep) == size);
    ZLX_ASSERT(size == SIZE_ALIGN_UP(size, ATOM_SIZE));
    rsep = ptr_delta(ptr, zlx_size_to_sptr(size));
    ZLX_ASSERT(rsep->left_size_ctl == sep->right_size_ctl);
    /* merge with block on the right if free */
    if (sep_free_right(rsep))
    {
        size_t rsize = sep_size_right(rsep);
        if (rsize) zlx_dlist_delete(ptr_delta(rsep, ATOM_SIZE));
        size += ATOM_SIZE + rsize;
        rsep = ptr_delta(ptr, zlx_size_to_sptr(size));
    }
    /* merge with block on the left if free */
    if (sep_free_left(sep))
    {
        size_t lsize = sep_size_left(sep);
        tlsf_sep_t * lsep;
        lsep = ptr_delta(sep, -zlx_size_to_ptrdiff(ATOM_SIZE + lsize));
        ZLX_ASSERT(lsep->right_size_ctl == sep->left_size_ctl);
        if (lsize) zlx_dlist_delete(ptr_delta(lsep, ATOM_SIZE));
        size += ATOM_SIZE + lsize;
        sep = lsep;
    }
    rsep->left_size_ctl = sep->right_size_ctl = size | SEP_CTL_FREE;
    insert_chunk_in_cell(tma, ptr_delta(sep, ATOM_SIZE),
                      zlx_tlsf_size_to_cell(size));
}

