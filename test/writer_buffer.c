#include <string.h>
#include "../include/zlx/writer/buffer.h"
#include "test.h"

int buffer_writer_test (void)
{
#define N 10
    uint8_t buf[N + 1];
    zlx_wbuf_t wb;

    zlx_wbuf_init(&wb, buf, N);
    buf[N] = 0;
    T(zlx_wbuf_writer(&wb, (uint8_t const *) "hello ", 6) == 6);
    T(zlx_wbuf_writer(&wb, (uint8_t const *) "world!", 6) == 6);
    T(buf[N] == 0);
    T(memcmp(buf, "hello worl", N) == 0);
    
    zlx_wbuf_init(&wb, buf, N);
    buf[N] = 0;
    T(zlx_wbuf_limit_writer(&wb, (uint8_t const *) "hello ", 6) == 6);
    T(zlx_wbuf_limit_writer(&wb, (uint8_t const *) "world!", 6) == 4);
    T(buf[N] == 0);
    T(memcmp(buf, "hello worl", N) == 0);

    return 0;
}

