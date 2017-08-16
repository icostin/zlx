#include <zlx/base.h>
/* this tests that we can compile declarations using ZLX_RESTRICT */

void int_copy (int * ZLX_RESTRICT dest, int const * ZLX_RESTRICT src)
{
    *dest = *src;
}

int restrict_test (void)
{
    int a = 5, b = 2;
    int_copy(&b, &a);
    return a != b;
}

