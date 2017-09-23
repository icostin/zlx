#include "../include/zlx/base.h"
#include "../include/zlx/int_fmt.h"
#include "../include/zlx/stdarray.h"
#include "../include/zlx/assert.h"

ZLX_API char const zlx_digit_char_table[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* zlx_u64_to_str ***********************************************************/
ZLX_API size_t ZLX_CALL zlx_u64_to_str
(
    uint8_t * str,
    uint64_t value,
    uint_fast8_t radix,
    uint32_t width,
    uint_fast8_t group,
    uint_fast8_t sep
)
{
    unsigned int g, a, b, i;

    ZLX_ASSERT(radix >= 2);
    ZLX_ASSERT(radix <= 36);
    for (i = 0, g = 0;;)
    {
        uint8_t digit;

        digit = value % radix;
        value /= radix;
        str[i++] = zlx_digit_char_table[digit];
        if (value == 0 && i >= width) break;
        if (++g == group)
        {
            str[i++] = sep;
            g = 0;
            if (value == 0 && i >= width) break;
        }
    }

    str[i] = 0;
    for (a = 0, b = i - 1; a < b; a++, b--)
    {
        uint8_t ch = str[a];
        str[a] = str[b];
        str[b] = ch;
    }
    return i;
}

/* zlx_i64_to_str ***********************************************************/
ZLX_API size_t ZLX_CALL zlx_i64_to_str
(
    uint8_t * str,
    int64_t value,
    uint_fast8_t sign_mode,
    uint_fast8_t radix,
    uint8_t const * prefix,
    uint32_t width,
    uint_fast8_t group,
    uint_fast8_t sep
)
{
    uint8_t * start = str;
    size_t w;

    switch (sign_mode)
    {
    case ZLX_NO_SIGN:
        break;
    case ZLX_SIGN_NEG:
        if (value >= 0) break;
        value = -value;
        *str++ = '-';
        break;
    case ZLX_SIGN_ALWAYS:
        if (value < 0)
        {
            *str++ = '-';
            value = -value;
            break;
        }
        *str++ = value ? '+' : ' ';
        break;
    case ZLX_SIGN_ALIGN:
        if (value < 0)
        {
            *str++ = '-';
            value = -value;
            break;
        }
        *str++ = ' ';
        break;
    }
    if (prefix) str = zlx_u8a_zcopy(str, prefix);
    w = str - start;
    if (w > width) width = 0;
    else width -= w;
    return w + zlx_u64_to_str(str, (uint64_t) value, radix, width, group, sep);
}

/* zlx_u64_from_str *********************************************************/
ZLX_API uint_fast8_t ZLX_CALL zlx_u64_from_str
(
    uint8_t const * ZLX_RESTRICT str,
    size_t len,
    uint_fast8_t radix,
    uint64_t * ZLX_RESTRICT value,
    size_t * ZLX_RESTRICT used_len
)
{
    size_t i;
    uint64_t v;
    uint_fast8_t r = 0;

    if (len == 0)
    {
        *value = 0;
        *used_len = 0;
        return ZLX_U64_STOP;
    }
    i = 0;
    if (radix == 0)
    {
        if (*str != '0' || len == 1) radix = 10;
        else
        {
            switch (str[1])
            {
            case 'b': radix = 2; i = 2; break;
            case 'o': radix = 8; i = 2; break;
            case 'd': radix = 10; i = 2; break;
            case 'x': radix = 16; i = 2; break;
            default:
                radix = 10;
            }
        }
    }

    for (v = 0; i < len; ++i)
    {
        uint8_t digit;
        uint64_t u, w;
        if (str[i] >= '0' && str[i] <= '9') digit = str[i] - '0';
        else
        {
            digit = str[i] | 0x20;
            if (digit >= 'a' && digit <= 'z') digit -= ('a' - 10);
            else digit = radix;
        }
        if (digit >= radix)
        {
            r = ZLX_U64_STOP;
            break;
        }
        w = v * radix;
        if (w / radix != v)
        {
            r = ZLX_U64_OVERFLOW;
            break;
        }
        u = w + digit;
        if (u < w)
        {
            r = ZLX_U64_OVERFLOW;
            break;
        }
        v = u;
    }
    *value = v;
    if (used_len) *used_len = i;
    return r;
}

