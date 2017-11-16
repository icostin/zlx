#include <string.h>
#include "../include/zlx/fmt.h"
#include "../include/zlx/writer/buffer.h"
#include "test.h"

int ZLX_CALL test_width_measure
(
    uint8_t const * ZLX_RESTRICT data,
    size_t size,
    size_t * ZLX_RESTRICT width,
    size_t * ZLX_RESTRICT parsed_size,
    void * ctx
)
{
    (void) ctx;
    if (memchr(data, '9', size)) return ZLX_UTF_ERR_NON_PRINTABLE;
    *width = *parsed_size = size;
    return 0;
}


int fmt_test (void)
{
#define N 100
    uint8_t buf[N + 1];
    zlx_wbuf_t wb;
    unsigned int fs;
    ptrdiff_t d;

    zlx_wbuf_init(&wb, buf, N); buf[N] = 0;
    fs = zlx_fmt(zlx_wbuf_writer, &wb, "hello $i", 2);
    TE(fs == 0, "fmt status: %u", fs);
    T(!strcmp((char *) buf, "hello 2"));

    memset(buf, 0, sizeof(buf));
    zlx_wbuf_init(&wb, buf, N);
    fs = zlx_fmt(zlx_wbuf_writer, &wb, "str: $s", "coo coo");
    TE(fs == 0, "fmt status: %u", fs);
    TE(!strcmp((char *) buf, "str: coo coo"), "got: '%s'", (char *) buf);

    d = zlx_sfmt(buf, 9, "test $i", 12345);
    TE(d == 10, "d=%d", (int) d);
    TE(!strcmp((char *) buf, "test 123"), "s: '%s'", (char *) buf);

    memset(buf, '-', N);
    zlx_wbuf_init(&wb, buf, 4); buf[N] = 0;
    fs = zlx_fmt(zlx_wbuf_limit_writer, &wb, "hello $i", 2);
    TE(fs == ZLX_FMT_WRITE_ERROR, "fmt status: %u", fs);
    T(!memcmp(buf, "hell", 4));

    T(zlx_sfmt(buf, 0, "") == -ZLX_FMT_WRITE_ERROR);

    d = zlx_sfmt(buf, 99, "test $08i", 12345);
    TE(!strcmp((char *) buf, "test 00012345"), "s: '%s'", (char *) buf);

    d = zlx_sfmt(buf, 99, "test $c", 0xD800);
    TE(d == -ZLX_FMT_MALFORMED, "d=%ld", (long) d);
    TE(!strcmp((char *) buf, "test "), "s: '%s'", (char *) buf);

    d = zlx_sfmt(buf, 99, "test $c", 0xFFFF);
    TE(d == -ZLX_FMT_WIDTH_ERROR, "d=%ld", (long) d);
    TE(!strcmp((char *) buf, "test "), "s: '%s'", (char *) buf);

    d = zlx_sfmt(buf, 99, "test $c", 0x100);
    TE(d == 7, "d=%ld", (long) d);
    TE(!strcmp((char *) buf, "test \xC4\x80"), "s: '%s'", (char *) buf);

    d = zlx_sfmt(buf, 99, "test $xb", 0xFE);
    TE(!strcmp((char *) buf, "test 0xFE"), "s: '%s'", (char *) buf);

    zlx_wbuf_init(&wb, buf, N); buf[N] = 0;
    d = zlx_wfmt(zlx_wbuf_writer, &wb, test_width_measure, NULL, "zzz $i", 9);
    buf[wb.offset] = 0;
    TE(!strcmp((char *) buf, "zzz "), "s: '%s'", (char *) buf);
    return 0;
}

