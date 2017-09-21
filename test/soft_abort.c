#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include "../include/zlx/assert.h"
#include "../include/zlx/obstream/buffer.h"
#include "soft_abort.h"

ZLX_LOCAL unsigned int soft_abort_count;
ZLX_LOCAL char assert_msg[0x100];
static jmp_buf soft_abort_jmp_buf;
static uint8_t got_jmp_buf;
static zlx_buffer_obstream_t assert_bobs;

void soft_abort (void);

/* init_soft_abort **********************************************************/
ZLX_LOCAL void init_soft_abort (void)
{
    soft_abort_count = 0;
    got_jmp_buf = 0;
    assert_msg[0] = 0;
    assert_msg[sizeof(assert_msg) - 1] = 0;

    zlx_assert_log = zlx_buffer_obstream_init(&assert_bobs,
                                              (uint8_t *) assert_msg,
                                              sizeof(assert_msg) - 1);
    zlx_abort = soft_abort;
}

/* soft_abort ***************************************************************/
void soft_abort (void)
{
    ++soft_abort_count;
    assert_msg[assert_bobs.offset] = 0;
    if (got_jmp_buf) longjmp(soft_abort_jmp_buf, 1);
    abort();
}

/* run_catching_aborts ******************************************************/
ZLX_LOCAL int run_catching_aborts 
(
    int (* func) (void *), 
    void * context,
    int volatile ret_on_abort
)
{
    jmp_buf jb;
    uint8_t prev_got_jb;
    int rv;

    prev_got_jb = got_jmp_buf;
    if (got_jmp_buf) memcpy(jb, soft_abort_jmp_buf, sizeof(jb));
    else got_jmp_buf = 1;
    if (!setjmp(soft_abort_jmp_buf)) rv = func(context);
    else rv = ret_on_abort;
    got_jmp_buf = prev_got_jb;
    if (prev_got_jb) memcpy(soft_abort_jmp_buf, jb, sizeof(jb));
    return rv;
}



