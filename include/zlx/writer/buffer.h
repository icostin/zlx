#ifndef _ZLX_WRITER_BUFFER_H
#define _ZLX_WRITER_BUFFER_H

#include "../language.h"
#include "interface.h"

/** @addtogroup writer
 *  @{ */

ZLX_C_DECL_BEGIN

/** Buffer writer instance.
 *  This function should be initialized by zlx_wbuf_init() and used as context
 *  to zlx_wbuf_writer(). */
typedef struct zlx_wbuf zlx_wbuf_t;
struct zlx_wbuf
{
    uint8_t * data;
    size_t size;
    size_t offset;
};

/** Inits a write-buffer instance.
 *  @param wbuf [out]
 *      instance to init
 *  @param data [in]
 *      data buffer to be populated by zlx_wbuf_writer()
 *  @param size [in]
 *      size of the buffer
 */
ZLX_API void ZLX_CALL zlx_wbuf_init
(
    zlx_wbuf_t * ZLX_RESTRICT wbuf,
    uint8_t * data,
    size_t size
);

/** Writer function for a write-buffer that counts how much data was sent but
 *  fills only the amount available in the write-buffer.
 *  @param context [in]
 *      pointer to a initialized #zlx_wbuf_t instance
 *  @param data [in]
 *      data to write
 *  @param size [in]
 *      size of data
 *  @returns always @a size
 */
ZLX_API size_t ZLX_CALL zlx_wbuf_writer
(
    void * context,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
);

/* zlx_wbuf_limit_writer ****************************************************/
/**
 *  Writer function for a write-buffer that succeeds as long as the data fits
 *  in the buffer.
 *  @returns how much was able to fit in the write-buffer.
 */
ZLX_API size_t ZLX_CALL zlx_wbuf_limit_writer
(
    void * context,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
);

ZLX_C_DECL_END

#endif

