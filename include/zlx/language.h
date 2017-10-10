#ifndef _ZLX_LANGUAGE_H
#define _ZLX_LANGUAGE_H

/** @defgroup language Language features
 *
 *  Macros for language features.
 *
 *  @{ */

#include "compiler.h"

/* define ZLX_INLINE and ZLX_FORCE_INLINE */
/**
 *  @def ZLX_INLINE
 *  Macro defined to compiler-specific function declarator keywords for
 *  inlining functions.
 *
 *  @def ZLX_FORCE_INLINE
 *  Macro defined to compiler-specific function declarator keywords for
 *  forcing the inlining of functions.
 *  Even if this macro is specified the compiler may choose to not inline the
 *  function.
 */
#if ZLX_MSVC
# define ZLX_INLINE __inline
# define ZLX_FORCE_INLINE __forceinline
#elif defined(ZLX_DOXYGEN)
# define ZLX_INLINE
# define ZLX_FORCE_INLINE
#else
# define ZLX_INLINE static inline
# define ZLX_FORCE_INLINE static inline __attribute__((always_inline))
#endif

/* define restrict keyword */
/**
 *  @def ZLX_RESTRICT
 *  Macro defined to the compiler-specific supported keyword for C99's restrict
 *  keyword.
 *  C++ does not support as standard a restrict keyword but compilers have
 *  their extension for that.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION >= 199901L
# define ZLX_RESTRICT restrict
#elif defined(__GNUC__) && __GNUC__ >= 3
# define ZLX_RESTRICT __restrict__
#elif defined(_MSC_VER) && _MSC_VER >= 1600
# define ZLX_RESTRICT __restrict
#else
# define ZLX_RESTRICT
#endif

/* ZLX_LIB_EXPORT and ZLX_LIB_IMPORT */
/**
 *  @def ZLX_LIB_EXPORT
 *  Compiler-specific variable/function declarator telling the linker that the
 *  symbol should be exported by the binary.
 *  Typically, a dynamic library header checks a macro to detect if it is compiling
 *  the library or a program using the library and decorates all APIs with
 *  #ZLX_LIB_EXPORT when building the library and with #ZLX_LIB_IMPORT when
 *  building the client program.
 *  @code
 *      #if defined(MYLIB_STATIC)
 *      # define MYLIB_API ZLX_LOCAL
 *      #elif defined(MYLIB_DYNAMIC)
 *      # define MYLIB_API ZLX_LIB_EXPORT
 *      #else
 *      # define MYLIB_API ZLX_LIB_IMPORT
 *      #endif
 *      MYLIB_API int mylib_version;
 *      MYLIB_API char * mylib_answer (char * question);
 *  @endcode
 *
 *  @def ZLX_LIB_IMPORT
 *  Compiler-specific variable/function declarator telling the linker that the
 *  symbol should be imported by the binary.
 *  This is used in tandem with #ZLX_LIB_EXPORT.
 */
#if defined(_WIN32)
# define ZLX_LIB_EXPORT __declspec(dllexport)
# define ZLX_LIB_IMPORT __declspec(dllimport)
# define ZLX_LOCAL
#else
# if defined(__GNUC__) && __GNUC__ >= 4
#  define ZLX_LIB_IMPORT __attribute__ ((visibility ("default")))
#  define ZLX_LIB_EXPORT __attribute__ ((visibility ("default")))
#  define ZLX_LOCAL __attribute__ ((visibility ("hidden")))
# else
#  define ZLX_LIB_IMPORT
#  define ZLX_LIB_EXPORT
#  define ZLX_LOCAL
# endif
#endif

/** @def ZLX_S0
 *  Stringize argument.
 */
#define ZLX_S0(x) #x


/** @def ZLX_S1
 *  Nested stringize, useful to expand in preprocessor argument before stringizing.
 */
#define ZLX_S1(x) ZLX_S0(x)

/** @def ZLX_TP0
 *  Token pasting for two tokens.
 */
#define ZLX_TP0(a, b) a##b
/** @def ZLX_TP1
 *  Nested token pasting to allow expansion of arguments.
 */
#define ZLX_TP1(a, b) ZLX_TP0(a, b)

/** @def ZLX_LINE_STR
 *  Current source line as string literal.
 */
#define ZLX_LINE_STR ZLX_S1(__LINE__)

/** @def ZLX_VA_EXPAND(...)
 *  Allows preprocessor to expand arguments.
 */
#if ZLX_MSVC
# define ZLX_VA_EXPAND(...) ZLX_TP1(__VA_ARGS__,)
#else
# define ZLX_VA_EXPAND(...) __VA_ARGS__
#endif

/** @def ZLX_CPP_ONLY(...)
 *  Macro that only expands when compiling a C++ file.
 */
#if defined(__cplusplus)
# define ZLX_CPP_ONLY(...) ZLX_VA_EXPAND(__VA_ARGS__)
#else
# define ZLX_CPP_ONLY(...)
#endif

/** @def ZLX_C_DECL_BEGIN
 *  Macro to mark de start of a C-mode declaration block (functions/globals).
 *  When compiling under C++ this is required to provide symbols from C files.
 *
 *  @def ZLX_C_DECL_END
 *  Macro to mark the end of a C-mode declaration block.
 */
#define ZLX_C_DECL_BEGIN ZLX_CPP_ONLY(extern "C" {)
#define ZLX_C_DECL_END ZLX_CPP_ONLY(})

/** @} */

#endif /* _ZLX_LANGUAGE_H */
