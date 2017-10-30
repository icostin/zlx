#include "../../include/zlx/clconv/hex.h"
#include "../../include/zlx/int/fmt.h"
#include "../../include/zlx/int/array.h"

/* zlx_clconv_bin_to_hex ****************************************************/
ZLX_API zlx_clconv_status_t ZLX_CALL zlx_clconv_bin_to_hex
(
    uint8_t const * ZLX_RESTRICT in,
    size_t in_len,
    uint8_t * ZLX_RESTRICT out,
    size_t out_len,
    size_t * ZLX_RESTRICT in_used_len,
    size_t * ZLX_RESTRICT out_used_len,
    void * ctx
)
{
    size_t i, o, ol;

    (void) ctx;
    ol = out_len & (SIZE_MAX << 1);
    for (i = o = 0; i < in_len && o < ol; i++)
    {
        out[o++] = (uint8_t) zlx_digit_char_table[in[i] >> 4];
        out[o++] = (uint8_t) zlx_digit_char_table[in[i] & 15];
    }
    *in_used_len = i;
    *out_used_len = o;
    return (i == in_len) ? ZLX_CLCONV_OK : ZLX_CLCONV_FULL;
}


/* zlx_clconv_hex_to_bin ****************************************************/
ZLX_API zlx_clconv_status_t ZLX_CALL zlx_clconv_hex_to_bin
(
    uint8_t const * ZLX_RESTRICT in,
    size_t in_len,
    uint8_t * ZLX_RESTRICT out,
    size_t out_len,
    size_t * ZLX_RESTRICT in_used_len,
    size_t * ZLX_RESTRICT out_used_len,
    void * ctx
)
{
    uint8_t const * skip_list = ctx;
    size_t i, o;
    zlx_clconv_status_t r;

    for (r = 0, i = o = 0; i < in_len; i++)
    {
        int lo, hi = zlx_digit_from_char(in[i], 16);
        if (hi < 0)
        {
            if (skip_list && zlx_u8a_scan(skip_list, in[i])) continue;
            r = ZLX_CLCONV_MALFORMED;
            break;
        }
        if (++i == in_len)
        {
            r = ZLX_CLCONV_INCOMPLETE;
            --i;
            break;
        }
        lo = zlx_digit_from_char(in[i], 16);
        if (lo < 0)
        {
            r = ZLX_CLCONV_MALFORMED;
            break;
        }
        if (o == out_len)
        {
            r = ZLX_CLCONV_FULL;
            --i;
            break;
        }
        out[o++] = (uint8_t) ((hi << 4) | lo);
    }
    *in_used_len = i;
    *out_used_len = o;
    return r;
}
