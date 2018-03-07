#ifndef _ZLX_OPTIONS_H
#define _ZLX_OPTIONS_H

/** @defgroup options Compile-time options
 *  Macros that affect building or using the library.
 *  All options start with ZLXOPT_ prefix.
 *  @{ */

/*  @def ZLXOPT_DOXYGEN
 *  This is defined as 1 in Doxygen configuration to control how documentation 
 *  is generated. */
#ifndef ZLXOPT_DOXYGEN
# define ZLXOPT_DOXYGEN 0
#endif

/** @def ZLXOPT_STATIC
 *  This must be defined as 1 when building the static library or using the
 *  static library.  */
#ifndef ZLXOPT_STATIC
# define ZLXOPT_STATIC 0
#endif

/** @def ZLXOPT_DYNAMIC
 *  Must be defined as 1 when building the dynamic library. */
#ifndef ZLXOPT_DYNAMIC
# define ZLXOPT_DYNAMIC 0
#endif

/** @def ZLXOPT_DEBUG
 *  When set to non-zero enables debug features of zlx library.
 *  Defaults to 1 only if _DEBUG is defined. */
#ifndef ZLXOPT_DEBUG
# ifdef _DEBUG
#  define ZLXOPT_DEBUG 1
# else
#  define ZLXOPT_DEBUG 0
# endif
#endif

/** @def ZLXOPT_ASSERT
 *  When set to non-zero enables debug features of zlx library.
 *  Defaults to 0 only if NDEBUG is defined. */
#ifndef ZLXOPT_ASSERT
# ifdef NDEBUG
#  define ZLXOPT_ASSERT 0
# else
#  define ZLXOPT_ASSERT 1
# endif
#endif

/** @def ZLXOPT_USE_STDDEF
 *  When non-zero it disables the use of stddef.h even in freestanding mode.
 *  Default is 1 since a standards compliant C compiler should provide stddef.h. */
#ifndef ZLXOPT_USE_STDDEF
# define ZLXOPT_USE_STDDEF 1
#endif

/** @def ZLXOPT_USE_STDINT
 *  When non-zero it disables the use of stdint.h even in freestanding mode.
 *  Default is 1 since a standards compliant C compiler should provide stdint.h. */
#ifndef ZLXOPT_USE_STDINT
# define ZLXOPT_USE_STDINT 1
#endif

/** @def ZLXOPT_FREESTANDING
 *  Affects the platform identification logic in zlx/platform.h to report
 *  ZLX_FREESTANDING as the platform and the ABI corresponding to the
 *  detected platform */
#ifndef ZLXOPT_FREESTANDING
# define ZLXOPT_FREESTANDING 0
#endif

/** @def ZLXOPT_EMIT_BUILTINS
 *  When building the library enables emitting functions for supporting
 *  builtins.
 *  This defaults to 1 when targetting a freestanding platform 
 *  (#ZLXOPT_FREESTANDING != 0)
 */
#ifndef ZLXOPT_EMIT_BUILTINS
# define ZLXOPT_EMIT_BUILTINS ZLXOPT_FREESTANDING
#endif

#ifndef ZLXOPT_USE_BUILTINS_FOR_FFS_FLS
# define ZLXOPT_USE_BUILTINS_FOR_FFS_FLS 1
#endif

/** @} */

#endif /* _ZLX_OPTIONS_H */
