#include <string.h>
#include "../include/zlx/log.h"
#include "../include/zlx/writer/buffer.h"
#include "test.h"
#include "soft_abort.h"

void log_init
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_writer_func_t func,
    void * context,
    zlx_log_level_t level
);

zlx_log_level_t log_set_level
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level
);

int ablog (void * ctx)
{
    (void) ctx;
    zlx_log_fmt(ctx, ZLX_LL_NONE, "x");
    return 0;
}

int log_test (void)
{
    zlx_wbuf_t wb;
    zlx_log_t zl;
    uint8_t buf[0x10];

    zlx_wbuf_init(&wb, buf, sizeof(buf));
    log_init(&zl, zlx_wbuf_writer, &wb, ZLX_LL_NONE);

    zlx_log_fmt(&zl, ZLX_LL_FATAL, "ding\n");
    T(wb.offset == 0);

    T(log_set_level(&zl, ZLX_LL_INFO) == ZLX_LL_NONE);
    T(log_set_level(&zl, ZLX_LL_INFO) == ZLX_LL_INFO);
    zlx_log_fmt(&zl, ZLX_LL_INFO, "info!\n");
    T(wb.offset == 6); T(!memcmp(buf, "info!\n", 6));

    zlx_log_fmt(&zl, ZLX_LL_ERROR, "$\berror!\n");
    T(zl.fmt_status == ZLX_FMT_MALFORMED);
    T(zl.level == ZLX_LL_NONE);

    T(run_catching_aborts(ablog, &zl, 1));

    return 0;
}

