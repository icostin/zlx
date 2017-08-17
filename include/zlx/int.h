#ifndef _ZLX_INT_H
#define _ZLX_INT_H

#include "base.h"

#if !defined(ZLX_USE_STDDEF)
# if defined(_MSC_VER) && _MSC_VER < 1900
#  define ZLX_USE_STDDEF 0
# else
#  define ZLX_USE_STDDEF 1
# endif
#endif


#if !defined(ZLX_USE_STDINT)
# if defined(_MSC_VER) && _MSC_VER < 1900
#  define ZLX_USE_STDINT 0
# else
#  define ZLX_USE_STDINT 1
# endif
#endif

#if ZLX_USE_STDDEF
# include <stddef.h>
#else
# error todo: implement stddef replacement
#endif

#if ZLX_USE_STDINT
# include <stdint.h>
#else
# error todo: implement stdint replacement
#endif

#endif /* _ZLX_INT_H */
