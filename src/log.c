#include "../include/zlx/log.h"
#include "../include/zlx/writer/nop.h"

ZLX_API zlx_log_t zlx_nop_log =
{
    zlx_nop_writer,
    NULL,
    ZLX_LL_NONE,
    ZLX_FMT_OK,
};

ZLX_API zlx_log_t * zlx_default_log = &zlx_nop_log;

/* zlx_log_vfmt *************************************************************/
ZLX_API void ZLX_CALL zlx_log_vfmt
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level,
    char const * fmt,
    va_list va
)
{
    zlx_fmt_status_t fs;

    ZLX_ASSERT(level > ZLX_LL_NONE);

    if (level > log->level) return;
    fs = zlx_vfmt(log->writer_function, log->writer_context, fmt, va);
    if (fs)
    {
        log->fmt_status = fs;
        log->level = ZLX_LL_NONE;
    }
}

/* zlx_log_fmt **************************************************************/
ZLX_API void ZLX_CALL zlx_log_fmt
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level,
    char const * fmt,
    ...
)
{
    va_list va;

    va_start(va, fmt);
    zlx_log_vfmt(log, level, fmt, va);
    va_end(va);
}

