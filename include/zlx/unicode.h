#ifndef _ZLX_UNICODE_H
#define _ZLX_UNICODE_H

/** @defgroup unicode Unicode support
 *  Set of functions that allow converting between various Unicode encodings,
 *  or computing the displayed width of a string in a character-based terminal.
 *  @{ */

#include "lib.h"
#include "int/type.h"

ZLX_C_DECL_BEGIN

#define ZLX_UNICODE_MAX_CODEPOINT               ((uint32_t) 0x10FFFF)
#define ZLX_UNICODE_CODEPOINT_LIMIT             ((uint32_t) 0x110000)

#define ZLX_UTF32LE_DEC                         (0 << 0)
#define ZLX_UTF32BE_DEC                         (1 << 0)
#define ZLX_UTF16LE_DEC                         (2 << 0)
#define ZLX_UTF16BE_DEC                         (3 << 0)
#define ZLX_UTF8_DEC                            (4 << 0)

#define ZLX_UTF32LE_ENC                         (0 << 3)
#define ZLX_UTF32BE_ENC                         (1 << 3)
#define ZLX_UTF16LE_ENC                         (2 << 3)
#define ZLX_UTF16BE_ENC                         (3 << 3)
#define ZLX_UTF8_ENC                            (4 << 3)

#define ZLX_UTF32_DEC_SURROGATES                (1 << 6)

#define ZLX_UTF16_DEC_UNPAIRED_SURROGATES       (1 << 7)
#define ZLX_UTF16_DEC_NO_PAIRING                (1 << 8)

#define ZLX_UTF8_DEC_NO_NUL_BYTE                (1 << 9)
#define ZLX_UTF8_DEC_TWO_BYTE_NUL               (1 << 10)
#define ZLX_UTF8_DEC_OVERLY_LONG                (1 << 11)
#define ZLX_UTF8_DEC_SURROGATES                 (1 << 12)
#define ZLX_UTF8_DEC_SURROGATE_PAIRS            (1 << 13)

#define ZLX_UTF32_ENC_BREAK_SUPPLEM             (1 << 14)
#define ZLX_UTF8_ENC_TWO_BYTE_NUL               (1 << 15)
#define ZLX_UTF8_ENC_BREAK_SUPPLEM              (1 << 16)

#define ZLX_UTF_ERR_TRUNC                       (-1)
#define ZLX_UTF_ERR_NUL_BYTE                   (-2)
#define ZLX_UTF_ERR_LEAD                       (-3)
#define ZLX_UTF_ERR_CONT1                      (-4)
#define ZLX_UTF_ERR_CONT2                      (-5)
#define ZLX_UTF_ERR_CONT3                      (-6)
#define ZLX_UTF_ERR_OVERLY_LONG                (-7)
#define ZLX_UTF_ERR_SURROGATE                   (-8)
#define ZLX_UTF_ERR_CP_TOO_BIG                 (-9)
#define ZLX_UTF_ERR_NO_CONV                     (-10)


/* zlx_decode_ucp_func_t ****************************************************/
/**
 *  Decoder function that produces a single Unicode codepoint.
 *  @param in [in]
 *      data to decode
 *  @param end [in]
 *      end of data
 *  @param flags [in]
 *      one of:
 *      - #ZLX_UTF32LE_DEC
 *      - #ZLX_UTF32BE_DEC
 *      - #ZLX_UTF16LE_DEC
 *      - #ZLX_UTF16BE_DEC
 *      - #ZLX_UTF8_DEC
 *      optionally ORed with a meaningful mask of:
 *      - #ZLX_UTF32_DEC_SURROGATES
 *      - #ZLX_UTF16_DEC_UNPAIRED_SURROGATES
 *      - #ZLX_UTF16_DEC_NO_PAIRING
 *      - #ZLX_UTF8_DEC_NO_NUL_BYTE
 *      - #ZLX_UTF8_DEC_OVERLY_LONG
 *      - #ZLX_UTF8_DEC_SURROGATES
 *      - #ZLX_UTF8_DEC_SURROGATE_PAIRS
 *  @param out [out]
 *      decoded codepoint 
 *  @returns 
 *      on success a positive integer representing the number of bytes used
 *      to decode the codepoint, or a negative interger on error 
 *      - ZLX_UTF_ERR_TRUNC
 *      - ZLX_UTF_ERR_NUL_BYTE
 *      - ZLX_UTF_ERR_LEAD
 *      - ZLX_UTF_ERR_CONT1
 *      - ZLX_UTF_ERR_CONT2
 *      - ZLX_UTF_ERR_CONT3
 *      - ZLX_UTF_ERR_OVERLY_LONG
 *      - ZLX_UTF_ERR_SURROGATE
 *      - ZLX_UTF_ERR_CP_TOO_BIG
 *      - ZLX_UTF_ERR_NO_CONV
 */
typedef ptrdiff_t (ZLX_CALL * zlx_decode_ucp_func_t) 
    (
        uint8_t const * in,
        uint8_t const * end,
        unsigned int flags,
        uint32_t * ZLX_RESTRICT out
    );

/* zlx_encoded_ucp_size_func_t **********************************************/
/**
 *  Type for functions computing the size of an encoded codepoint.
 */
typedef size_t (ZLX_CALL * zlx_encoded_ucp_size_func_t)
    (
        uint32_t ucp,
        unsigned int flags
    );

/* zlx_encode_ucp_func_t ****************************************************/
/**
 *  Encoder function for single Unicode codepoints.
 */
typedef size_t (ZLX_CALL * zlx_encode_ucp_func_t)
    (
        uint32_t ucp,
        unsigned int flags,
        uint8_t * ZLX_RESTRICT out
    );

