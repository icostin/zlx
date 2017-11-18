#include "../include/zlx/fmt.h"
#include "../include/zlx/int/fmt.h"
#include "../include/zlx/int/array.h"
#include "../include/zlx/clconv/interface.h"
#include "../include/zlx/clconv/hex.h"
#include "../include/zlx/clconv/c_escape.h"
#include "../include/zlx/unicode.h"
#include "../include/zlx/writer/buffer.h"
#include "../include/zlx/assert.h"

/* zlx_vwfmt ****************************************************************/
#define CMD_NONE 0
#define CMD_BUF 1
#define CMD_STR 2
#define CMD_CONV 3
#define ALIGN_DEFAULT 0
#define ALIGN_LEFT 1
#define ALIGN_RIGHT 2
#define STR_ESC_NONE 0
#define STR_ESC_C 1
#define STR_ESC_HEX 2
#define STR_ESC_CUSTOM 3

ZLX_API unsigned int ZLX_CALL zlx_vwfmt
(
    zlx_writer_func_t writer,
    void * writer_context,
    zlx_unicode_text_width_measure_func_t width_func,
    void * width_ctx,
    char const * ZLX_RESTRICT fmt,
    va_list va
)
{
    //static uint8_t const nul = 0;
    static uint8_t const empty_spaces[] =  // what are we living for?
        "                                                                ";
    uint8_t buffer[0x400];
    uint8_t const * f = (uint8_t const *) fmt;
    uint8_t const * str = NULL;
    uint8_t const * num_pfx;
    zlx_clconv_func_t conv = NULL;
    void * conv_ctx = NULL;
    uint_fast8_t sep;
    uint_fast8_t group_len;
    char zero_fill;
    char align_mode;
    zlx_sign_fmt_t sign_mode;
    size_t z, req_width, arg_len, prec, arg_width, wparsed;
    uint32_t ucp;
    uint8_t radix;
    int64_t i64;
    char cmd;
    char esc_mode;
    size_t ofs, in_len, out_len;
    zlx_clconv_status_t cc;
    zlx_clconv_c_escape_t cectx;

    for (;;)
    {
        uint8_t const * sfmt = f;

        while (*f && *f != '$') f++;
        z = (size_t) (f - sfmt);
        if (z && writer(writer_context, sfmt, z) != z)
            return ZLX_FMT_WRITE_ERROR;
        if (*f == 0) break;
        zero_fill = 0;
        req_width = 0;
        sign_mode = ZLX_SIGN_NEG;
        align_mode = ALIGN_DEFAULT;
        esc_mode = STR_ESC_NONE;
        radix = 0;
        prec = SIZE_MAX;
        num_pfx = NULL;
        sep = '_';
        group_len = 64;
        arg_width = 0;
        arg_len = 0;
        f++;
        for (cmd = CMD_NONE; cmd == CMD_NONE; ++f)
        {
            switch (*f)
            {
            case '0':
                zero_fill = 1;
                continue;
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                for (req_width = 0; *f >= '0' && *f <= '9'; ++f)
                    req_width = req_width * 10 + *f - '0';
                --f;
                break;
            case 'c':
                ucp = va_arg(va, uint32_t);
                if (!zlx_ucp_is_valid(ucp)) return ZLX_FMT_MALFORMED;
                arg_len = zlxi_ucp_to_utf8(ucp, buffer);
                if (width_func(buffer, arg_len, &arg_width, &wparsed,
                               width_ctx))
                    return ZLX_FMT_WIDTH_ERROR;
                cmd = CMD_BUF;
                if (align_mode == ALIGN_DEFAULT) align_mode = ALIGN_LEFT;
                break;
            case 'b':
                sign_mode = ZLX_NO_SIGN;
                i64 = (uint8_t) va_arg(va, int);
            l_int:
                if (radix == 0) radix = 10;
                arg_len = zlx_i64_to_str(buffer, i64, sign_mode, radix, num_pfx,
                                         zero_fill ? (uint32_t) req_width : 1,
                                         group_len, sep);
                if (width_func(buffer, arg_len, &arg_width, &wparsed,
                               width_ctx))
                    return ZLX_FMT_WIDTH_ERROR;
                cmd = CMD_BUF;
                if (align_mode == ALIGN_DEFAULT) align_mode = ALIGN_RIGHT;
                break;
            case 'B':
                i64 = (int8_t) va_arg(va, int);
                goto l_int;
            case 'w':
                sign_mode = ZLX_NO_SIGN;
                i64 = (uint16_t) va_arg(va, int);
                goto l_int;
            case 'W':
                i64 = (int16_t) va_arg(va, int);
                goto l_int;
            case 'd':
                sign_mode = ZLX_NO_SIGN;
                i64 = va_arg(va, uint32_t);
                goto l_int;
            case 'D':
                i64 = va_arg(va, int32_t);
                goto l_int;
            case 'q':
                sign_mode = ZLX_NO_SIGN;
                i64 = va_arg(va, int64_t);
                goto l_int;
            case 'Q':
                i64 = va_arg(va, int64_t);
                goto l_int;
            case 'i':
                sign_mode = ZLX_NO_SIGN;
                i64 = va_arg(va, unsigned int);
                goto l_int;
            case 'I':
                i64 = va_arg(va, signed int);
                goto l_int;
            case 'l':
                sign_mode = ZLX_NO_SIGN;
                i64 = va_arg(va, long int);
                goto l_int;
            case 'L':
                i64 = va_arg(va, signed long int);
                goto l_int;
            case 'h':
                sign_mode = ZLX_NO_SIGN;
                i64 = (unsigned short int) va_arg(va, int);
                goto l_int;
            case 'H':
                i64 = (signed short int) va_arg(va, int);
                goto l_int;
            case 'z':
                sign_mode = ZLX_NO_SIGN;
                i64 = (int64_t) va_arg(va, size_t);
                goto l_int;
            case 'Z':
                i64 = va_arg(va, ptrdiff_t);
                goto l_int;
            case 'p':
                sign_mode = ZLX_NO_SIGN;
            case 'P':
                i64 = va_arg(va, intptr_t);
                if (radix == 0) radix = 16;
                goto l_int;
            case 'y':
                num_pfx = (uint8_t const *) "0b";
            case 'Y':
                radix = 2;
                break;
            case 'o':
                num_pfx = (uint8_t const *) "0o";
            case 'O':
                radix = 8;
                break;
            case 'n':
                num_pfx = (uint8_t const *) "0d";
            case 'N':
                radix = 10;
                break;
            case 'x':
                num_pfx = (uint8_t const *) "0x";
            case 'X':
                esc_mode = STR_ESC_HEX;
                radix = 16;
                break;
            case 's':
                str = va_arg(va, uint8_t const *);
                if (prec == SIZE_MAX) arg_len = zlx_u8a_zlen(str);
                else arg_len = prec;
                switch (esc_mode)
                {
                case STR_ESC_NONE:
                    cmd = CMD_STR;
                    if (width_func(str, arg_len, &arg_width, &wparsed,
                                   width_ctx))
                        return ZLX_FMT_WIDTH_ERROR;
                    break;
                case STR_ESC_HEX:
                    cmd = CMD_CONV;
                    conv = zlx_clconv_bin_to_hex;
                    conv_ctx = NULL;
                    break;
                case STR_ESC_C:
                    cmd = CMD_CONV;
                    zlx_clconv_c_escape_init(&cectx);
                    conv = zlx_clconv_c_escape;
                    conv_ctx = &cectx;
                    break;
                case STR_ESC_CUSTOM:
                    cmd = CMD_CONV;
                    break;
                }
                if (cmd == CMD_CONV && req_width)
                {
                    size_t width;
                    for (arg_width = 0, ofs = 0; ofs < arg_len; ofs += in_len)
                    {
                        cc = conv(str + ofs, arg_len - ofs,
                                  buffer, sizeof buffer,
                                  &in_len, &out_len, conv_ctx);
                        if (cc && cc != ZLX_CLCONV_FULL)
                            return ZLX_FMT_CONV_ERROR;
                        if (width_func(buffer, out_len, &width,
                                       &wparsed, width_ctx))
                            return ZLX_FMT_WIDTH_ERROR;
                        arg_width += width;
                    }
                    cc = conv(NULL, 0, buffer, sizeof buffer,
                              &in_len, &out_len, conv_ctx);
                    if (cc) return ZLX_FMT_CONV_ERROR;
                    if (width_func(buffer, out_len, &width, &wparsed, width_ctx))
                        return ZLX_FMT_WIDTH_ERROR;
                    arg_width += width;
                }
                break;
            case '.':
                ++f;
                if (*f == '*')
                {
                    prec = va_arg(va, size_t);
                }
                else
                {
                    for (prec = 0; *f >= '0' && *f <= '9'; ++f)
                        prec = prec * 10 + *f - '0';
                    --f;
                }
                break;
            case '<':
                align_mode = ALIGN_LEFT;
                break;
            case '>':
                align_mode = ALIGN_RIGHT;
                break;
            case '/':
                if (f[1] < '0' || f[1] > '9' || f[2] == 0)
                    return ZLX_FMT_MALFORMED;
                group_len = (uint_fast8_t) (f[1] - '0');
                sep = f[2];
                f += 2;
                break;
            case 'e':
                esc_mode = STR_ESC_C;
                break;
            case 'E':
                esc_mode = STR_ESC_CUSTOM;
                conv = va_arg(va, zlx_clconv_func_t);
                conv_ctx = va_arg(va, void *);
                break;
            default:
                return ZLX_FMT_MALFORMED;
            }
        }

        if (arg_width < req_width && align_mode == ALIGN_RIGHT)
        {
            size_t pad_width = req_width - arg_width;
            size_t clen;
            for (; pad_width; pad_width -= clen)
            {
                clen = pad_width;
                if (clen > sizeof(empty_spaces) - 1)
                    clen = sizeof(empty_spaces) - 1;
                if (writer(writer_context, empty_spaces, clen) != clen)
                    return ZLX_FMT_WRITE_ERROR;
            }
        }

        switch (cmd)
        {
        case CMD_BUF:
            z = writer(writer_context, buffer, arg_len);
            if (z != arg_len) return ZLX_FMT_WRITE_ERROR;
            break;
        case CMD_STR:
            z = writer(writer_context, str, arg_len);
            if (z != arg_len) return ZLX_FMT_WRITE_ERROR;
            break;
        case CMD_CONV:
            for (ofs = 0; ofs < arg_len; ofs += in_len)
            {
                cc = conv(str + ofs, arg_len - ofs,
                          buffer, sizeof buffer, &in_len, &out_len, conv_ctx);
                if (cc && cc != ZLX_CLCONV_FULL) return ZLX_FMT_CONV_ERROR;
                if (writer(writer_context, buffer, out_len) != out_len)
                    return ZLX_FMT_WRITE_ERROR;
            }
            cc = conv(NULL, 0, buffer, sizeof buffer, &in_len, &out_len,
                      conv_ctx);
            if (cc) return ZLX_FMT_CONV_ERROR;
            if (writer(writer_context, buffer, out_len) != out_len)
                return ZLX_FMT_WRITE_ERROR;
        }

        if (arg_width < req_width && align_mode == ALIGN_LEFT)
        {
            size_t pad_width = req_width - arg_width;
            size_t clen;
            for (; pad_width; pad_width -= clen)
            {
                clen = pad_width;
                if (clen > sizeof(empty_spaces) - 1)
                    clen = sizeof(empty_spaces) - 1;
                if (writer(writer_context, empty_spaces, clen) != clen)
                    return ZLX_FMT_WRITE_ERROR;
            }
        }

    }
    //if (writer(&nul, 1, writer_context) != 1) return ZLX_FMT_WRITE_ERROR;

    return 0;
}
#undef CMD_NONE
#undef CMD_BUF
#undef CMD_STR
#undef CMD_CONV
#undef ALIGN_DEFAULT
#undef ALIGN_LEFT
#undef ALIGN_RIGHT
#undef STR_ESC_NONE
#undef STR_ESC_C
#undef STR_ESC_HEX

