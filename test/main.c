#include <string.h>
#include <stdio.h>
#include "../include/zlx/int/fmt.h"
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

int tlsf_endurance_test (uint64_t ops);

/* main *********************************************************************/
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

    if (argc >= 2)
    {
        if (!strcmp(argv[1], "std") || !strcmp(argv[1], "standard")) {}
        else if (!strcmp(argv[1], "tlsf-endurance"))
        {
            uint64_t n;
            size_t l;

            if (argc < 3) n = 0x100000;
            else if (zlx_u64_from_str((uint8_t const *) argv[2],
                                      strlen(argv[2]), 0, &n, &l))
            {
                fprintf(stderr, "bad iteration count: %s\n", argv[2]);
                return 127;
            }
            return tlsf_endurance_test(n);
        }
        else
        {
            fprintf(stderr, "unknown command '%s'\n", argv[1]);
            return 127;
        }
    }

    ZLX_DMSG("launching standard tests...");
#define T(t) trc = t(); if (printf("  %s: %s (ret code %u)\n", #t, trc ? "FAILED" : "passed", trc) >= 0) ; else { fprintf(stderr, "output error\n"); return 64 | rc; } rc |= !!trc;
#include "test_list.inc"
#undef T

    return rc;
}

