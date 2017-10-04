#ifndef _ZLX_ASSERT_H
#define _ZLX_ASSERT_H

/** @defgroup Assert
 *
 *  Basic assert support.
 *
 *  @{
 */

#include "base.h"
#include "obstream/base.h"

ZLX_CPP_ONLY(extern "C" {)

/* zlx_assert_log */
/**
 * Output byte stream used to log a failed assertion.
 */
extern ZLX_API zlx_obstream_t * zlx_assert_log;

/*  zlx_abort  */
/**
 *  Abort function to be called when an assertion fails.
 */
extern ZLX_API void (* zlx_abort) (void);

/** @def ZLX_ASSERT(cond)
 *  This macro inserts a check that the given condition is true and aborts if 
 *  not.
 *  If NDEBUG is defined then no check is performed (similar to standard C's 
 *  assert function).
 *
 *  @hideinitializer
 */
#if NDEBUG && !defined ZLXOPT_ENABLE_ASSERT
# define ZLX_ASSERT(cond) ((void) 0)
#else
# define _ZLX_ASSERT_MSG(cond) (__FILE__ ":" ZLX_LINE_STR ": *** ASSERTION FAILED: " #cond)
# define ZLX_ASSERT(cond) ((cond) ? (void) 0 : ((void) zlx_obstream_write(zlx_assert_log, (uint8_t const *) _ZLX_ASSERT_MSG(cond), sizeof(_ZLX_ASSERT_MSG(cond)) - 1), zlx_abort()))
#endif

ZLX_CPP_ONLY(})

/** @} */

#endif /* _ZLX_ASSERT_H */