/* zlx_wfmt *****************************************************************/
ZLX_API unsigned int ZLX_CALL zlx_wfmt
(
    zlx_writer_func_t writer,
    void * writer_context,
    zlx_unicode_text_width_measure_func_t width_func,
    void * width_ctx,
    char const * ZLX_RESTRICT fmt,
    ...
)
{
    va_list va;
    unsigned int rc;

    va_start(va, fmt);
    rc = zlx_vwfmt(writer, writer_context, width_func, width_ctx, fmt, va);
    va_end(va);
    return rc;
}

/* zlx_vfmt *****************************************************************/
ZLX_API unsigned int ZLX_CALL zlx_vfmt
(
    zlx_writer_func_t writer,
    void * writer_context,
    char const * ZLX_RESTRICT fmt,
    va_list va
)
{
    return zlx_vwfmt(writer, writer_context, zlx_utf8_term_width, NULL, fmt, va);
}

/* zlx_fmt ******************************************************************/
ZLX_API unsigned int ZLX_CALL zlx_fmt
(
    zlx_writer_func_t writer,
    void * writer_context,
    char const * ZLX_RESTRICT fmt,
    ...
)
{
    va_list va;
    unsigned int rc;

    va_start(va, fmt);
    rc = zlx_vfmt(writer, writer_context, fmt, va);
    va_end(va);
    return rc;
}

