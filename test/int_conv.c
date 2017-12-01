#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include "../include/zlx/int/ops.h"
#include "test.h"
#include "soft_abort.h"

ZLX_LOCAL uint8_t u16_to_u8 (uint16_t v16);
ZLX_LOCAL uint8_t u8_from_u16 (uint16_t v16);
ZLX_LOCAL uint8_t cast_u16_to_u8 (uint16_t v16);
ZLX_LOCAL uint8_t cast_u8_from_u16 (uint16_t v16);

ZLX_LOCAL uint8_t u32_to_u8 (uint32_t v32);
ZLX_LOCAL uint8_t u8_from_u32 (uint32_t v32);
ZLX_LOCAL uint8_t cast_u32_to_u8 (uint32_t v32);
ZLX_LOCAL uint8_t cast_u8_from_u32 (uint32_t v32);

ZLX_LOCAL int8_t u8_to_s8 (uint8_t v8);
ZLX_LOCAL int8_t s8_from_u8 (uint8_t v8);
ZLX_LOCAL int8_t cast_u8_to_s8 (uint8_t v8);
ZLX_LOCAL int8_t cast_s8_from_u8 (uint8_t v8);

ZLX_LOCAL int8_t u16_to_s8 (uint16_t v16);
ZLX_LOCAL int8_t s8_from_u16 (uint16_t v16);
ZLX_LOCAL int8_t cast_u16_to_s8 (uint16_t v16);
ZLX_LOCAL int8_t cast_s8_from_u16 (uint16_t v16);

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

