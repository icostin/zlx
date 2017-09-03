#ifndef _ZLX_PREPROCESSOR_H
#define _ZLX_PREPROCESSOR_H

/* Guess compiler */
#if defined(__clang__)
# define ZLX_CLANG 1
#elif defined(__GNUC__)
# define ZLX_GCC 1
#elif defined(_MSC_VER)
# define ZLX_MSVC 1
#endif

#ifndef ZLX_CLANG
#define ZLX_CLANG 0
#endif

#ifndef ZLX_GCC
#define ZLX_GCC 0
#endif

#ifndef ZLX_MSVC
#define ZLX_MSVC 0
#endif

/* define ZLX_INLINE and ZLX_FORCE_INLINE */
#if ZLX_MSVC
# define ZLX_INLINE __inline
# define ZLX_FORCE_INLINE __forceinline
#else
# define ZLX_INLINE static inline
# define ZLX_FORCE_INLINE static inline __attribute__((always_inline))
#endif

/* define restrict keyword */
#if defined(__STDC_VERSION__) && __STDC_VERSION >= 199901L
# define ZLX_RESTRICT restrict
#elif defined(__GNUC__) && __GNUC__ >= 3
# define ZLX_RESTRICT __restrict__
#elif defined(_MSC_VER) && _MSC_VER >= 1600
# define ZLX_RESTRICT __restrict
#endif

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

#define ZLX_S0(x) #x
#define ZLX_S1(x) ZLX_S0(x)

#define ZLX_TP0(a, b) a##b
#define ZLX_TP1(a, b) ZLX_TP0(a, b)

#define ZLX_LINE_STR ZLX_S1(__LINE__)

#if ZLX_MSVC
# define ZLX_VA_EXPAND(...) ZLX_TP1(__VA_ARGS__,)
#else
# define ZLX_VA_EXPAND(...) __VA_ARGS__
#endif


#if defined(__cplusplus)
# define ZLX_CPP_ONLY(...) ZLX_VA_EXPAND(__VA_ARGS__)
#else
# define ZLX_CPP_ONLY(...)
#endif


#endif

