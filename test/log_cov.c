#include "../include/zlx/log.h"

void log_init
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_writer_func_t func,
    void * context,
    zlx_log_level_t level
)
{
    zlx_log_init(log, func, context, level);
}

zlx_log_level_t log_set_level
(
    zlx_log_t * ZLX_RESTRICT log,
    zlx_log_level_t level
)
{
    return zlx_log_set_level(log, level);
}
