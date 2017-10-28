#define _POSIX_C_SOURCE 200001
#define _BSD_SOURCE 1
#define _GNU_SOURCE 1
#include <stdint.h>
#include <inttypes.h>
#include "../include/zlx/int/ops.h"
#include "test.h"
#include "soft_abort.h"
#include <signal.h>
#include <setjmp.h>
#include "../include/zlx/assert.h"

int div_by_ptr (void * ctx)
{
    uint64_t r;
    zlx_u64_div_mod(1, (uint64_t) (uintptr_t) ctx, &r);
    return 0;
}

void * g = NULL;
sigjmp_buf jb;
sighandler_t div_sh;

void trap_div_error (int signum)
{
    siglongjmp(jb, signum);
}

int div_mod_test (void)
{
    uint64_t q, r;

#define DMT(a, b, qe, re) \
    q = zlx_u64_div_mod(UINT64_C(a), UINT64_C(b), &r); \
    TE(q == qe, "q=%"PRIu64, q); TE(r == re, "r=%"PRIu64, r);

    DMT(1, 1, 1, 0);
    DMT(1, 2, 0, 1);
    DMT(3, 2, 1, 1);
    DMT(0x100000000, 2, 0x80000000, 0);
    DMT(0xFFFFFFFFFFFFFFFF, 0x123456789A, 235929600, 174063615);
    if (!sigsetjmp(jb, SIGFPE))
    {
        div_sh = signal(SIGFPE, trap_div_error);
        div_by_ptr(g);
        return 1;
    }
    else
    {
        signal(SIGFPE, div_sh);
    }

    return 0;
}

