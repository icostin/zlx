#ifndef _ZLX_INT_ARRAY_H
#define _ZLX_INT_ARRAY_H

#include "../lib.h"
#include "type.h"

/** @defgroup int_array Integer Array support
 *  Provides support for working with arrays of various integer types.
 *  @{ */

ZLX_C_DECL_BEGIN

#define T uint8_t
#define F(_n) zlx_u8a_##_n
#define CMP(a, b) ((int) (a) - (int) (b))
#define EQU(a, b) ((a) == (b))
#define FDP ZLX_API
#define CCONV ZLX_CALL
#include "../array.h"

/** Zeroes out the given byte array. */
ZLX_INLINE void zlx_u8a_zero (uint8_t * ZLX_RESTRICT array, size_t count);

/** Fills a byte array with a given value. */
ZLX_API void ZLX_CALL zlx_u8a_fill 
(
    uint8_t * ZLX_RESTRICT array,
    size_t count,
    uint8_t value
);

/** Copies a byte array to a non-overlapping location. */
ZLX_API void ZLX_CALL zlx_u8a_copy
(
    uint8_t * ZLX_RESTRICT dest,
    uint8_t const * ZLX_RESTRICT src,
    size_t count
);

/** Copies a zero-terminated byte array to a non-overlapping location. 
 *  @returns the location of the terminating zero in the destination
 */
ZLX_API uint8_t * ZLX_CALL zlx_u8a_zcopy
(
    uint8_t * ZLX_RESTRICT dest,
    uint8_t const * ZLX_RESTRICT src
);

/** Compares two byte-arrays.
 *  @returns 0 for equal, negative for @a a less than @a b, positive for @a a
 *  greater than @a b
 */
ZLX_API int ZLX_CALL zlx_u8a_cmp
(
    uint8_t const * a,
    uint8_t const * b,
    size_t n
);

/** Compares two zero-terminated byte arrays.
 *  @returns 0 for equal, negative for @a a less than @a b, positive for @a a
 *  greater than @a b
 */
ZLX_API int ZLX_CALL zlx_u8a_zcmp
(
    uint8_t const * a,
    uint8_t const * b
);

/** Scans for a byte value. */
ZLX_API uint8_t * ZLX_CALL zlx_u8a_scan
(
    uint8_t const * ptr,
    uint8_t value
);

/** Computes length of zero-terminated byte array. */
ZLX_API size_t ZLX_CALL zlx_u8a_zlen (uint8_t const * ZLX_RESTRICT a);

/** Searches for a byte */
ZLX_API uint8_t * ZLX_CALL zlx_u8a_search
(
    uint8_t const * begin,
    uint8_t const * end,
    uint8_t value
);

/** Makes space inside an array for inserting bytes.
 *  Shifts tail elements to make room for placing new bytes at the given 
 *  location. If there is not enough room to extend the array is 
 *  reallocated to accommodate the new chunk.
 *  @note
 *      this function does not set the values in the locations prepared
 *      for insertion
 *
 *  @param ap [in, out]
 *      pointer to the array base pointer;
 *      the pointer should be obtained from a previous allocation using
 *      the given allocator, or the allocator should always fail
 *      reallocations.
 *  @param np [in, out]
 *      pointer to the number of used elements in the array
 *  @param mp [in, out]
 *      pointer to the number of allocated elements for the array (capacity)
 *  @param p [in]
 *      index where to insert
 *  @param q [in]
 *      number of bytes to insert
 *  @returns 
 *      a pointer to the start of the inserted location or NULL on error;
 *      basically, on success the return value is (*ap + p)
 *
 */
ZLX_API uint8_t * ZLX_CALL zlx_u8a_insert
(
    uint8_t * ZLX_RESTRICT * ZLX_RESTRICT ap,
    size_t * ZLX_RESTRICT np,
    size_t * ZLX_RESTRICT mp,
    size_t p,
    size_t q,
    zlx_ma_t * ma
);

ZLX_C_DECL_END
/** @} */

#endif /* _ZLX_INT_ARRAY_H */
