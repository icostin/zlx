#ifndef _ZLX_ARCH_H
#define _ZLX_ARCH_H

/** @defgroup arch Architecture support
 *
 *  This component defines macros that describe the target architecture.
 *
 *  As the library is tested on different platforms, macros will be added
 *  here to describe the underlying architectures
 *
 *  @{
 *
 *  @def ZLX_LITTLE_ENDIAN
 *  Macro defined to 1 when targetting a little endian platform, 0 otherwise.
 *  @warn
 *      Do not probe endianness using ifdef as the macro is always defined,
 *      just use if.
 *
 *  @def ZLX_BIG_ENDIAN
 *  Macro defined to 1 when targetting a big endian platform, 0 otherwise.
 *  @warn
 *      Do not probe endianness using ifdef as the macro is always defined,
 *      just use if.
 *
 *  @def ZLX_UNALIGNED_ACCESS
 *  Macro defined to 1 when targetting a platform that can access unaligned
 *  data, 0 otherwise.
 *  @warn
 *      Do not check using ifdef as the macro is always defined, just use if.
 *
 *  @def ZLX_IA32
 *  Macro defined to 1 when targetting IA32, 0 otherwise.
 *
 *  @def ZLX_AMD64
 *  Macro defined to 1 when targetting AMD64 (or Intel 64, EM64T), 0 otherwise.
 *
 **/

#if !defined(ZLX_IA32) && (defined(_M_IX86) || defined(_M_I86) || defined(__X86__) || defined(_X86_) || defined(__i386__) || defined(__i386))
# define ZLX_IA32 1
#endif

#if !defined(ZLX_AMD64) && (defined(_M_X64) || defined(_M_AMD64) || defined(__amd64__) || defined(__x86_64))
# define ZLX_AMD64 1
#endif


#ifndef ZLX_IA32
# define ZLX_IA32 0
#endif

#ifndef ZLX_AMD64
# define ZLX_AMD64 0
#endif


#if !defined(ZLX_LITTLE_ENDIAN) && (ZLX_IA32 || ZLX_AMD64)
# define ZLX_LITTLE_ENDIAN 1
#endif

#ifndef ZLX_LITTLE_ENDIAN
# define ZLX_LITTLE_ENDIAN 0
#endif

#ifndef ZLX_BIG_ENDIAN
# define ZLX_BIG_ENDIAN 0
#endif



#ifndef ZLX_UNALIGNED_ACCESS
/* autodetect if unaligned accesses are allowed */
# if ZLX_IA32 || ZLX_AMD64
#  define ZLX_UNALIGNED_ACCESS 1
# else
#  define ZLX_UNALIGNED_ACCESS 0
# endif
#endif


/** @def ZLX_BITS
 *  Number of bits for the natural data unit on the target architecture.
 */
#if ZLX_IA32
# define ZLX_BITS 32
#elif ZLX_AMD64
# define ZLX_BITS 64
#else
# error ZLX_BITS unknown for target architecture
#endif


/** @} */

#endif /* _ZLX_ARCH_H */

