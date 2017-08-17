#ifndef _ZLX_BASE_H
#define _ZLX_BASE_H

#include "preprocessor.h"
#include "arch.h"
#include "platform.h"

/* define ZLX_API */
#if defined(ZLX_STATIC)
# define ZLX_API
#elif defined(ZLX_DYNAMIC)
# define ZLX_API ZLX_LIB_EXPORT
#else
# define ZLX_API ZLX_LIB_IMPORT
#endif


#endif
