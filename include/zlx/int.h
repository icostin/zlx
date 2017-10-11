#ifndef _ZLX_INT_H
#define _ZLX_INT_H

/** @defgroup int Integer
 *
 *  Various helper macros and functions that operate on integers.
 *
 *  Including <zlx/int.h> makes available the following standard int types:
 *  - uint8_t
 *  - uint16_t
 *  - uint32_t
 *  - uint64_t
 *  - uint_least8_t
 *  - uint_least16_t
 *  - uint_least32_t
 *  - uint_least64_t
 *  - uint_fast8_t
 *  - uint_fast16_t
 *  - uint_fast32_t
 *  - uint_fast64_t
 *  - size_t
 *  - ptrdiff_t
 *  - intptr_t
 *  - uintptr_t
 *
 *  @note
 *      ssize_t is not necessarely defined as it is a POSIX type.
 *
 *  @{
 */

#include "options.h"

#if ZLXOPT_USE_STDINT
# include <stdint.h>
#else
# error todo: mimick stdint.h
#endif

#if ZLXOPT_USE_STDDEF
# include <stddef.h>
#else
# error todo: mimick stddef.h
#endif

/** @} */

#endif /* _ZLX_INT_H */
