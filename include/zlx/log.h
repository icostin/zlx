#ifndef _ZLX_OBSTREAM_LOG_H
#define _ZLX_OBSTREAM_LOG_H

/** @defgroup log Logger
 *  Simple logging facility.
 *
 *  @{
 */

#include "base.h"
#include "obstream/base.h"

/* zlx_log_level_t **********************************************************/
/**
 *  Enum for log levels
 */
typedef enum zlx_log_level_enum
{
    ZLX_LL_NONE, /**< suppresses all messages */
    ZLX_LL_FATAL, /**< allows only fatal messages */
    ZLX_LL_ERROR, /**< allows fatal and error messages */
    ZLX_LL_WARNING, /**< allows warning and more severe messages */
    ZLX_LL_INFO, /**< allows info and higher severity messages */
    ZLX_LL_DEBUG, /**< allows debug and higher severity messages */
} zlx_log_level_t;

/* zlx_log_t ****************************************************************/
/**
 *  Logger instance data.
 */
typedef struct zlx_log zlx_log_t;
struct zlx_log
{
    zlx_log_level_t level;
    zlx_obstream_t * stream;
};

/* zlx_log_init *************************************************************/
/**
 *  Inits a logger.
 */
ZLX_INLINE void zlx_log_init
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level,
    zlx_obstream_t * ZLX_RESTRICT stream
)
{
    log->level = level;
    log->stream = stream;
}

/* zlx_log_data *************************************************************/
/**
 *  Logs data.
 *  @returns 0 for success, 1 for a partial write
 */
ZLX_INLINE int zlx_log_data
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
)
{
    if (level > log->level) return 0;
    return zlx_obstream_write(log->stream, data, size) != size;
}

/** @} */

#endif /* _ZLX_OBSTREAM_LOG_H */

