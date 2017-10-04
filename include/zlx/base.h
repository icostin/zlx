#ifndef _ZLX_BASE_H
#define _ZLX_BASE_H

/** @defgroup base Base definitions
 *
 *  All zlx headers that define types or APIs depend on zlx/base.h to define how APIs are exported.
 *  @{
 */

#include "preprocessor.h"
#include "arch.h"
#include "platform.h"

/** @def ZLX_API
 *  Makes the function/variable symbol public to allow linking with the zlx library.
 */
#if defined(ZLX_STATIC)
# define ZLX_API
#elif defined(ZLX_DYNAMIC)
# define ZLX_API ZLX_LIB_EXPORT
#else
# define ZLX_API ZLX_LIB_IMPORT
#endif

/** @def ZLX_CALL
 *  Macro representing the calling conventiom of all APIs offered by this
 *  library.
 */
#define ZLX_CALL ZLX_FAST_CALL

#ifdef ZLXOPT_USE_NON_INLINED_API
# define _ZLXI_NAME(n) zlxni##n
#else
# define _ZLXI_NAME(n) zlxi##n
#endif

/** @} */

#endif
