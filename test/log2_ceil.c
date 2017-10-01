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
    T(zlxni_u16_log2_ceil(0x80) == 7);
    T(zlxni_u16_log2_ceil(0x8000) == 15);
    TE(zlxni_u16_log2_ceil(0x8001) == 16, 
       "log2ceil(%u)=%u", 0x8001, zlxni_u32_log2_ceil(0x8001));
    T(zlxni_u16_log2_ceil(0xFFFF) == 16);
    T(zlxni_u32_log2_ceil(0x8000) == 15);
    T(zlxni_u32_log2_ceil(0x80000000) == 31);
    TE(zlxni_u32_log2_ceil(0x80000001) == 32, 
       "log2ceil(%u)=%u", 0x80000001, zlxni_u32_log2_ceil(0x80000001));
    T(zlxni_u32_log2_ceil(0xFFFFFFFF) == 32);
    T(zlxni_u64_log2_ceil(0x80000000) == 31);
    T(zlxni_u64_log2_ceil((UINT64_C(1) << 63) - 1) == 63);
    T(zlxni_u64_log2_ceil(UINT64_C(1) << 63) == 63);
    T(zlxni_u64_log2_ceil((UINT64_C(1) << 63) + 1) == 64);
    T(zlxni_size_log2_ceil(0xFF00) == 16);
    T(zlxni_uptr_log2_ceil(0xFF01) == 16);
    return 0;
}

