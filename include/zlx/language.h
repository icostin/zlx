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
#if ZLX_MSC
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
 *  their extension for it.
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
 *  @def ZLX_LOCAL
 *  Compiler-specific global symbol declarator that specifies the symbol is
 *  visible to other compilation units in the same module, but it is not
 *  visible to other modules.
 *
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
#if ZLX_MSC
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
 *  Macro to mark the start of a C-mode declaration block (functions/globals).
 *  When compiling under C++ this is required to provide symbols from C files.
 *
 *  @def ZLX_C_DECL_END
 *  Macro to mark the end of a C-mode declaration block.
 */
#define ZLX_C_DECL_BEGIN ZLX_CPP_ONLY(extern "C" {)
#define ZLX_C_DECL_END ZLX_CPP_ONLY(})

#define ZLX_ITEM_COUNT(array) (sizeof(array) / sizeof((array)[0]))


#define _ZLX_VA_COUNT_M1(ignore, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, n, ...) n
#define ZLX_VA_COUNT_M1(...) ZLX_VA_EXPAND(_ZLX_VA_COUNT_M1(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define _ZLX_VA_FOR_EACH_0(m, c, ...)
#define _ZLX_VA_FOR_EACH_1(m, c, a0, ...) m(c, a0)
#define _ZLX_VA_FOR_EACH_2(m, c, a0, a1, ...) m(c, a0) m(c, a1)
#define _ZLX_VA_FOR_EACH_3(m, c, a0, a1, a2, ...) m(c, a0) m(c, a1) m(c, a2)
#define _ZLX_VA_FOR_EACH_4(m, c, a0, a1, a2, a3, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3)
#define _ZLX_VA_FOR_EACH_5(m, c, a0, a1, a2, a3, a4, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4)
#define _ZLX_VA_FOR_EACH_6(m, c, a0, a1, a2, a3, a4, a5, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5)
#define _ZLX_VA_FOR_EACH_7(m, c, a0, a1, a2, a3, a4, a5, a6, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6)
#define _ZLX_VA_FOR_EACH_8(m, c, a0, a1, a2, a3, a4, a5, a6, a7, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7)
#define _ZLX_VA_FOR_EACH_9(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8)
#define _ZLX_VA_FOR_EACH_10(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9)
#define _ZLX_VA_FOR_EACH_11(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10)
#define _ZLX_VA_FOR_EACH_12(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11)
#define _ZLX_VA_FOR_EACH_13(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12)
#define _ZLX_VA_FOR_EACH_14(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13)
#define _ZLX_VA_FOR_EACH_15(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14)
#define _ZLX_VA_FOR_EACH_16(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15)
#define _ZLX_VA_FOR_EACH_17(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16)
#define _ZLX_VA_FOR_EACH_18(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17)
#define _ZLX_VA_FOR_EACH_19(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18)
#define _ZLX_VA_FOR_EACH_20(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19)
#define _ZLX_VA_FOR_EACH_21(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20)
#define _ZLX_VA_FOR_EACH_22(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21)
#define _ZLX_VA_FOR_EACH_23(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22)
#define _ZLX_VA_FOR_EACH_24(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23)
#define _ZLX_VA_FOR_EACH_25(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24)
#define _ZLX_VA_FOR_EACH_26(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25)
#define _ZLX_VA_FOR_EACH_27(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26)
#define _ZLX_VA_FOR_EACH_28(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27)
#define _ZLX_VA_FOR_EACH_29(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28)
#define _ZLX_VA_FOR_EACH_30(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29)
#define _ZLX_VA_FOR_EACH_31(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30)
#define _ZLX_VA_FOR_EACH_32(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31)
#define _ZLX_VA_FOR_EACH_33(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32)
#define _ZLX_VA_FOR_EACH_34(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33)
#define _ZLX_VA_FOR_EACH_35(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34)
#define _ZLX_VA_FOR_EACH_36(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35)
#define _ZLX_VA_FOR_EACH_37(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36)
#define _ZLX_VA_FOR_EACH_38(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37)
#define _ZLX_VA_FOR_EACH_39(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38)
#define _ZLX_VA_FOR_EACH_40(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39)
#define _ZLX_VA_FOR_EACH_41(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40)
#define _ZLX_VA_FOR_EACH_42(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41)
#define _ZLX_VA_FOR_EACH_43(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42)
#define _ZLX_VA_FOR_EACH_44(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43)
#define _ZLX_VA_FOR_EACH_45(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44)
#define _ZLX_VA_FOR_EACH_46(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45)
#define _ZLX_VA_FOR_EACH_47(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46)
#define _ZLX_VA_FOR_EACH_48(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47)
#define _ZLX_VA_FOR_EACH_49(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48)
#define _ZLX_VA_FOR_EACH_50(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49)
#define _ZLX_VA_FOR_EACH_51(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50)
#define _ZLX_VA_FOR_EACH_52(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51)
#define _ZLX_VA_FOR_EACH_53(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52)
#define _ZLX_VA_FOR_EACH_54(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53)
#define _ZLX_VA_FOR_EACH_55(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54)
#define _ZLX_VA_FOR_EACH_56(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55)
#define _ZLX_VA_FOR_EACH_57(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55) m(c, a56)
#define _ZLX_VA_FOR_EACH_58(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55) m(c, a56) m(c, a57)
#define _ZLX_VA_FOR_EACH_59(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55) m(c, a56) m(c, a57) m(c, a58)
#define _ZLX_VA_FOR_EACH_60(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55) m(c, a56) m(c, a57) m(c, a58) m(c, a59)
#define _ZLX_VA_FOR_EACH_61(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55) m(c, a56) m(c, a57) m(c, a58) m(c, a59) m(c, a60)
#define _ZLX_VA_FOR_EACH_62(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55) m(c, a56) m(c, a57) m(c, a58) m(c, a59) m(c, a60) m(c, a61)
#define _ZLX_VA_FOR_EACH_63(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55) m(c, a56) m(c, a57) m(c, a58) m(c, a59) m(c, a60) m(c, a61) m(c, a62)
#define _ZLX_VA_FOR_EACH_64(m, c, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, ...) m(c, a0) m(c, a1) m(c, a2) m(c, a3) m(c, a4) m(c, a5) m(c, a6) m(c, a7) m(c, a8) m(c, a9) m(c, a10) m(c, a11) m(c, a12) m(c, a13) m(c, a14) m(c, a15) m(c, a16) m(c, a17) m(c, a18) m(c, a19) m(c, a20) m(c, a21) m(c, a22) m(c, a23) m(c, a24) m(c, a25) m(c, a26) m(c, a27) m(c, a28) m(c, a29) m(c, a30) m(c, a31) m(c, a32) m(c, a33) m(c, a34) m(c, a35) m(c, a36) m(c, a37) m(c, a38) m(c, a39) m(c, a40) m(c, a41) m(c, a42) m(c, a43) m(c, a44) m(c, a45) m(c, a46) m(c, a47) m(c, a48) m(c, a49) m(c, a50) m(c, a51) m(c, a52) m(c, a53) m(c, a54) m(c, a55) m(c, a56) m(c, a57) m(c, a58) m(c, a59) m(c, a60) m(c, a61) m(c, a62) m(c, a63)
#define ZLX_VA_FOR_EACH(macro, /* context, args... */ ...) \
    ZLX_VA_EXPAND(ZLX_TP1(_ZLX_VA_FOR_EACH_, ZLX_VA_COUNT_M1(__VA_ARGS__))(macro, __VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,))

#define _ZLX_ENUM_STR(c, v) ,ZLX_S1(v)

/* ZLX_ENUM_DECL ************************************************************/
/**
 *  Declares a simple enum and a helper function to convert to string.
 *  @note
 *      Due to the preprocessor macros used to generate the code the enum
 *      is limited to 64 values.
 */
#define ZLX_ENUM_DECL(type, prefix, seed, first, ...) \
    enum prefix##_enum { first = seed, __VA_ARGS__ }; \
    ZLX_INLINE char const * prefix##_as_str (enum prefix##_enum e) { \
        static char const * const t[] = { \
            ZLX_S1(first) ZLX_VA_FOR_EACH(_ZLX_ENUM_STR, x, __VA_ARGS__) \
        }; \
        unsigned int i = (unsigned int) e - first; \
        if (i >= ZLX_ITEM_COUNT(t)) return "<unknown-" #prefix ">"; \
        return t[i]; \
    } \
    typedef enum prefix##_enum type

#define ZLX_STATIC_ASSERT(cond) \
    typedef int ZLX_TP1(_zlx_static_assert_, __COUNTER__)[1 - 2 * !(cond)]

/** @} */

#endif /* _ZLX_LANGUAGE_H */
