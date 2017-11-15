#include <string.h>
#include "../include/zlx/fmt.h"
#include "../include/zlx/writer/buffer.h"
#include "test.h"

int fmt_test (void)
{
#define N 100
    uint8_t buf[N + 1];
    zlx_wbuf_t wb;
    unsigned int fs;

    zlx_wbuf_init(&wb, buf, N);
    buf[N] = 0;
    
    fs = zlx_fmt(zlx_wbuf_writer, &wb, "hello $i", 2);
    TE(fs == 0, "fmt status: %u", fs);
    T(!strcmp((char *) buf, "hello 2"));

    return 0;
}

