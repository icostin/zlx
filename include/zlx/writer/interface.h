#ifndef _ZLX_WRITER_H
#define _ZLX_WRITER_H

/** @defgroup writer Writer
 *  @{ */

#include "../lib.h"
#include "../int.h"

/** Function for writing data.
 *  @param context [in/out]
 *      writer context
 *  @param data [in]
 *      data buffer
 *  @param size [in]
 *      size of buffer
 *  @returns
 *      number of bytes written; if the value is different compared to @a size
 *      then there was an error writing the data and the error should be 
 *      retrieved from the context
 */
typedef size_t (ZLX_CALL * zlx_writer_func_t)
    (
        void * context,
        uint8_t const * ZLX_RESTRICT data,
        size_t size
    );

/** @} */

#endif /* _ZLX_WRITER_H */


