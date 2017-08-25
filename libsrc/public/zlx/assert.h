#ifndef _ZLX_ASSERT_H
#define _ZLX_ASSERT_H

#include "base.h"
#include "obstream/base.h"

ZLX_CPP_ONLY(extern "C" {)

extern ZLX_API zlx_obstream_t * zlx_assert_log;
extern ZLX_API void (* zlx_abort) (void);

#if NDEBUG && !defined ZLXOPT_ENABLE_ASSERT
# define ZLX_ASSERT(cond) ((void) 0)
#else
# if __STDC_VERSION__ >= 199901L
#  define _ZLX_ASSERT_MSG(cond) __FILE__ ":" ZLX_LINE_STR ": in " __func__ "(): *** ASSERTION FAILED: " #cond
# else
#  define _ZLX_ASSERT_MSG(cond) __FILE__ ":" ZLX_LINE_STR ": *** ASSERTION FAILED: " #cond
# endif
# define ZLX_ASSERT(cond) ((cond) ? (void) 0 : ((void) zlx_obstream_write(zlx_assert_log, _ZLX_ASSERT_MSG(cond), sizeof(_ZLX_ASSERT_MSG(cond)) - 1), zlx_abort()))
#endif

ZLX_CPP_ONLY(})

#endif /* _ZLX_ASSERT_H */
