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

#define TLSF_MAGIC_LIT "[tlsf!]"
#define TLSF_MAGIC_PTR ((uint8_t const *) TLSF_MAGIC_LIT)
#define TLSF_MAGIC_LEN (sizeof(TLSF_MAGIC_LIT) - 1)

/****************************************************************************/

typedef uint32_t column_mask_t;
#define cmask_lssb zlx_u32_lssb

typedef size_t row_mask_t;
#define rmask_lssb zlx_size_lssb

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
    zlx_tlsf_block_t * block_table;
    row_mask_t row_mask;
    size_t max_block_size;
    size_t block_count;
    uint8_t row_count;
    uint8_t magic[TLSF_MAGIC_LEN];
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
    zlx_np_t * chunk,
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

/* ptr_add ******************************************************************/
ZLX_INLINE void * ptr_add (void * ptr, size_t delta)
{
    ZLX_ASSERT((uintptr_t) ptr + delta >= (uintptr_t) ptr);
    return (uint8_t *) ptr + delta;
}

/* ptr_sub ******************************************************************/
ZLX_INLINE void * ptr_sub (void * ptr, size_t delta)
{
    ZLX_ASSERT((uintptr_t) ptr - delta <= (uintptr_t) ptr);
    return (uint8_t *) ptr - delta;
}


/* left_chunk_size **********************************************************/
ZLX_INLINE size_t left_chunk_size (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return sep->left_size_ctl & SEP_SIZE_MASK;
}

/* right_chunk_size *********************************************************/
ZLX_INLINE size_t right_chunk_size (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return sep->right_size_ctl & SEP_SIZE_MASK;
}

/* left_chunk_is_free *******************************************************/
ZLX_INLINE int left_chunk_is_free (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return (sep->left_size_ctl & SEP_CTL_MASK) == SEP_CTL_FREE;
}

// ZLX_INLINE int sep_used_left (tlsf_sep_t * ZLX_RESTRICT sep)
// {
//     return (sep->left_size_ctl & SEP_CTL_MASK) == SEP_CTL_USED;
// }

/* right_chunk_is_free ******************************************************/
ZLX_INLINE int right_chunk_is_free (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return (sep->right_size_ctl & SEP_CTL_MASK) == SEP_CTL_FREE;
}

#if ZLXOPT_ASSERT
/* right_chunk_is_used ******************************************************/
ZLX_INLINE int right_chunk_is_used (tlsf_sep_t * ZLX_RESTRICT sep)
{
    return !right_chunk_is_free(sep);
}
#endif


