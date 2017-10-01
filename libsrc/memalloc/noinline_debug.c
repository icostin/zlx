#undef NDEBUG
#undef _DEBUG
#define _DEBUG

#include "../../include/zlx/memalloc.h"

/* zlxnid_alloc *************************************************************/
ZLX_API void * ZLX_CALL zlxnid_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    size_t size
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
)
{
    return zlxi_alloc(ma, size, src, line, func, info);
}

/* zlxnid_realloc ***********************************************************/
ZLX_API void * ZLX_CALL zlxnid_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * old_ptr,
    size_t old_size,
    size_t new_size
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
)
{
    return zlxi_realloc(ma, old_ptr, old_size, new_size, src, line, func, info);
}

/* zlxnid_free **************************************************************/
ZLX_API void ZLX_CALL zlxnid_free
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size
    , char const * src
    , unsigned int line
    , char const * func
)
{
    zlxi_free(ma, ptr, size, src, line, func);
}

/* zlxnid_array_alloc *******************************************************/
ZLX_API int ZLX_CALL zlxnid_array_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
)
{
    return zlxi_array_alloc(ma, ptr, count, new_count, item_size,
                            src, line, func, info);
}

/* zlxnid_array_realloc *****************************************************/
ZLX_API int ZLX_CALL zlxnid_array_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
)
{
    return zlxi_array_realloc(ma, ptr, count, new_count, item_size,
                              src, line, func, info);
}

