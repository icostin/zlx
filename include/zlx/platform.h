#ifndef _ZLX_PLATFORM_H
#define _ZLX_PLATFORM_H

#include "arch.h"


#if defined(ZLX_FREESTANDING) && ZLX_FREESTANDING

#else

# undef ZLX_FREESTANDING
# define ZLX_FREESTANDING 0

# if defined(_WIN32)

#  define ZLX_MSWIN 1

# elif defined(_AIX)

#  define ZLX_AIX 1

# elif defined(__unix__) || defined(__unix)

#  define ZLX_UNIX 1

#  include <unistd.h>
#  if defined(POSIX_VERSION)
#   define ZLX_POSIX POSIX_VERSION
#  endif

#  if defined(__DragonFly__)
#   define ZLX_BSD 1
#   define ZLX_DRAGONFLY_BSD 1
#  elif defined(__FreeBSD__)
#   define ZLX_BSD 1
#   define ZLX_FREEBSD 1
#  elif defined(__NetBSD__)
#   define ZLX_BSD 1
#   define ZLX_NETBSD 1
#  elif defined(__OpenBSD__)
#   define ZLX_BSD 1
#   define ZLX_OPENBSD 1
#  endif

# endif /* OS defines */

#endif /* ZLX_FREESTANDING */

/* assume System V ABI if we're not under a Microsoft environment */
#if !defined(ZLX_ABI_SYSV) && !defined(ZLX_ABI_MS)

# if defined(_WIN32)
#  define ZLX_ABI_MS 1
# else
#  define ZLX_ABI_SYSV 1
# endif

#endif

#if ZLX_IA32 && ZLX_ABI_MS
# define ZLX_FAST_CALL __fastcall
#elif ZLX_IA32 && ZLX_ABI_SYSV && (ZLX_GCC || ZLX_CLANG)
# define ZLX_FAST_CALL __attribute__((regparm((3))))
#else
# define ZLX_FAST_CALL
#endif

#endif /* _ZLX_PLATFORM_H */

