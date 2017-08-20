#ifndef _ZLX_OBSTREAM_BASE_H
#define _ZLX_OBSTREAM_BASE_H

#include "../base.h"
#include "../int.h"

typedef struct zlx_obstream zlx_obstream_t;
typedef struct zlx_obstream_type zlx_obstream_type_t;
typedef size_t (ZLX_CALL * zlx_obstream_write_func_t) 
    (
        zlx_obstream_t * ZLX_RESTRICT stream,
        uint8_t const * ZLX_RESTRICT data,
        size_t size
    );

struct zlx_obstream_type
{
    zlx_obstream_write_func_t write;
};

struct zlx_obstream
{
    zlx_obstream_type_t * type;
};

/* zlx_obstream_write *******************************************************/
ZLX_INLINE size_t zlx_obstream_write
(
    zlx_obstream_t * ZLX_RESTRICT obstream,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
)
{
    return obstream->type->write(obstream, data, size);
}

#endif /* _ZLX_OBSTREAM_BASE_H */
