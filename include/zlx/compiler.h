#ifndef _ZLX_COMPILER_H
#define _ZLX_COMPILER_H

/** @defgroup compiler Compiler identification
 *  Macros to help with compiler identification.
 *
 *  @{
 *
 *  @def ZLX_CLANG
 *  Macro set to 1 if clang is identified as the compiler, 0 otherwise.
 *
 *  @def ZLX_GCC
 *  Macro set to 1 if gcc is identified as the compiler, 0 otherwise.
 *
 *  @def ZLX_MSC
 *  Macro set to 1 if Microsoft C is identified as the compiler, 0 otherwise.
 *
 *  @def ZLX_UNKNOWN_COMPILER
 *  Macro set to 1 if the compiler is not known, 0 otherwise.
 *
 *  @def ZLX_COMPILER_NAME
 *  String literal representing the identified compiler.
 */

#if defined(__clang__)
# define ZLX_CLANG 1
# define ZLX_COMPILER_NAME "clang"
#elif defined(__GNUC__)
# define ZLX_GCC 1
# define ZLX_COMPILER_NAME "gcc"
#elif defined(_MSC_VER)
# define ZLX_MSC 1
# define ZLX_COMPILER_NAME "Microsoft C"
#else
# define ZLX_UNKNOWN_COMPILER 1
# define ZLX_COMPILER_NAME "unknown-compiler"
#endif

#if !defined(ZLX_CLANG)
# define ZLX_CLANG 0
#endif

#if !defined(ZLX_GCC)
# define ZLX_GCC 0
#endif

#if !defined(ZLX_MSC)
# define ZLX_MSC 0
#endif

#if !defined(ZLX_UNKNOWN_COMPILER)
# define ZLX_UNKNOWN_COMPILER 0
#endif

/** @} */

#endif /* _ZLX_COMPILER_H */
