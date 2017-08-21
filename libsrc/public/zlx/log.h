#ifndef _ZLX_OBSTREAM_LOG_H
#define _ZLX_OBSTREAM_LOG_H

#include "base.h"
#include "obstream/base.h"

typedef enum zlx_log_level_enum
{
    ZLX_LL_NONE,
    ZLX_LL_FATAL,
    ZLX_LL_ERROR,
    ZLX_LL_WARNING,
    ZLX_LL_INFO,
    ZLX_LL_DEBUG
} zlx_log_level_t;

typedef struct zlx_log zlx_log_t;
struct zlx_log
{
    zlx_log_level_t level;
    zlx_obstream_t * stream;
};

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
    return zlx_obstream_write(log->stream, data, size);
}


#endif /* _ZLX_OBSTREAM_LOG_H */

