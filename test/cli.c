#include <stdio.h>
#include <zlx.h>

int inline_test (void);
int restrict_test (void);
int line_str_test (void);
int nop_obstream_test (void);
int buffer_obstream_test (void);

int main (int argc, char const * const * argv)
{
    int rc = 0, trc;

    puts("zlx_test");
    printf("  zlx_lib: %s\n", zlx_lib_name);

#define T(t) rc |= trc = t(); printf("  %s: %s\n", #t, trc ? "FAILED" : "passed");
    T(inline_test);
    T(restrict_test);
    T(line_str_test);
    T(use_int_test);
    T(nop_obstream_test);
    T(buffer_obstream_test);
    return rc;
}

