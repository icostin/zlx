#include "../../include/zlx/memalloc/tlsf.h"

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
};

ZLX_STATIC_ASSERT(sizeof(void *) == 4 || sizeof(void *) == 8);
static uint8_t const ATOM_SIZE_LOG2 = (sizeof(void *) == 4 ? 3 : 4);

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
    (void) ma_p;
    (void) buffer;
    (void) size;
    (void) max_alloc_size_log2;
    (void) ATOM_SIZE_LOG2;
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