/* zlx_tlsf_create **********************************************************/
ZLX_API zlx_tlsf_status_t ZLX_CALL zlx_tlsf_create
(
    zlx_ma_t * * ZLX_RESTRICT ma_p,
    void * buffer,
    size_t buffer_size,
    size_t max_block_size
)
{
    uint8_t row_count;
    size_t size_needed, free_size, size;
    tlsf_ma_t * tma;
    tlsf_sep_t * btab_pre_sep;
    tlsf_sep_t * btab_post_sep;
    tlsf_sep_t * end_sep;
    zlx_np_t * free_list_entry;
    uintptr_t va, end_va;
    unsigned int i, init_cell, init_row;

    if (max_block_size == 0 ||
        max_block_size < buffer_size ||
        max_block_size > ZLX_TLSF_BLOCK_LIMIT)
        return ZLX_TLSF_BAD_MAX;

    row_count = compute_row_count(max_block_size);
    M("row_count = $i", row_count);

    va = (uintptr_t) buffer;
    end_va = va + buffer_size;
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

    tma->block_table = (zlx_tlsf_block_t *) va;
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

    zlx_u8a_copy(tma->magic, TLSF_MAGIC_PTR, TLSF_MAGIC_LEN);

    tma->block_table[0].data = buffer;
    tma->block_table[0].size = buffer_size;

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
    tlsf_ma_t * ZLX_RESTRICT tma = (tlsf_ma_t *) ma;
    uintptr_t begin, end;
    size_t chunk_size;
    void * chunk_ptr;
    unsigned int cell;

    begin = (uintptr_t) buffer;
    end = begin + size;
    begin = UPTR_ALIGN_UP(begin, ATOM_SIZE);
    end = UPTR_ALIGN_DOWN(end, ATOM_SIZE);
    M("ma=$p (block_count=$z), buffer=$p, size=$xz: begin=$p end=$p",
      ma, tma->block_count, buffer, size, begin, end);
    if (begin > end || end - begin < 3 * ATOM_SIZE)
        return ZLX_TLSF_BUFFER_TOO_SMALL;

    chunk_size = end - begin - 2 * ATOM_SIZE;
    if (chunk_size > tma->max_block_size)
    {
        M("buffer too large: chunk_size=$xz max=$xz",
          chunk_size, tma->max_block_size);
        return ZLX_TLSF_BUFFER_TOO_LARGE;
    }
    chunk_ptr = (void *) (begin + ATOM_SIZE);

    {
        tlsf_sep_t * lsep = (tlsf_sep_t *) begin;
        lsep->left_size_ctl = 0 | SEP_CTL_USED;
        lsep->right_size_ctl = chunk_size | SEP_CTL_FREE;
    }

    {
        tlsf_sep_t * rsep = (tlsf_sep_t *) (end - ATOM_SIZE);
        rsep->left_size_ctl = chunk_size | SEP_CTL_FREE;
        rsep->right_size_ctl = 0 | SEP_CTL_USED;
    }

    cell = zlx_tlsf_size_to_cell(chunk_size);
    M("add free chunk of $xz bytes at cell $d", chunk_size, cell);
    insert_chunk_in_cell(tma, chunk_ptr, cell);

    {
        tlsf_sep_t * btab_lsep = ptr_sub(tma->block_table, ATOM_SIZE);
        size_t btab_size = right_chunk_size(btab_lsep);
        size_t btab_cap = btab_size / sizeof(zlx_tlsf_block_t);
        M("btab_size=$xz btab_cap=$z btab_len=$z", btab_size, btab_cap,
          tma->block_count);
        ZLX_ASSERT(right_chunk_is_used(btab_lsep));
        ZLX_ASSERT(tma->block_count <= btab_cap);
        if (tma->block_count == btab_cap)
        {
            /* need to resize block table. try to double it */
            void * new_btab_ptr = tlsf_realloc(tma->block_table, btab_size,
                                               btab_size << 1, ma);
            M("doubling btab: $p", new_btab_ptr);
            if (new_btab_ptr == NULL)
            {
                new_btab_ptr = tlsf_realloc(tma->block_table,
                                            btab_size,
                                            btab_size + ATOM_SIZE,
                                            ma);
                M("adding one atom to btab: $p", new_btab_ptr);
                if (new_btab_ptr == NULL)
                {
                    M("cannot realloc the block table");
                    extract_free_chunk(tma, chunk_ptr, cell);
                    return ZLX_TLSF_BUFFER_TOO_SMALL;
                }
            }
            tma->block_table = new_btab_ptr;
        }
        tma->block_table[tma->block_count].data = buffer;
        tma->block_table[tma->block_count].size = size;
        tma->block_count += 1;
    }

    return ZLX_TLSF_OK;
}

