#ifndef _ZLX_OUTPUT_BYTE_STREAM
#define _ZLX_OUTPUT_BYTE_STREAM

#include "base.h"
#include "int.h"

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

#endif

