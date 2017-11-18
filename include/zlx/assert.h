#ifndef _ZLX_ASSERT_H
#define _ZLX_ASSERT_H

/** @defgroup assert Assert
 *
 *  Basic assert support.
 *
 *  @{
 */

#include "options.h"
#include "lib.h"
#include "writer/interface.h"

ZLX_C_DECL_BEGIN

/** @def ZLX_ASSERT(cond)
 *  This macro inserts a check that the given condition is true and aborts if
 *  not.
 *  The check is performed only if ZLXOPT_ENABLE_ASSERT is non-zero.
 *  Unless specified explicitly, this option is defined to 1 if NDEBUG is
 *  defined and 0 otherwise.
 *
 *  @hideinitializer
 */
#if ZLXOPT_ASSERT
# define _ZLX_ASSERT_MSG(cond) (__FILE__ ":" ZLX_LINE_STR ": *** ASSERTION FAILED: " #cond)
# define ZLX_ASSERT(cond) \
    ((cond) ? (void) 0 : zlx_assert_failed(_ZLX_ASSERT_MSG(cond)))
#else
# define ZLX_ASSERT(cond) ((void) 0)
#endif

/* zlx_assert_set_writer ****************************************************/
/**
 *  Sets the writer function and context to invoke when displaying the
 *  assertion failed message right before abort.
 *  The library starts initialized with a nop writer that silently discards
 *  the message.
 */
ZLX_API void ZLX_CALL zlx_assert_set_writer
(
    zlx_writer_func_t func,
    void * context
);

/* zlx_assert_set_abort *****************************************************/
/**
 *  Sets the abort function to be called when an assertion fails
 *  The default function is a NULL pointer which usually generates a dump of
 *  the hosting process.
 */
ZLX_API void ZLX_CALL zlx_assert_set_abort
(
    void (* abort_func) (void)
);

/* zlx_assert_failed ********************************************************/
/**
 *  Function called when an assertion fails.
 *  This function calls the writer and the abort configured through calls
 *  to zlx_assert_set_writer() and zlx_assert_set_abort().
 *  This function is used from #ZLX_ASSERT.
 */
ZLX_API void ZLX_CALL zlx_assert_failed
(
    char const * msg
);

ZLX_C_DECL_END

/** @} */

#endif /* _ZLX_ASSERT_H */
