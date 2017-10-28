#include "../../include/zlx/options.h"
#include "../../include/zlx/int/ops.h"
#include "../../include/zlx/assert.h"

unsigned int zero = 0;

ZLX_API uint64_t ZLX_CALL zlx_u64_div_mod
(
    uint64_t a,
    uint64_t b,
    uint64_t * ZLX_RESTRICT remainder
)
{
    uint8_t ak, bk, c;
    uint64_t quotient;

    if (a <= (b - 1))
    {
        *remainder = a;
        if (b == 0) return ((uint32_t) a / zero);
        return 0;
    }

    ak = zlx_u64_log2_floor(a);
    bk = zlx_u64_log2_floor(b);
    ZLX_ASSERT(ak >= bk);
    c = (uint8_t) (ak - bk);

    quotient = 0;
    do
    {
        uint64_t f;

        f = b << c;
        if (f <= a)
        {
            quotient |= UINT64_C(1) << c;
            a -= f;
        }
    }
    while (c--);
    ZLX_ASSERT(a < b);
    *remainder = a;
    return quotient;
}

#if ZLXOPT_EMIT_BUILTINS
ZLX_API uint64_t __udivdi3 (uint64_t a, uint64_t b)
{
    uint64_t r;
    return zlx_u64_div_mod(a, b, &r);
}

ZLX_API uint64_t __umoddi3 (uint64_t a, uint64_t b)
{
    uint64_t r;
    zlx_u64_div_mod(a, b, &r);
    return r;
}
#endif

