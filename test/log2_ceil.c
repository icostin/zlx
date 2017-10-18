#include "../include/zlx/lib.h"
#include "../include/zlx/int/ops.h"
#include "test.h"

ZLX_LOCAL uint8_t u8_log2_ceil (uint8_t x);
ZLX_LOCAL uint16_t u16_log2_ceil (uint16_t x);
ZLX_LOCAL uint32_t u32_log2_ceil (uint32_t x);
ZLX_LOCAL uint64_t u64_log2_ceil (uint64_t x);
ZLX_LOCAL uint8_t size_log2_ceil (size_t x);
ZLX_LOCAL uint8_t uptr_log2_ceil (uintptr_t x);

int log2_ceil_test (void)
{
    unsigned int i;
    for (i = 0; i < 8; ++i)
    {
        unsigned int n, l;

        n = (1U << i) - 1; l = u8_log2_ceil((uint8_t) n);
        TE(l == i || (l == 0 && i == 1), "n=%u log2ceil(n)=%u", n, l);

        n = (1U << i); l = u8_log2_ceil((uint8_t) n);
        TE(l == i, "n=%u log2ceil(n)=%u", n, l);

        n = (1U << i) + 1; l = u8_log2_ceil((uint8_t) n);
        TE(l == i + 1, "n=%u log2ceil(n)=%u", n, l);
    }
    T(u8_log2_ceil(0xFF) == 8);
    T(u16_log2_ceil(0x80) == 7);
    T(u16_log2_ceil(0x8000) == 15);
    TE(u16_log2_ceil(0x8001) == 16, 
       "log2ceil(%u)=%u", 0x8001, u32_log2_ceil(0x8001));
    T(u16_log2_ceil(0xFFFF) == 16);
    T(u32_log2_ceil(0x8000) == 15);
    T(u32_log2_ceil(0x80000000) == 31);
    TE(u32_log2_ceil(0x80000001) == 32, 
       "log2ceil(%u)=%u", 0x80000001, u32_log2_ceil(0x80000001));
    T(u32_log2_ceil(0xFFFFFFFF) == 32);
    T(u64_log2_ceil(0x80000000) == 31);
    T(u64_log2_ceil((UINT64_C(1) << 63) - 1) == 63);
    T(u64_log2_ceil(UINT64_C(1) << 63) == 63);
    T(u64_log2_ceil((UINT64_C(1) << 63) + 1) == 64);
    T(size_log2_ceil(0xFF00) == 16);
    T(uptr_log2_ceil(0xFF01) == 16);
    return 0;
}

