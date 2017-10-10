#ifndef _ZLX_LIB_H
#define _ZLX_LIB_H

/** @defgroup lib zlx library
 *  ZLX Library specific macros/variables/functions.
 *  @{ */
#include "options.h"
#include "compiler.h"
#include "arch.h"
#include "language.h"
#include "platform.h"

/** @def ZLX_CALL
 *  Default calling convention for exposed APIs. */
#define ZLX_CALL ZLX_FAST_CALL

/** @def ZLX_API
 *  Declarator prefix for ZLX API functions/variables. */
#if ZLXOPT_STATIC
# define ZLX_API ZLX_LOCAL
#elif ZLXOPT_DYNAMIC
# define ZLX_API ZLX_LIB_EXPORT
#else
# define ZLX_API ZLX_LIB_IMPORT
#endif

ZLX_C_DECL_BEGIN

ZLX_API char const * const zlx_lib_id;

ZLX_C_DECL_END

/** @} */

#endif /* _ZLX_LIB_H */

