#ifndef _ZLX_OPTIONS_H
#define _ZLX_OPTIONS_H

/** @defgroup options ZLX compile-time options
 *  Macros that affect building or using the library.
 *  All options start with ZLXOPT_ prefix.
 *  @{ */

/** When defined it disables the use of stddef.h even in freestanding mode */
#define ZLXOPT_NO_STDDEF

/** When defined it isables the use of stdint.h even in freestanding mode */
#define ZLXOPT_NO_STDINT

/** Affects the platform identification logic in zlx/platform.h to report
 *  ZLX_FREESTANDING as the platform and the ABI corresponding to the
 *  detected platform */
#define ZLXOPT_FREESTANDING

/** @} */

#endif /* _ZLX_OPTIONS_H */
