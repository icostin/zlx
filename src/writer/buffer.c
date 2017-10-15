#include "../../include/zlx/writer/buffer.h"

/* zlx_wbuf_init ************************************************************/
ZLX_API void ZLX_CALL zlx_wbuf_init
(
    zlx_wbuf_t * ZLX_RESTRICT wbuf,
    uint8_t * data,
    size_t size
)
{
    wbuf->data = data;
    wbuf->size = size;
    wbuf->offset = 0;
}

/* zlx_wbuf_writer **********************************************************/
ZLX_API size_t ZLX_CALL zlx_wbuf_writer
(
    void * context,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
)
{
    zlx_wbuf_t * ZLX_RESTRICT wbuf = context;
    
    if (wbuf->offset < wbuf->size)
    {
        size_t i, copy_size;

        copy_size = wbuf->size - wbuf->offset;
        if (copy_size > size) copy_size = size;

        for (i = 0; i < copy_size; ++i)
            wbuf->data[wbuf->offset + i] = data[i];
    }

    wbuf->offset += size;
    return size;
}


