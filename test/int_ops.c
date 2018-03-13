#include "../include/zlx/int/ops.h"
#include "test.h"
#include "soft_abort.h"

#define SSB(n, t) \
    ZLX_LOCAL unsigned int n##_lssb (t); \
    ZLX_LOCAL unsigned int n##_mssb (t); \
    int n##_lssb_v (void * context) { \
        t * p = context; \
        return (int) n##_lssb(*p); \
    } \
    int n##_mssb_v (void * context) { \
        t * p = context; \
        return (int) n##_mssb(*p); \
    }

SSB(u8, uint8_t)
SSB(u16, uint16_t)
SSB(u32, uint32_t)
SSB(u64, uint64_t)
SSB(uptr, uintptr_t)
SSB(size, size_t)

#define X(n, t) \
    t n##_zero = 0; \
    rc = run_catching_aborts(n##_lssb_v, &n##_zero, -1234); T(rc == -1234); \
    rc = run_catching_aborts(n##_mssb_v, &n##_zero, -1234); T(rc == -1234); \
    for (i = 0; i < sizeof(t) * 8; ++i) { \
        unsigned int j = i + 2; \
        if (j >= sizeof(t) * 8) j = i; \
        T(n##_lssb((t) ((t) 1 << i)) == i); \
        T(n##_mssb((t) ((t) 1 << i)) == i); \
        T(n##_lssb((t) ((t) 5 << i)) == i); \
        T(n##_mssb((t) ((t) 5 << i)) == j); \
    }

int int_ops_test (void)
{
    int rc;
    unsigned int i;

    X(u8, uint8_t);
    X(u16, uint16_t);
    X(u32, uint32_t);
    X(u64, uint64_t);
    X(uptr, uintptr_t);
    X(size, size_t);

    return 0;
}

