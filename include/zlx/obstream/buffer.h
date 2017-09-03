#ifndef _ZLX_BUFFER_OBSTREAM_H
#define _ZLX_BUFFER_OBSTREAM_H

#include "base.h"

typedef struct zlx_buffer_obstream zlx_buffer_obstream_t;
struct zlx_buffer_obstream
{
    zlx_obstream_t as_obstream;
    uint8_t * data;
    size_t size;
    size_t offset;
};

ZLX_API zlx_obstream_t * ZLX_CALL zlx_buffer_obstream_init
(
    zlx_buffer_obstream_t * ZLX_RESTRICT stream,
    uint8_t * data,
    size_t size
);


#endif
