#define ZLXOPT_ENABLE_ASSERT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "../include/zlx/assert.h"
#include "soft_abort.h"

int x = 40;
int y = 2;

int assert_pass_test (void)
{
    ZLX_ASSERT(x + y == 42);
    return 0;
}

int fail_assert (void * ctx)
{
    (void) ctx;
    ZLX_ASSERT(x + y != 42);

    return 0;
}

int assert_fail_test (void)
{
    int r, fa;
    unsigned int ac = soft_abort_count;

    fa = run_catching_aborts(fail_assert, NULL, 1);

    r = fa && soft_abort_count == ac + 1 && !strcmp(assert_msg, "test/assert.c:24: *** ASSERTION FAILED: x + y != 42");
    if (!r)
    {
        printf("assert_fail_test: fa=%d sac-ac=%d assert_msg=\"%s\"\n", 
               fa, soft_abort_count - ac, assert_msg);
    }

    return !r;
}

