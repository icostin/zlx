#ifndef _ZLX_WRITER_NOP_H
#define _ZLX_WRITER_NOP_H

#include "../lib.h"
#include "../int.h"

/** @addtogroup writer
 *  @{ */

/** Writer that always succeeds.
 *  @param context [in/out]
 *      ignored context
 *  @param data [in]
 *      ignored data to size
 *  @param size [in]
 *      size of data
 *  @returns @a size
 */
ZLX_API size_t ZLX_CALL zlx_nop_writer
(
    void * context,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
);

/** @} */

#endif
