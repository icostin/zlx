#ifndef _ZLX_WRITER_H
#define _ZLX_WRITER_H

#include "base.h"
#include "int.h"

typedef size_t (ZLX_CALL * zlx_writer_func_t)
    (
        void * writer_ctx,
        uint8_t const * ZLX_RESTRICT data,
        size_t size
    );

#endif /* _ZLX_WRITER_H */

