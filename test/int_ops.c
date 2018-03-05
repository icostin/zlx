#include "../include/zlx/int/ops.h"
#include "test.h"
#include "soft_abort.h"

ZLX_LOCAL unsigned int u32_lssb (uint32_t n);
ZLX_LOCAL unsigned int u32_mssb (uint32_t n);

int u32_lssb_v (void * context)
{
    uint32_t * p = context;
    return (int) u32_lssb(*p);
}

int u32_mssb_v (void * context)
{
    uint32_t * p = context;
    return (int) u32_mssb(*p);
}

int int_ops_test (void)
{
    int rc;
    int v = 0;

    rc = run_catching_aborts(u32_lssb_v, &v, -1234);
    T(rc == -1234);
    T(u32_lssb(1) == 0);
    T(u32_lssb(2) == 1);
    T(u32_lssb(3) == 0);
    T(u32_lssb(4) == 2);
    T(u32_lssb(0x80000000) == 31);
    T(u32_lssb(0x80000100) == 8);
    T(u32_lssb(0x80010000) == 16);
    T(u32_lssb(0x81000000) == 24);

    rc = run_catching_aborts(u32_mssb_v, &v, -1234);
    T(rc == -1234);
    T(u32_mssb(1) == 0);
    T(u32_mssb(2) == 1);
    T(u32_mssb(3) == 1);
    T(u32_mssb(4) == 2);
    T(u32_mssb(5) == 2);
    T(u32_mssb(0x80000000) == 31);
    T(u32_mssb(0xC0000000) == 31);
    T(u32_mssb(0x60000001) == 30);
    T(u32_mssb(0x80000100) == 31);
    T(u32_mssb(0x80010000) == 31);
    T(u32_mssb(0x81000000) == 31);
    T(u32_mssb(0x00400000) == 22);
    T(u32_mssb(0x00002000) == 13);
    T(u32_mssb(0x00000010) == 4);

    return 0;
}

