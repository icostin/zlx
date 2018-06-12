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
              ZLX_TLSF_BUFFER_TOO_LARGE,
              ZLX_TLSF_BAD_BUFFER,
              ZLX_TLSF_BAD_MAX,
              ZLX_TLSF_NO_SUP);

/* ZLX_TLSF_BLOCK_LIMIT *****************************************************/
/**
 *  Maximum size that can be allocated in a single alloc call.
 */
#define ZLX_TLSF_BLOCK_LIMIT (((size_t) 1 << (sizeof(void *) * 8 - 1)) - 1)

/* zlx_tlsf_block_t *********************************************************/
/**
 *  Represents a block of memory used by a TLSF heap.
 *  See zlx_tlsf_get_blocks().
 */
typedef struct zlx_tlsf_block zlx_tlsf_block_t;
struct zlx_tlsf_block
{
    uint8_t * data;
    size_t size;
};

/* zlx_tlsf_create **********************************************************/
/**
 *  Creates a TLSF heap.
 *  @param ma_p [out]
 *      receives a pointer to the memory allocator instance
 *  @param buffer [in]
 *      buffer to hold the allocator instance and the data for allocations
 *  @param size [in]
 *      size of buffer
 *  @param max_block_size [in]
 *      max size that can be requested in a single malloc call
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
    size_t max_block_size
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

/* zlx_tlsf_size_to_cell ****************************************************/
/**
 *  Computes which TLSF cell contains blocks of the given size.
 */
ZLX_API unsigned int ZLX_CALL zlx_tlsf_size_to_cell
(
    size_t size
);

/* zlx_tlsf_cell_to_size ****************************************************/
/**
 *  Returns the lowest size belonging to the given TLSF cell.
 */
ZLX_API size_t ZLX_CALL zlx_tlsf_cell_to_size
(
    unsigned int cell
);

/* zlx_tlsf_walk ************************************************************/
/**
 *  Traverses the heap attempting to detect corruptions.
 *  @retval 0 no corruptions detected
 *  @retval 1 heap corrupted
 */
ZLX_API int ZLX_CALL zlx_tlsf_walk
(
    zlx_ma_t * ma
);

/* zlx_tlsf_get_blocks ******************************************************/
/**
 *  Retrieves the list of blocks in use by the given TLSF heap.
 *
 *  This list contains the pairs pointer and size exactly as they were passed 
 *  to zlx_tlsf_create() and each successful call to zlx_tlsf_add_block().
 *
 *  The returned array is guaranteed valid until zlx_tlsf_add_block() or one 
 *  of current blocks is freed. Normal heap allocations/deallocations do not
 *  affect this array.
 *
 *  @param ma [in]
 *      TLSF heap; undefined behavior if this is not a TLSF heap (obtained
 *      from zlx_tlsf_create())
 *  @param blocks [out]
 *      receives a pointer to an array of block structures
 *  @returns the number of blocks in the heap
 */
ZLX_API size_t ZLX_CALL zlx_tlsf_get_blocks
(
    zlx_ma_t const * ma,
    zlx_tlsf_block_t * * blocks
);

ZLX_C_DECL_END

/** @} */

#endif
