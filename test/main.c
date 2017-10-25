#include <stdio.h>
#include <zlx.h>
#include "soft_abort.h"

#define T(fn) int fn (void);
#include "test_list.inc"
#undef T

int main (int argc, char const * const * argv)
{
    int rc = 0, trc;

    (void) argc;
    (void) argv;
    printf("* using zlx lib: %s\n", zlx_lib_id);

    init_soft_abort();
#define T(t) trc = t(); if (printf("  %s: %s (ret code %u)\n", #t, trc ? "FAILED" : "passed", trc) >= 0) ; else { fprintf(stderr, "output error\n"); return 64 | rc; } rc |= !!trc;
#include "test_list.inc"
#undef T

    return rc;
}

