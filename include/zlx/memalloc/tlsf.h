#ifndef _ZLX_TLSF_H
#define _ZLX_TLSF_H

/** @defgroup tlsf Two Level Segregated Fit memory allocator
 *
 *  @{
 */

#include "../int/type.h"
#include "interface.h"

ZLX_C_DECL_BEGIN

ZLX_ENUM_DECL(zlx_tlsf_status_t, zlx_tlsf_status, 0,
              ZLX_TLSF_OK,
              ZLX_TLSF_BUFFER_TOO_SMALL,
              ZLX_TLSF_NO_SUP);

/* zlx_tlsf_create **********************************************************/
/**
 *  Creates a TLSF heap.
 *  @param ma_p [out]
 *      receives a pointer to the memory allocator instance
 *  @param buffer [in]
 *      buffer to hold the allocator instance and the data for allocations
 *  @param size [in]
 *      size of buffer
 *  @retval ZLX_TLSF_OK
 *      @a ma_p initialized
 *  @retval ZLX_TLSF_BUFFER_TOO_SMALL
 *      buffer to small for heap control structures
 *  @warning
 *      the allocator does not implement any synchronization for thread-safety;
 *      if multithreaded use is desired operations should be guarded by a
 *      mutex
 */
ZLX_API zlx_tlsf_status_t ZLX_CALL zlx_tlsf_create
(
    zlx_ma_t * * ZLX_RESTRICT ma_p,
    void * buffer,
    size_t size,
    uint8_t max_alloc_size_log2
);

/* zlx_tlsf_add_block *******************************************************/
/**
 *  Adds a new block to an existing heap.
 *  @param ma [in|out]
 *      allocator instance obtained from a previous zlx_tlsf_create()
 *  @param buffer [in]
 *      new buffer to be added to the pool of buffers
 *  @param size [in]
 *      size of buffer
 *  @retval ZLX_TLSF_OK
 *      @a ma_p initialized
 *  @retval ZLX_TLSF_BUFFER_TOO_SMALL
 *      buffer to small to support even a single allocation
 *  @retval ZLX_TLSF_NO_SUP
 *      feature not supported
 */
ZLX_API zlx_tlsf_status_t ZLX_CALL zlx_tlsf_add_block
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * buffer,
    size_t size
);

ZLX_API unsigned int zlx_tlsf_size_to_cell
(
    size_t size
);

ZLX_API size_t zlx_tlsf_cell_to_size
(
    unsigned int cell
);

ZLX_C_DECL_END

/** @} */

#endif