/* zlx_vfmt *****************************************************************/
ZLX_API ptrdiff_t ZLX_CALL zlx_vsfmt
(
    uint8_t * ZLX_RESTRICT out,
    size_t size,
    char const * ZLX_RESTRICT fmt,
    va_list va
)
{
    zlx_wbuf_t wb;
    unsigned int rv;

    if (!size) return -ZLX_FMT_WRITE_ERROR;
    zlx_wbuf_init(&wb, out, size - 1);
    rv = zlx_vfmt(zlx_wbuf_writer, &wb, fmt, va);
    out[wb.offset < size ? wb.offset : size - 1] = 0;
    /* wbuf_writer should never return write error */
    ZLX_ASSERT(rv != ZLX_FMT_WRITE_ERROR);

    return rv ? -(ptrdiff_t) rv : (ptrdiff_t) wb.offset;
}


/* zlx_sfmt *****************************************************************/
ZLX_API ptrdiff_t ZLX_CALL zlx_sfmt
(
    uint8_t * ZLX_RESTRICT out,
    size_t size,
    char const * ZLX_RESTRICT fmt,
    ...
)
{
    va_list va;
    ptrdiff_t rc;

    va_start(va, fmt);
    rc = zlx_vsfmt(out, size, fmt, va);
    va_end(va);
    return rc;
}

