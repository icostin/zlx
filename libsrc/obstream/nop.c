#include "../../include/zlx/obstream/nop.h"

size_t ZLX_CALL nop_obstream_write
(
    zlx_obstream_t * ZLX_RESTRICT stream,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
)
{
    (void) stream;
    (void) data;
    return size;
}

static zlx_obstream_type_t const nop_obstream_type =
{
    nop_obstream_write
};

ZLX_API zlx_obstream_t zlx_nop_obstream =
{
    (zlx_obstream_type_t *) &nop_obstream_type
};