/* zlx_ucp_is_valid *********************************************************/
/**
 *  Tells if a Unicode codepoint is valid.
 *  @retval 1 valid codepoint
 *  @retval 0 invalid codepoint
 */
ZLX_INLINE unsigned int zlx_ucp_is_valid
(
    uint32_t ucp
)
{
    return ucp < 0x110000 && ((ucp & 0x1FF800) != 0xD800);
}

/* zlx_ucp_to_utf8_len ******************************************************/
ZLX_INLINE unsigned int zlx_ucp_to_utf8_len
(
    uint32_t ucp
)
{
    return ucp < 0x80 ? 1 : (ucp < 0x800 ? 2 : 3 + (ucp >= 0x10000));
}

/* zlxi_ucp_to_utf8 *********************************************************/
/**
 *  Encodes a single Unicode codepoint in UTF8.
 *  This works to encode any valid and invalid codepoints below 0x10FFFF.
 *  @returns number of bytes written.
 *  @warning the output buffer must be large enough to store the encoded
 *  value.
 */
ZLX_INLINE unsigned int zlxi_ucp_to_utf8
(
    uint32_t ucp,
    uint8_t * out
)
{
    if (ucp < 0x80) { *out = (uint8_t) ucp; return 1; }
    if (ucp < 0x800)
    {
        out[0] = (uint8_t) (0xC0 | (ucp >> 6));
        out[1] = (uint8_t) (0x80 | (ucp & 0x3F));
        return 2;
    }
    if (ucp < 0x10000)
    {
        out[0] = (uint8_t) (0xE0 | (ucp >> 12));
        out[1] = (uint8_t) (0x80 | ((ucp >> 6) & 0x3F));
        out[2] = (uint8_t) (0x80 | (ucp & 0x3F));
        return 3;
    }
    out[0] = (uint8_t) (0xF0 | (ucp >> 18));
    out[1] = (uint8_t) (0x80 | ((ucp >> 12) & 0x3F));
    out[2] = (uint8_t) (0x80 | ((ucp >> 6) & 0x3F));
    out[3] = (uint8_t) (0x80 | (ucp & 0x3F));
    return 4;
}

/* zlx_utf8_lead_to_len *****************************************************/
/**
 *  Returns the UTF-8 length given a valid UTF-8 lead byte.
 */
ZLX_INLINE size_t zlx_utf8_lead_to_len
(
    uint8_t lead_byte
)
{
    /* 0xFx -> 4, 0xEx -> 3, 0x[CD]x -> 2, 0x[89AB] -> illegal, 0x[0-7]x -> 1*/
    return ((0xE5000000 >> ((lead_byte >> 3) & 0x1E)) & 3) + 1;
}

/* zlx_utf8_lead_valid ******************************************************/
/**
 *  Tells if the given byte is a valid UTF-8 lead byte.
 *  @note this does not reject lead bytes that can be used in overly long
 *  encoded chars (such as 0xC0, 0xC1).
 */
ZLX_INLINE unsigned int zlx_utf8_lead_valid
(
    uint8_t lead_byte
)
{
    return (0x7F00FFFF >> (lead_byte >> 3)) & 1;
}

/* zlx_utf32le_to_ucp *******************************************************/
ZLX_API ptrdiff_t ZLX_CALL zlx_utf32le_to_ucp
(
    uint8_t const * in,
    uint8_t const * end,
    unsigned int flags,
    uint32_t * ZLX_RESTRICT out
);

/* zlx_utf16le_to_ucp *******************************************************/
ZLX_API ptrdiff_t ZLX_CALL zlx_utf16le_to_ucp
(
    uint8_t const * in,
    uint8_t const * end,
    unsigned int flags,
    uint32_t * ZLX_RESTRICT out
);

/* zlx_utf8_to_ucp **********************************************************/
ZLX_API ptrdiff_t ZLX_CALL zlx_utf8_to_ucp
(
    uint8_t const * in,
    uint8_t const * end,
    unsigned int flags,
    uint32_t * ZLX_RESTRICT out
);

/* zlx_uconv ****************************************************************/
/**
 *  Converts unicode text from one encoding to another.
 *  @returns the size needed to hold all converted input; on error a negative
 *      value is returned (see ZLX_UTF[...]_ERR_[...])
 */
ZLX_API ptrdiff_t ZLX_CALL zlx_uconv
(
    uint8_t const * ZLX_RESTRICT in,
    size_t in_size,
    unsigned int flags,
    uint8_t * ZLX_RESTRICT out,
    size_t out_size,
    size_t * in_pos
);

/* zlx_ucp_term_width *******************************************************/
/**
 *  Computes the typical width of a Unicode codepoint when displayed in a
 *  character based terminal.
 *  @retval -1 Unicode codepoint invalid or not printable
 *  @retval 0 zero-width character (f.ex. combining characters)
 *  @retval 1 normal width character
 *  @retval 2 double width characted
 */
ZLX_API int ZLX_CALL zlx_ucp_term_width (uint32_t ucp);

/* zlx_utf8_term_width ******************************************************/
/**
 *  @param obj [unused]
 *  @param data [in]
 *      printable UTF8 string
 *  @param size [in]
 *      size of UTF8 data in bytes
 *  @returns 
 *      width in terminal character cells of the given text or a negative
 *      number for errors
 *  @retval -1 bad UTF8
 *  @retval -2 non printable character (such as control char)
 *  @retval -3 int overflow storing width
 */
ZLX_API ptrdiff_t ZLX_CALL zlx_utf8_term_width
(
    void * ZLX_RESTRICT obj,
    uint8_t const * ZLX_RESTRICT data,
    size_t size
);

ZLX_C_DECL_END

/** @} */

#endif /* _ZLX_UNICODE_H */

