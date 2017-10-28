#include "../include/zlx/int/ops.h"
#include "test.h"

ZLX_LOCAL uint8_t u8_log2_floor (uint8_t x);
ZLX_LOCAL uint8_t u16_log2_floor (uint16_t x);
ZLX_LOCAL uint8_t u32_log2_floor (uint32_t x);
ZLX_LOCAL uint8_t u64_log2_floor (uint64_t x);

int log2_floor_test (void)
{
    unsigned int i;

    T(u8_log2_floor(0) == 0);
    for (i = 1; i < 0x100; ++i)
    {
        uint8_t b = u8_log2_floor((uint8_t) i);
        T((i >> b) == 1);
    }

    for (i = 8; i < 16; ++i)
    {
        T(u16_log2_floor((uint16_t) ((1 << i) - 1)) == i - 1);
        T(u16_log2_floor((uint16_t) (1 << i)) == i);
        T(u16_log2_floor((uint16_t) ((1 << i) + 1)) == i);
    }

    for (i = 8; i < 32; ++i)
    {
        T(u32_log2_floor((UINT32_C(1) << i) - 1) == i - 1);
        T(u32_log2_floor((UINT32_C(1) << i)) == i);
        T(u32_log2_floor((UINT32_C(1) << i) + 1) == i);
    }

    for (i = 8; i < 64; ++i)
    {
        T(u64_log2_floor((UINT64_C(1) << i) - 1) == i - 1);
        T(u64_log2_floor((UINT64_C(1) << i)) == i);
        T(u64_log2_floor((UINT64_C(1) << i) + 1) == i);
    }

    return 0;
}

