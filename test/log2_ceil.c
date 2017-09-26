#include <zlx/int.h>
#include "test.h"

int log2_ceil_test (void)
{
    unsigned int i;
    for (i = 0; i < 8; ++i)
    {
        unsigned int n, l;

        n = (1 << i) - 1; l = zlxni_u8_log2_ceil(n);
        TE(l == i || (l == 0 && i == 1), "n=%u log2ceil(n)=%u", n, l);

        n = (1 << i); l = zlxni_u8_log2_ceil(n);
        TE(l == i, "n=%u log2ceil(n)=%u", n, l);

        n = (1 << i) + 1; l = zlxni_u8_log2_ceil(n);
        TE(l == i + 1, "n=%u log2ceil(n)=%u", n, l);
    }
    T(zlxni_u8_log2_ceil(0xFF) == 8);
    return 0;
}

