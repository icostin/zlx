#define ZLXOPT_ENABLE_ASSERT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "../include/zlx/assert.h"
#include "soft_abort.h"
#include "test.h"

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
    unsigned int ac = soft_abort_count;

    T(run_catching_aborts(fail_assert, NULL, 1) == 1);
    T(soft_abort_count == ac + 1);
    TE(!strcmp(assert_msg, 
               "test/assert.c:22: *** ASSERTION FAILED: x + y != 42"),
       "assert_msg: %s", assert_msg);

    return 0;
}

