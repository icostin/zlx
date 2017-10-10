#ifndef _ZLX_PLATFORM_H
#define _ZLX_PLATFORM_H

/** @defgroup platform Platform
 *  Target platform and ABI identification
 *  @{ 
 *
 *  @def ZLX_FREESTANDING
 *  Defined as 1 if a freestanding target platform is determined or 
 *  #ZLXOPT_FREESTANDING is specified, not defined otherwise.
 *
 *  @def ZLX_MSWIN
 *  Defined as 1 only when Microsoft Windows is targetted.
 *
 *  @def ZLX_AIX
 *  Defined as 1 only when AIX is targetted.
 *
 *  @def ZLX_UNIX
 *  Defined as 1 only when a Unix flavor is targetted.
 *
 *  @def ZLX_LINUX
 *  Defined as 1 only when Linux (GNU / Android / etc) is targetted.
 *
 *  @def ZLX_BSD
 *  Defined as 1 only when a BSD flavor is targetted.
 *  
 *  @def ZLX_DRAGONFLY_BSD
 *  Defined as 1 only when Dragonfly BSD is targetted.
 *
 *  @def ZLX_FREEBSD
 *  Defined as 1 only when FreeBSD is targetted.
 *
 *  @def ZLX_NETBSD
 *  Defined as 1 only when NetBSD is targetted.
 *
 *  @def ZLX_OPENBSD
 *  Defined as 1 only when OpenBSD is targetted.
 **/

#include "options.h"

#if ZLXOPT_FREESTANDING

#define ZLX_FREESTANDING 1

#else

#define ZLX_FREESTANDING 0

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

#  if defined(__linux__)
#   define ZLX_LINUX 1
#  elif defined(__DragonFly__)
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
# endif

#endif /* ZLXOPT_FREESTANDING */

#ifndef ZLX_MSWIN
# define ZLX_MSWIN 0
#endif

#ifndef ZLX_AIX
# define ZLX_AIX 0
#endif

#ifndef ZLX_UNIX
# define ZLX_UNIX 0
#endif

#ifndef ZLX_LINUX
# define ZLX_LINUX 0
#endif

#ifndef ZLX_BSD
# define ZLX_BSD 0
#endif

#ifndef ZLX_DRAGONFLY_BSD
# define ZLX_DRAGONFLY_BSD 0
#endif

#ifndef ZLX_FREEBSD
# define ZLX_FREEBSD 0
#endif

#ifndef ZLX_NETBSD
# define ZLX_NETBSD 0
#endif

#ifndef ZLX_OPENBSD
# define ZLX_OPENBSD 0
#endif

/** @} */

#endif /* _ZLX_PLATFORM_H */
