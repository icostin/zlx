#include <string.h>
#include "../include/zlx/log.h"
#include "../include/zlx/obstream/buffer.h"

int log_test (void)
{
    zlx_buffer_obstream_t bos;
    uint8_t buf[10];
    zlx_log_t log;

    buf[8] = 'x';

    if (&bos.as_obstream != zlx_buffer_obstream_init(&bos, buf, 8))
        return 1;

    zlx_log_init(&log, ZLX_LL_WARNING, &bos.as_obstream);

    if (zlx_log_data(&log, ZLX_LL_FATAL, (uint8_t *) "aaa", 3)) return 1;
    if (zlx_log_data(&log, ZLX_LL_DEBUG, (uint8_t *) "bbb", 3)) return 1;
    if (zlx_log_data(&log, ZLX_LL_ERROR, (uint8_t *) "ccc", 3)) return 1;
    if (zlx_log_data(&log, ZLX_LL_INFO, (uint8_t *) "ddd", 3)) return 1;
    if (!zlx_log_data(&log, ZLX_LL_WARNING, (uint8_t *) "eee", 3)) return 1;

    return memcmp(buf, "aaaccceex", 9);
}


