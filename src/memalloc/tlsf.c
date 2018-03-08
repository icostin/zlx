#include "../../include/zlx/memalloc/tlsf.h"
#include <zlx/int/ops.h>

typedef struct tlsf_block tlsf_block_t;
struct tlsf_block
{
    uint8_t * data;
    size_t size;
};

typedef struct tlsf_sep tlsf_sep_t;
struct tlsf_sep
{
    uintptr_t size_and_ctl;
};

typedef struct tlsf_ma tlsf_ma_t;
struct tlsf_ma
{
    zlx_ma_t ma;
    tlsf_block_t * block_table;
    size_t block_count;
    uint8_t row_count;
};

typedef uint32_t stripe_mask_t;

ZLX_STATIC_ASSERT(sizeof(void *) == 4 || sizeof(void *) == 8);
#define ATOM_SIZE_LOG2 4
#define ATOM_SIZE ((size_t) 1 << ATOM_SIZE_LOG2)
#define COLUMN_COUNT_LOG2 5
#define COLUMN_COUNT (1 << COLUMN_COUNT_LOG2)
#define COLUMN_MASK (COLUMN_COUNT - 1)

/****************************************************************************/

/* zlx_tlsf_create **********************************************************/
ZLX_API zlx_tlsf_status_t ZLX_CALL zlx_tlsf_create
(
    zlx_ma_t * * ZLX_RESTRICT ma_p,
    void * buffer,
    size_t size,
    uint8_t max_alloc_size_log2
)
{
    uint8_t row_count;

    (void) ma_p;
    (void) buffer;
    (void) size;
    (void) max_alloc_size_log2;
    (void) ATOM_SIZE_LOG2;

    if (max_alloc_size_log2 > sizeof(void *) * 8)
        max_alloc_size_log2 = sizeof(void *) * 8;

    row_count = max_alloc_size_log2 <= ATOM_SIZE_LOG2 + COLUMN_COUNT_LOG2
        ? 1 : max_alloc_size_log2 - (ATOM_SIZE_LOG2 + COLUMN_COUNT_LOG2);
    (void) row_count;


    return ZLX_TLSF_NO_SUP;
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

    q = zlx_u32_mssb((uint32_t) (atom_count >> COLUMN_COUNT_LOG2));
    // q = 9
    return zlx_size_to_uint((q << COLUMN_COUNT_LOG2) + (atom_count >> q));
}

/* zlx_tlsf_cell_to_size ****************************************************/
ZLX_API size_t zlx_tlsf_cell_to_size
(
    unsigned int cell
)
{
    unsigned int q;
    if (cell < COLUMN_COUNT) return zlx_uint_to_size(cell << ATOM_SIZE_LOG2);
    q = (cell >> COLUMN_COUNT_LOG2) - 1;
    return zlx_uint_to_size(COLUMN_COUNT | (cell & COLUMN_MASK))
        << (q + ATOM_SIZE_LOG2);
}

