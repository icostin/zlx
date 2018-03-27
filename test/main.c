#include <stdio.h>
#include "../include/zlx.h"
#include "../include/zlx/log.h"
#include "../include/zlx/debug.h"
#include "soft_abort.h"

#define T(fn) int fn (void);
#include "test_list.inc"
#undef T

size_t ZLX_CALL log_writer
(
    void * ctx,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
)
{
    return fwrite(data, 1, size, ctx);
}


int main (int argc, char const * const * argv)
{
    zlx_log_t log;
    int rc = 0, trc;

    (void) argc;
    (void) argv;

    printf("* using zlx lib: %s\n", zlx_lib_id);

    zlx_log_init(&log, log_writer, stderr, ZLX_LL_DEBUG);
    zlx_default_log = &log;

    ZLX_DMSG("initializing soft abort...");
    init_soft_abort();

    ZLX_DMSG("launching tests...");
#define T(t) trc = t(); if (printf("  %s: %s (ret code %u)\n", #t, trc ? "FAILED" : "passed", trc) >= 0) ; else { fprintf(stderr, "output error\n"); return 64 | rc; } rc |= !!trc;
#include "test_list.inc"
#undef T

    return rc;
}

