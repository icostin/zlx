#include "../include/zlx/int/ops.h"
#include "test.h"
#include "soft_abort.h"

ZLX_LOCAL unsigned int u32_ffs (uint32_t n);

int u32_ffs_v (void * context)
{
    uint32_t * p = context;
    return (int) u32_ffs(*p);
}

int int_ops_test (void)
{
    int rc;
    int v = 0;

    rc = run_catching_aborts(u32_ffs_v, &v, -1234);
    T(rc == -1234);
    T(u32_ffs(1) == 0);
    T(u32_ffs(2) == 1);
    T(u32_ffs(3) == 0);
    T(u32_ffs(4) == 2);
    T(u32_ffs(0x80000000) == 31);
    T(u32_ffs(0x80000100) == 8);
    T(u32_ffs(0x80010000) == 16);
    T(u32_ffs(0x81000000) == 24);
    return 0;
}

