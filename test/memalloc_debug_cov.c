#undef ZLXOPT_ASSERT
#define ZLXOPT_ASSERT 1

#undef ZLXOPT_DEBUG
#define ZLXOPT_DEBUG 1

#include <stdio.h>
#include "../include/zlx/memalloc/interface.h"

void * cov_dbg_alloc 
(
    zlx_ma_t * ZLX_RESTRICT ma,
    size_t size,
    char const * info
)
{
    return zlx_alloc(ma, size, info);
}

void * cov_dbg_realloc 
(
    zlx_ma_t * ZLX_RESTRICT ma, 
    void * old_ptr,
    size_t old_size,
    size_t new_size
)
{
    return zlx_realloc(ma, old_ptr, old_size, new_size);
}

void cov_dbg_free
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size
)
{
    zlx_free(ma, ptr, size);
}


