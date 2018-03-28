#ifndef _ZLX_LOG_H
#define _ZLX_LOG_H

#include <stdarg.h>
#include "language.h"
#include "writer/interface.h"
#include "fmt.h"
#include "assert.h"

ZLX_C_DECL_BEGIN

enum zlx_log_level
{
    ZLX_LL_NONE = 0,
    ZLX_LL_FATAL,
    ZLX_LL_ERROR,
    ZLX_LL_WARNING,
    ZLX_LL_INFO,
    ZLX_LL_DEBUG,
    ZLX_LL_DEBUG2,
};
typedef enum zlx_log_level zlx_log_level_t;

typedef struct zlx_log zlx_log_t;
struct zlx_log
{
    zlx_writer_func_t writer_function;
    void * writer_context;
    zlx_log_level_t level;
    zlx_fmt_status_t fmt_status;
};

ZLX_INLINE void zlx_log_init
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_writer_func_t func,
    void * context,
    zlx_log_level_t level
)
{
    log->writer_function = func;
    log->writer_context = context;
    log->level = level;
    log->fmt_status = ZLX_FMT_OK;
}

ZLX_INLINE zlx_log_level_t zlx_log_set_level
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level
)
{
    zlx_log_level_t prev_level = log->level;
    log->level = level;
    return prev_level;
}


ZLX_API void ZLX_CALL zlx_log_vfmt
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level,
    char const * fmt,
    va_list va
);

ZLX_API void ZLX_CALL zlx_log_fmt
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level,
    char const * fmt,
    ...
);

ZLX_API extern zlx_log_t zlx_nop_log;
ZLX_API extern zlx_log_t * zlx_default_log;



ZLX_C_DECL_END

#endif
