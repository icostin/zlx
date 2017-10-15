#include "../include/zlx/assert.h"
#include "../include/zlx/writer/nop.h"

static void (* abort_func) (void) = NULL;
static void * writer_context = NULL;
static zlx_writer_func_t writer_func = zlx_nop_writer;

/* zlx_assert_set_writer ****************************************************/
ZLX_API void ZLX_CALL zlx_assert_set_writer
(
    zlx_writer_func_t func,
    void * context
)
{
    writer_func = func;
    writer_context = context;
}

/* zlx_assert_set_abort *****************************************************/
ZLX_API void ZLX_CALL zlx_assert_set_abort
(
    void (* func) (void)
)
{
    abort_func = func;
}

/* zlx_assert_failed ********************************************************/
ZLX_API void ZLX_CALL zlx_assert_failed
(
    char const * msg
)
{
    size_t n;
    for (n = 0; msg[n]; ++n);
    writer_func(writer_context, (uint8_t const *) msg, n);
    abort_func();
}

