#include "../../include/zlx/clconv/c_escape.h"
#include "../../include/zlx/int/fmt.h"

static uint32_t cet[8] =
{
    0xFFFFFFFF, 0x00000084, 0x10000000, 0x80000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

static char ec[] = "abtnvfr";

/* byte_c_escape_len ********************************************************/
static unsigned int byte_c_escape_len
(
    uint8_t v
)
{
    return (cet[v >> 5] & ((uint32_t) 1 << (v & 31))) == 0 ? 1
        : ((v >= 7 && v <= 13) || ((uint8_t) (v - 0x20) < 0x5F) ? 2: 4);
}

/* zlx_clconv_c_escape_force_hex ********************************************/
ZLX_API zlx_clconv_status_t ZLX_CALL zlx_clconv_c_escape_force_hex
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
    size_t i, o;
    uint_fast8_t r;
    (void) ctx;
    for (r = ZLX_CLCONV_OK, i = o = 0; i < in_len; ++i)
    {
        uint8_t v = in[i];
        unsigned int cl = byte_c_escape_len(v);
        if (o + cl > out_len) { r = ZLX_CLCONV_FULL; break; }
        if (cl == 1) out[o++] = v;
        else
        {
            out[o++] = '\\';

            if (cl == 4)
            {
                out[o++] = 'x';
                out[o++] = (uint8_t) zlx_digit_char_table[v >> 4];
                out[o++] = (uint8_t) zlx_digit_char_table[v & 15];
            }
            else if (v >= 7 && v <= 13) out[o++] = (uint8_t) ec[v - 7];
            else out[o++] = v;
        }
    }
    *in_used_len = i;
    *out_used_len = o;
    return r;
}

/* zlx_clconv_c_escape_init *************************************************/
ZLX_API void ZLX_CALL zlx_clconv_c_escape_init
(
    zlx_clconv_c_escape_t * ctx
)
{
    ctx->crt_byte = -1;
}

#include <stdio.h>
/* zlx_clconv_c_escape ******************************************************/
ZLX_API zlx_clconv_status_t ZLX_CALL zlx_clconv_c_escape
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
    zlx_clconv_c_escape_t * ZLX_RESTRICT cectx = ctx;
    int v = cectx->crt_byte;
    size_t i, o;
    zlx_clconv_status_t r;

    i = o = 0; r = ZLX_CLCONV_OK;
    if (v < 0 && in_len) v = in[i++];
    while (v >= 0)
    {
        if (cet[v >> 5] & ((uint32_t) 1 << (v & 31)))
        {
            /* must escape char */
            if (v < 7 || (v >= 14 && v < 32) || v >= 0x7F)
            {
                /* hex/octal escape needed */
                if (i == in_len && in) { r = ZLX_CLCONV_OK; break; }
                if (o + 4 > out_len) { r = ZLX_CLCONV_FULL; break; }
                out[o++] = '\\';
                if (!in || zlx_digit_from_char(in[i], 16) < 0)
                {
                    out[o++] = 'x';
                    out[o++] = (uint8_t) zlx_digit_char_table[v >> 4];
                    out[o++] = (uint8_t) zlx_digit_char_table[v & 15];
                }
                else
                {
                    out[o++] = (uint8_t) zlx_digit_char_table[v >> 6];
                    out[o++] = (uint8_t) zlx_digit_char_table[(v >> 3) & 7];
                    out[o++] = (uint8_t) zlx_digit_char_table[v & 7];
                }
            }
            else
            {
                // short escape
                if (o + 2 > out_len) { r = ZLX_CLCONV_FULL; break; }
                out[o++] = '\\';
                out[o++] = (uint8_t) (v < 32 ? ec[v - 7] : v);
            }
        }
        else
        {
            // non-escaped char
            if (o == out_len) { r = ZLX_CLCONV_FULL; break; }
            out[o++] = (uint8_t) v;
        }
        v = (i == in_len) ? -1 : in[i++];
    }
    cectx->crt_byte = v;
    *in_used_len = i;
    *out_used_len = o;
    return r;
}

