#include "../../include/zlx/memalloc/nop.h"

/* zlx_ma_nop_realloc *******************************************************/
ZLX_API void * ZLX_CALL zlx_ma_nop_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
)
{
    (void) ma;
    return (new_size <= old_size) ? old_ptr : NULL;
}

/* zlx_ma_nop_info_set ******************************************************/
ZLX_API void ZLX_CALL zlx_ma_nop_info_set
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ZLX_RESTRICT ptr,
    char const * ZLX_RESTRICT src,
    unsigned int line,
    char const * ZLX_RESTRICT func,
    char const * ZLX_RESTRICT info
)
{
    (void) ma;
    (void) ptr;
    (void) src;
    (void) line;
    (void) func;
    (void) info;
}


/* zlx_ma_nop_check *********************************************************/
ZLX_API void ZLX_CALL zlx_ma_nop_check
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ZLX_RESTRICT ptr,
    size_t size,
    char const * ZLX_RESTRICT src,
    unsigned int line,
    char const * ZLX_RESTRICT func
)
{
    (void) ma;
    (void) ptr;
    (void) size;
    (void) src;
    (void) line;
    (void) func;
}

ZLX_API zlx_ma_t zlx_ma_nop =
{
    zlx_ma_nop_realloc,
    zlx_ma_nop_info_set,
    zlx_ma_nop_check
};

