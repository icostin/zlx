#ifndef _ZLX_LIB_H
#define _ZLX_LIB_H

/** @defgroup lib Library identification
 *  Describes features available at run-time and configuration details.
 *  @{
 **/
#include "base.h"

ZLX_CPP_ONLY(extern "C" {)

/* zlx_lib_name *************************************************************/
/**
 *  Library name identifying flavour (static/dynamic, debug/checked/release,
 *  target platform)
 */
ZLX_API extern char const * const zlx_lib_name;

ZLX_CPP_ONLY(})

/** @} */

#endif
