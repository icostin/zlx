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

/* define ZLX_CALL to whatever "fast" calling convention the target 
 * platform has */
/*  ZLX_CALL  */
/**
 *  macro representing the calling conventiom of all APIs offered
 *  by this library.
 */
#define ZLX_CALL ZLX_FAST_CALL


#endif
