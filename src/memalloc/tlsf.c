#include "../../include/zlx/memalloc/tlsf.h"
#include "../../include/zlx/int/ops.h"

ZLX_STATIC_ASSERT(sizeof(void *) == 4 || sizeof(void *) == 8);
#define ATOM_SIZE_LOG2 4
#define ATOM_SIZE ((size_t) 1 << ATOM_SIZE_LOG2)
#define COLUMN_COUNT_LOG2 5
#define COLUMN_COUNT (1 << COLUMN_COUNT_LOG2)
#define COLUMN_MASK (COLUMN_COUNT - 1)

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
    size_t max_alloc_size;
    size_t block_count;
    uint8_t row_count;
};

static size_t const ALLOC_SIZE_LIMIT =
    ((size_t) 1 << (sizeof(void *) * 8 - 1)) - 1;


/****************************************************************************/

static uint8_t compute_row_count
(
    size_t max_alloc_atoms
)
{
    zlx_uint_t q;

    ZLX_ASSERT(max_alloc_atoms > 0);
    ZLX_ASSERT(max_alloc_atoms <= (ALLOC_SIZE_LIMIT >> ATOM_SIZE_LOG2));

    q = zlx_size_mssb(max_alloc_atoms);
    return q < COLUMN_COUNT_LOG2 ? 1 : zlx_uint_to_u8(q - COLUMN_COUNT_LOG2 + 2);
}

/* zlx_tlsf_create **********************************************************/
ZLX_API zlx_tlsf_status_t ZLX_CALL zlx_tlsf_create
(
    zlx_ma_t * * ZLX_RESTRICT ma_p,
    void * buffer,
    size_t size,
    size_t max_alloc_size
)
{
    uint8_t row_count;
    size_t max_alloc_atoms;
    size_t min_buffer_size;
    tlsf_ma_t * tma;

    (void) ma_p;
    (void) buffer;
    (void) size;
    if (max_alloc_size == 0 || max_alloc_size > ALLOC_SIZE_LIMIT)
        return ZLX_TLSF_BAD_MAX;

    max_alloc_atoms = (max_alloc_size + ATOM_SIZE - 1) >> ATOM_SIZE_LOG2;
    row_count = compute_row_count(max_alloc_atoms);
    min_buffer_size = sizeof(tlsf_ma_t) 
        + (size_t) row_count * COLUMN_COUNT * sizeof(void *)
        + ATOM_SIZE * 3;

    if (size < min_buffer_size) 
        return ZLX_TLSF_BUFFER_TOO_SMALL;

    tma = (tlsf_ma_t *) buffer;

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

