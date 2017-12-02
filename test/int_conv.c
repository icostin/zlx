#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include "../include/zlx/int/ops.h"
#include "test.h"
#include "soft_abort.h"

#define C(s, d) \
    zlx_##d##_t s##_to_##d (zlx_##s##_t v); \
    zlx_##d##_t d##_from_##s (zlx_##s##_t v); \
    zlx_##d##_t cast_##s##_to_##d (zlx_##s##_t v); \
    zlx_##d##_t cast_##d##_from_##s (zlx_##s##_t v)

#define AC(d) \
    C(u8, d); C(u16, d); C(u32, d); C(u64, d); C(uptr, d); C(usize, d); C(size, d);  \
    C(s8, d); C(s16, d); C(s32, d); C(s64, d); C(sptr, d); C(ssize, d); C(ptrdiff, d)

AC(u8); AC(u16); AC(u32); AC(u64); AC(uptr); AC(usize); AC(size);
AC(s8); AC(s16); AC(s32); AC(s64); AC(sptr); AC(ssize); AC(ptrdiff);

#undef AC
#undef C

#define ICT(func, expr, exp_rc, exp_val) \
    static int func (void * o) { \
        int64_t * p = (int64_t *) o; \
        *p = (int64_t) expr; \
        return 0; \
    } \
    typedef int func##_tag
#include "int_conv.inc"
#undef ICT

int int_conv_test (void)
{
    int rc;
    int64_t v;

#define ICT(func, expr, exp_rc, exp_val) \
    v = 0; \
    rc = run_catching_aborts(func, &v, 1); \
    TE(rc == exp_rc, "%s: ret %d", #func, rc); \
    TE(v == exp_val, "%s: val64 %"PRIu64" %"PRId64" %"PRIX64, #func, v, v, v)
#include "int_conv.inc"
#undef ICT

    return 0;
}

