#undef _DEBUG

#include "../../include/zlx/memalloc.h"

ZLX_API void * ZLX_CALL ZLX_TP1(PFX, _alloc)
(
    zlx_ma_t * ZLX_RESTRICT ma,
    size_t size
)
{
    return zlxi_alloc(ma, size);
}

ZLX_API void * ZLX_CALL ZLX_TP1(PFX, _realloc)
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * old_ptr,
    size_t old_size,
    size_t new_size
)
{
    return zlxi_realloc(ma, old_ptr, old_size, new_size);
}

ZLX_API void ZLX_CALL ZLX_TP1(PFX, _free)
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size
)
{
    zlxi_free(ma, ptr, size);
}

ZLX_API int ZLX_CALL ZLX_TP1(PFX, _array_alloc)
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
)
{
    return zlxi_array_alloc(ma, ptr, count, new_count, item_size);
}

ZLX_API int ZLX_CALL ZLX_TP1(PFX, _array_realloc)
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
)
{
    return zlxi_array_realloc(ma, ptr, count, new_count, item_size);
}

/* vim: set ft=c : */
