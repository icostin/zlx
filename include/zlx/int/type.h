#ifndef _ZLX_INT_TYPE_H
#define _ZLX_INT_TYPE_H

/** @addtogroup int Integer
 *
 *  Including <zlx/int/type.h> makes available the following standard int types:
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

#include "../options.h"

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

typedef uint8_t zlx_u8_t;
typedef uint16_t zlx_u16_t;
typedef uint32_t zlx_u32_t;
typedef uint64_t zlx_u64_t;
typedef int8_t zlx_s8_t;
typedef int16_t zlx_s16_t;
typedef int32_t zlx_s32_t;
typedef int64_t zlx_s64_t;
typedef size_t zlx_size_t, zlx_usize_t;
typedef ptrdiff_t zlx_ptrdiff_t, zlx_ssize_t;
typedef uintptr_t zlx_uptr_t;
typedef intptr_t zlx_sptr_t;
typedef unsigned char zlx_uchar_t;
typedef signed char zlx_char_t, zlx_schar_t;
typedef unsigned short int zlx_ushort_t;
typedef signed short int zlx_sshort_t, zlx_short_t;
typedef unsigned int zlx_uint_t;
typedef signed int zlx_sint_t, zlx_int_t;
typedef unsigned long int zlx_ulong_t;
typedef signed long int zlx_slong_t, zlx_long_t;

/** @} */

#endif /* _ZLX_INT_TYPE_H */
