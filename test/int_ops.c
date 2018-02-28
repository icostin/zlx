#include "../include/zlx/int/ops.h"
#include "test.h"
#include "soft_abort.h"

ZLX_LOCAL unsigned int u32_ctz (uint32_t n);

int u32_ctz_v (void * context)
{
    uint32_t * p = context;
    return (int) u32_ctz(*p);
}

int int_ops_test (void)
{
    int rc;
    int v = 0;

    rc = run_catching_aborts(u32_ctz_v, &v, -1234);
    T(rc == -1234);
    T(u32_ctz(1) == 0);
    T(u32_ctz(2) == 1);
    T(u32_ctz(3) == 0);
    T(u32_ctz(4) == 2);
    T(u32_ctz(0x80000000) == 31);
    T(u32_ctz(0x80000100) == 8);
    T(u32_ctz(0x80010000) == 16);
    T(u32_ctz(0x81000000) == 24);
    return 0;
}

