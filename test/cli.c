#include <stdio.h>
#include "soft_abort.h"
#include "../include/zlx.h"

#define T(fn) int fn (void);
#include "test_list.inc"
#undef T

int main (int argc, char const * const * argv)
{
    int rc = 0, trc;

    (void) argc; (void) argv;

    init_soft_abort();

    puts("zlx_test");
    printf("  zlx_lib: %s\n", zlx_lib_name);

#define T(t) trc = t(); if (printf("  %s: %s (ret code %u)\n", #t, trc ? "FAILED" : "passed", trc) >= 0) ; else { fprintf(stderr, "output error\n"); return 64 | rc; } rc |= !!trc;
#include "test_list.inc"
#undef T

    return rc;
}

