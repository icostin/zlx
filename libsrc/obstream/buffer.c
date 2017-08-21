#include "../public/zlx/obstream/buffer.h"

static size_t ZLX_CALL buffer_obstream_write
(
    zlx_obstream_t * ZLX_RESTRICT stream,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
);

static zlx_obstream_type_t const buffer_obstream_type =
{
    buffer_obstream_write
};

/* buffer_obstream_write ****************************************************/
static size_t ZLX_CALL buffer_obstream_write
(
    zlx_obstream_t * ZLX_RESTRICT stream,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
)
{
    zlx_buffer_obstream_t * ZLX_RESTRICT bs = (zlx_buffer_obstream_t *) stream;
    uint8_t * ZLX_RESTRICT o;
    uint8_t const * data_end;
    size_t space_left = bs->size - bs->offset;

    if (size > space_left) size = space_left;

    data_end = data + size;
    o = bs->data + bs->offset;
    while (data < data_end) *o++ = *data++;

    bs->offset += size;
    return size;
}

/* zlx_buffer_obstream_init *************************************************/
ZLX_API zlx_obstream_t * ZLX_CALL zlx_buffer_obstream_init
(
    zlx_buffer_obstream_t * ZLX_RESTRICT stream,
    uint8_t * data,
    size_t size
)
{
    stream->as_obstream.type = (zlx_obstream_type_t *) &buffer_obstream_type;
    stream->data = data;
    stream->size = size;
    stream->offset = 0;
    return &stream->as_obstream;
}

