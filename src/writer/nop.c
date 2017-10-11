#include "../../include/zlx/writer/nop.h"

ZLX_API size_t ZLX_CALL zlx_nop_writer
(
    void * context,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
)
{
    (void) context;
    (void) data;
    return size;
}

