#define ZLXOPT_ENABLE_ASSERT
#include <stdio.h>
#include <string.h>
#include "../include/zlx/assert.h"
#include "../include/zlx/obstream/buffer.h"
#include "../include/zlx/obstream/nop.h"

int x = 40;
int y = 2;
static int assert_count = 0;

void assert_test_abort (void)
{
    ++assert_count;
}

int assert_pass_test (void)
{
    ZLX_ASSERT(x + y == 42);
    return 0;
}

int assert_fail_test (void)
{
    int r;
    int ac = assert_count;
    uint8_t buf[0x80];
    zlx_buffer_obstream_t bobs;
    void (* ab) (void);

    ab = zlx_abort;

    zlx_assert_log = zlx_buffer_obstream_init(&bobs, buf, sizeof(buf) - 1);
    zlx_abort = assert_test_abort;

    ZLX_ASSERT(x + y != 42);

    buf[bobs.offset] = 0;
    r = assert_count == ac + 1 && !strcmp((char *) buf, "test/assert.c:36: *** ASSERTION FAILED: x + y != 42");
    //puts((char *) buf);

    zlx_assert_log = &zlx_nop_obstream;
    zlx_abort = ab;
    return !r;
}

