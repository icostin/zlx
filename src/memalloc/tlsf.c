#include "../../include/zlx/memalloc/tlsf.h"
#include "../../include/zlx/int/ops.h"
#include "../../include/zlx/int/array.h"
#include "../../include/zlx/dlist.h"

/****************************************************************************/

ZLX_STATIC_ASSERT(sizeof(void *) == 4 || sizeof(void *) == 8);
#define ATOM_SIZE_LOG2 4
#define ATOM_SIZE ((size_t) 1 << ATOM_SIZE_LOG2)
#define COLUMN_COUNT_LOG2 5
#define COLUMN_COUNT (1 << COLUMN_COUNT_LOG2)
#define COLUMN_MASK (COLUMN_COUNT - 1)

#define UPTR_ALIGN_UP(p, a) (((p) + (a) - 1) & ~(uintptr_t) ((a) - 1))
#define UPTR_ALIGN_DOWN(p, a) ((p) & ~(uintptr_t) ((a) - 1))

#define SEP_CTL_MASK 1
#define SEP_CTL_FREE 1
#define SEP_CTL_USED 0
#define SEP_SIZE_MASK (~(uintptr_t) SEP_CTL_MASK)

/****************************************************************************/

typedef uint32_t column_mask_t;
typedef size_t row_mask_t;

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

/* tlsf_realloc *************************************************************/
static void * ZLX_CALL tlsf_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
)
{
    (void) old_ptr;
    (void) old_size;
    (void) new_size;
    (void) ma;
    return NULL;
#if 0
    tlsf_ma_t * ZLX_RESTRICT tma = (tlsf_ma_t *) ma;

    if (!old_size)
    {
        unsigned int cell;

        if (!new_size) return NULL;
        /* alloc */

        /* get the cell where the requested size falls into */
        cell = zlx_tlsf_size_to_cell(new_size);

        cell += (zlx_tlsf_cell_to_size(cell) != new_size);
    }
    else if (!new_size)
    {
        /* free */
    }
    else
    {
        /* realloc */
    }
#endif
}



/* compute_row_count ********************************************************/
static uint8_t compute_row_count
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

    va = (uintptr_t) buffer;
    end_va = va + size;
    if (end_va < va) return ZLX_TLSF_BAD_BUFFER;

    end_va = UPTR_ALIGN_DOWN(end_va, ATOM_SIZE);
    if (va >= end_va) return ZLX_TLSF_BUFFER_TOO_SMALL;

    va = UPTR_ALIGN_UP(va, sizeof(void *));
    size = zlx_uptr_to_size(end_va - va);

    tma = (tlsf_ma_t *) va;
    va += ZLX_FIELD_OFFSET(tlsf_ma_t, free_list_table);
    va += (size_t) row_count * COLUMN_COUNT * sizeof(tma->free_list_table[0]);

    tma->cmask_table = (column_mask_t *) va;
    va += sizeof(column_mask_t) * row_count;

    va = UPTR_ALIGN_UP(va, ATOM_SIZE);
    btab_pre_sep = (tlsf_sep_t *) va;
    va += sizeof(tlsf_sep_t);

    tma->block_table = (tlsf_block_t *) va;
    tma->block_count = 1;
    va += ATOM_SIZE;

    btab_post_sep = (tlsf_sep_t *) va;
    va += ATOM_SIZE;

    free_list_entry = (zlx_np_t *) va;
    va += ATOM_SIZE;

    size_needed = va - (uintptr_t) tma + ATOM_SIZE;
    if (size_needed > size) 
    {
        *(uintptr_t *) ma_p = size_needed;
        return ZLX_TLSF_BUFFER_TOO_SMALL;
    }

    end_va -= ATOM_SIZE;
    end_sep = (tlsf_sep_t *) end_va;

    free_size = end_va - va;
    init_cell = zlx_tlsf_size_to_cell(free_size);
    ZLX_ASSERT(init_cell < row_count * COLUMN_COUNT);
    init_row = init_cell >> COLUMN_COUNT_LOG2;

    *ma_p = &tma->ma;
    tma->ma.realloc = tlsf_realloc;
    tma->ma.contains = NULL;
    tma->ma.info_set = NULL;
    tma->ma.check = NULL;
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