/* zlx_tlsf_size_to_cell ****************************************************/
ZLX_API unsigned int ZLX_CALL zlx_tlsf_size_to_cell
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
ZLX_API size_t ZLX_CALL zlx_tlsf_cell_to_size
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
    lsep = ptr_sub(old_ptr, ATOM_SIZE);

    ZLX_ASSERT(((uintptr_t) old_ptr & ATOM_OFS_MASK) == 0);
    ZLX_ASSERT(right_chunk_is_used(lsep));

    old_size = SIZE_ALIGN_UP(old_size, ATOM_SIZE);
    ZLX_ASSERT(right_chunk_size(lsep) == old_size);

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
        rsep = ptr_add(old_ptr, old_size);
        ZLX_ASSERT(rsep->left_size_ctl == lsep->right_size_ctl);
        if (right_chunk_is_free(rsep))
        {
            zlx_np_t * np = ptr_add(rsep, ATOM_SIZE);
            size_t rsize = right_chunk_size(rsep);
            if (rsize)
                extract_free_chunk(tma, np, zlx_tlsf_size_to_cell(rsize));
            free_size = ATOM_SIZE + rsize;
            M("take over free block on the right - $xz", free_size);
            rsep = ptr_add(np, rsize);
        }
        else free_size = 0;

        free_size += old_size - new_size - ATOM_SIZE;
        sep = ptr_add(old_ptr, new_size);
        sep->left_size_ctl = lsep->right_size_ctl = new_size | SEP_CTL_USED;
        sep->right_size_ctl =
            rsep->left_size_ctl = free_size | SEP_CTL_FREE;
        insert_chunk_in_cell(tma, ptr_add(sep, ATOM_SIZE),
                          zlx_tlsf_size_to_cell(free_size));
        return old_ptr;
    }
    else
    {
        /* realloc - enlarge allocation */
        tlsf_sep_t * sep = ptr_add(old_ptr, old_size);
        size_t size = right_chunk_size(sep);
        ZLX_ASSERT(sep->left_size_ctl == lsep->right_size_ctl);
        if (right_chunk_is_free(sep)
            && new_size - old_size <= size + ATOM_SIZE)
        {
            /* can enlarge in-place */
            tlsf_sep_t * rsep;
            rsep = ptr_add(sep, size + ATOM_SIZE);
            ZLX_ASSERT(sep->right_size_ctl == rsep->left_size_ctl);

            if (size)
                extract_free_chunk(tma, ptr_add(sep, ATOM_SIZE),
                                   zlx_tlsf_size_to_cell(size));

            if (new_size - old_size == size + ATOM_SIZE)
            {
                /* use the entire free buffer */
                rsep->left_size_ctl =
                    lsep->right_size_ctl = new_size | SEP_CTL_USED;
            }
            else
            {
                /* shrink the free block on the right */
                sep = ptr_add(old_ptr, new_size);
                size -= new_size - old_size;
                sep->left_size_ctl =
                    lsep->right_size_ctl = new_size | SEP_CTL_USED;
                sep->right_size_ctl =
                    rsep->left_size_ctl = size | SEP_CTL_FREE;
                insert_chunk_in_cell(tma, ptr_add(sep, ATOM_SIZE),
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
    ZLX_ASSERT(((tma->row_mask >> start_row) & 1)
               == !!tma->cmask_table[start_row]);
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

    sep = extract_free_chunk(tma, tma->free_list_table[cell].next, cell);
    chunk = ptr_add(sep, ATOM_SIZE);
    chunk_size = sep->right_size_ctl & SEP_SIZE_MASK;
    M("got chunk of size $xz", chunk_size);
    ZLX_ASSERT(size <= chunk_size);

    rsep = ptr_add(chunk, chunk_size);
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

        split = ptr_add(chunk, size);
        M("leftover=$xz split=$p", leftover, split);
        list_entry = ptr_add(split, ATOM_SIZE);

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
    zlx_np_t * np,
    unsigned int cell
)
{
    tlsf_sep_t * sep;

    M("np=$p cell=$xi", np, cell);
    ZLX_ASSERT(cell > 0);
    ZLX_ASSERT((tma->row_mask >> (cell >> COLUMN_COUNT_LOG2)) & 1);
    ZLX_ASSERT(np != &tma->free_list_table[cell]);

    zlx_dlist_delete(np);

    sep = ptr_sub(np, ATOM_SIZE);
    ZLX_ASSERT((sep->right_size_ctl & SEP_CTL_MASK) == SEP_CTL_FREE);

    if (zlx_dlist_is_empty(&tma->free_list_table[cell]))
    {
        unsigned int row, col;

        row = cell >> COLUMN_COUNT_LOG2;
        col = cell & COLUMN_MASK;
        M("cell=$xi row=$xi col=$xi is left empty", cell, row, col);

        tma->cmask_table[row] &= ~((column_mask_t) 1 << col);
        M("cmask=$xi", tma->cmask_table[row]);
        if (tma->cmask_table[row] == 0)
        {
            tma->row_mask &= ~((row_mask_t) 1 << row);
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
    M("alloc: ma=$p size=$xz cell=$xi", tma, size, cell);

    free_cell = free_cell_lookup(tma, cell);
    ptr = (free_cell == 0) ? NULL : alloc_chunk_from_cell(tma, free_cell, size);
    M("alloc size=$xz -> ptr=$p", size, ptr);
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
    tlsf_sep_t * sep = ptr_sub(ptr, ATOM_SIZE);
    tlsf_sep_t * rsep;

    M("free ptr=$p size=$xz: sep.left=$xz sep.right=$xz",
      ptr, size, sep->left_size_ctl, sep->right_size_ctl);
    ZLX_ASSERT(((uintptr_t) ptr & ATOM_OFS_MASK) == 0);
    ZLX_ASSERT(right_chunk_is_used(sep));
    size = SIZE_ALIGN_UP(size, ATOM_SIZE);
    ZLX_ASSERT(right_chunk_size(sep) == size);
    ZLX_ASSERT(size == SIZE_ALIGN_UP(size, ATOM_SIZE));
    rsep = ptr_add(ptr, size);
    ZLX_ASSERT(rsep->left_size_ctl == sep->right_size_ctl);
    /* merge with block on the right if free */
    if (right_chunk_is_free(rsep))
    {
        size_t rsize = right_chunk_size(rsep);
        M("merging with free block on the right rsize=$xz, size=$xz",
          rsize, size);
        if (rsize)
            extract_free_chunk(tma, ptr_add(rsep, ATOM_SIZE),
                               zlx_tlsf_size_to_cell(rsize));
        size += ATOM_SIZE + rsize;
        M("merged with free block on the right rsize=$xz, new size=$xz",
          rsize, size);
        rsep = ptr_add(ptr, size);
    }
    /* merge with block on the left if free */
    if (left_chunk_is_free(sep))
    {
        size_t lsize = left_chunk_size(sep);
        tlsf_sep_t * lsep;
        lsep = ptr_sub(sep, ATOM_SIZE + lsize);
        ZLX_ASSERT(lsep->right_size_ctl == sep->left_size_ctl);
        M("merging with free block on the left lsize=$xz size=$xz",
          lsize, size);
        if (lsize)
            extract_free_chunk(tma, ptr_add(lsep, ATOM_SIZE),
                               zlx_tlsf_size_to_cell(lsize));
        size += ATOM_SIZE + lsize;
        sep = lsep;
        M("merge with free block on the left lsize=$xz size=$xz", lsize, size);
    }
    rsep->left_size_ctl = sep->right_size_ctl = size | SEP_CTL_FREE;
    insert_chunk_in_cell(tma, ptr_add(sep, ATOM_SIZE),
                      zlx_tlsf_size_to_cell(size));
}

/* zlx_tlsf_walk ************************************************************/
ZLX_API int ZLX_CALL zlx_tlsf_walk
(
    zlx_ma_t * ma
)
{
    tlsf_ma_t * ZLX_RESTRICT tma = (tlsf_ma_t *) ma;
    unsigned int row, col;
    int bug = 0;

    ZLX_DFMT("ma = $p ----------------------------------\n", ma);
    ZLX_DFMT("row_count         $04xb\n", tma->row_count);
    ZLX_DFMT("row_mask          $018xz\n", tma->row_mask);
    for (row = 0; row < tma->row_count; ++row)
    {
        int bad = (!!tma->cmask_table[row]) != ((tma->row_mask >> row) & 1);

        if (!bad && tma->cmask_table[row] == 0) continue;
        ZLX_DFMT("row[$04xb]          cmask=$010xd, rmask is $s\n",
                 row, tma->cmask_table[row], bad ? "BAD ***" : "ok");
        bug |= bad;
    }

    for (row = 0; row < tma->row_count; ++row)
    {
        for (col = 0; col < COLUMN_COUNT; ++col)
        {
            zlx_np_t * np;
            unsigned int cell = (row << COLUMN_COUNT_LOG2) | col;
            if (zlx_dlist_is_empty(&tma->free_list_table[cell]) &&
                !((tma->cmask_table[row] >> col) & 1))
                continue;
            ZLX_DFMT("cell=$04xd row=$02xb col=$02xb range=[$xz-$xz]\n",
                     cell, row, col, zlx_tlsf_cell_to_size(cell),
                     zlx_tlsf_cell_to_size(cell + 1) - 1);
            for (np = tma->free_list_table[cell].next;
                 np != &tma->free_list_table[cell];
                 np = np->next)
            {
                tlsf_sep_t * lsep = ptr_sub(np, ATOM_SIZE);
                tlsf_sep_t * rsep = ptr_add(np, right_chunk_size(lsep));
                int bad = !right_chunk_is_free(lsep);
                bad |= (lsep->right_size_ctl != rsep->left_size_ctl);
                bad |= !((tma->row_mask >> row) & 1);
                ZLX_DFMT("                  <$s:$08xz $s:$08xz | ptr=$018p | $s:$08xz $s:$08xz>\n",
                         left_chunk_is_free(lsep) ? "f" : "u", left_chunk_size(lsep),
                         right_chunk_is_free(lsep) ? "f" : "u", right_chunk_size(lsep),
                         np,
                         left_chunk_is_free(rsep) ? "f" : "u", left_chunk_size(rsep),
                         right_chunk_is_free(rsep) ? "f" : "u", right_chunk_size(rsep));
                bug |= bad;
            }
        }
    }
    ZLX_DFMT("ma = $p - $s =================================\n", ma,
             bug ? "CORRUPT" : "ok");
    return bug;
}

/* zlx_tlsf_get_blocks ******************************************************/
ZLX_API size_t ZLX_CALL zlx_tlsf_get_blocks
(
    zlx_ma_t const * ma,
    zlx_tlsf_block_t * * blocks
)
{
    tlsf_ma_t * ZLX_RESTRICT tma = (tlsf_ma_t *) ma;
    ZLX_ASSERT(zlx_u8a_cmp(tma->magic, TLSF_MAGIC_PTR, TLSF_MAGIC_LEN) == 0);
    *blocks = tma->block_table;
    return tma->block_count;
}

