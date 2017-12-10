#ifndef _ZLX_MA_NOP_H
#define _ZLX_MA_NOP_H

/** @defgroup ma_nop NOP Memory allocator 
 *  Dummy memory allocator that does not allocate memory but implements the 
 *  allocator interface functions to do nothing. 
    @{
 **/

#include "interface.h"

ZLX_C_DECL_BEGIN

/* zlx_ma_nop_realloc *******************************************************/
/**
 *  Reallocator function that never allocates.
 *  @returns same pointer if the new size is at most the old size.
 */
ZLX_API void * ZLX_CALL zlx_ma_nop_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
);

ZLX_API int ZLX_CALL zlx_ma_nop_contains
(
    void * ptr,
    size_t size,
    zlx_ma_t * ZLX_RESTRICT ma
);

ZLX_API void ZLX_CALL zlx_ma_nop_info_set
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ZLX_RESTRICT ptr,
    char const * ZLX_RESTRICT src,
    unsigned int line,
    char const * ZLX_RESTRICT func,
    char const * ZLX_RESTRICT info
);

ZLX_API void ZLX_CALL zlx_ma_nop_check
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ZLX_RESTRICT ptr,
    size_t size,
    char const * ZLX_RESTRICT src,
    unsigned int line,
    char const * ZLX_RESTRICT func
);

ZLX_API zlx_ma_t zlx_ma_nop;

ZLX_C_DECL_END

/** @} */

#endif
