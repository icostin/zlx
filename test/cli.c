#include <stdio.h>
#include <zlx.h>

#define T(fn) int fn (void);
#include "test_list.inc"
#undef T

int main (int argc, char const * const * argv)
{
    int rc = 0, trc;

    (void) argc; (void) argv;

    puts("zlx_test");
    printf("  zlx_lib: %s\n", zlx_lib_name);

#define T(t) rc |= trc = t(); if (printf("  %s: %s\n", #t, trc ? "FAILED" : "passed") >= 0) ; else { fprintf(stderr, "output error\n"); return 64 | rc; }
#include "test_list.inc"
#undef T

    return rc;
}

