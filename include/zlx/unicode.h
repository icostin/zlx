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

#define ZLX_UTF32LE_DEC                         (1 << 0)
#define ZLX_UTF32BE_DEC                         (2 << 0)
#define ZLX_UTF16LE_DEC                         (3 << 0)
#define ZLX_UTF16BE_DEC                         (4 << 0)
#define ZLX_UTF8_DEC                            (5 << 0)

#define ZLX_UTF32LE_ENC                         (1 << 3)
#define ZLX_UTF32BE_ENC                         (2 << 3)
#define ZLX_UTF16LE_ENC                         (3 << 3)
#define ZLX_UTF16BE_ENC                         (4 << 3)
#define ZLX_UTF8_ENC                            (5 << 3)

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
#define ZLX_UTF_ERR_NUL_BYTE                    (-2)
#define ZLX_UTF_ERR_LEAD                        (-3)
#define ZLX_UTF_ERR_CONT1                       (-4)
#define ZLX_UTF_ERR_CONT2                       (-5)
#define ZLX_UTF_ERR_CONT3                       (-6)
#define ZLX_UTF_ERR_OVERLY_LONG                 (-7)
#define ZLX_UTF_ERR_SURROGATE                   (-8)
#define ZLX_UTF_ERR_CP_TOO_BIG                  (-9)
#define ZLX_UTF_ERR_NO_CONV                     (-10)
#define ZLX_UTF_ERR_NON_PRINTABLE               (-11)


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

/* zlx_ucp_to_utf8_size *****************************************************/
/**
 *  Computes the size in bytes of the UTF8 encoded Unicode codepoint.
 *  @param ucp [in]
 *      codepoint (any integer strictly under ZLX_UNICODE_CODEPOINT_LIMIT)
 *  @param flags [in]
 *      bitmask with only the following relevant flags:
 *      - #ZLX_UTF8_ENC_TWO_BYTE_NUL
 *      - #ZLX_UTF8_ENC_BREAK_SUPPLEM
 *  @returns
 *      a value from 1 to 6
 *  @note
 *      specifying @a flags as
 *      #ZLX_UTF8_ENC_TWO_BYTE_NUL | #ZLX_UTF8_ENC_BREAK_SUPPLEM
 *      correponds to MUTF8 encoding (modified utf8 encoding)
 */
ZLX_API size_t ZLX_CALL zlx_ucp_to_utf8_size
(
    uint32_t ucp,
    unsigned int flags
);

/* zlx_ucp_to_utf16_size ****************************************************/
/**
 *  Computes the size in bytes of the UTF16 encoded Unicode codepoint.
 *  @param ucp [in]
 *      codepoint (any integer strictly under ZLX_UNICODE_CODEPOINT_LIMIT)
 *  @param flags [ignored]
 *      ignored
 *  @returns
 *      2 or 4
 */
ZLX_API size_t ZLX_CALL zlx_ucp_to_utf16_size // in bytes
(
    uint32_t ucp,
    unsigned int flags
);

/* zlx_ucp_to_utf32_size ****************************************************/
/**
 *  Computes the size in bytes of the UTF32 encoded Unicode codepoint.
 *  @param ucp [in]
 *      codepoint (any integer strictly under ZLX_UNICODE_CODEPOINT_LIMIT)
 *  @param flags [in]
 *      0 or #ZLX_UTF32_ENC_BREAK_SUPPLEM
 *  @returns
 *      4 or 8
 *  @note
 *      specifying #ZLX_UTF32_ENC_BREAK_SUPPLEM causes a supplemental plane
 *      codepoint (>= 0x10000) to be broken into a surrogate pair
 */
ZLX_API size_t ZLX_CALL zlx_ucp_to_utf32_size
(
    uint32_t ucp,
    unsigned int flags
);

/* zlx_ucp_to_utf8 **********************************************************/
/**
 *  Encodes a Unicode codepoint in UTF8.
 *  @param ucp [in]
 *      codepoint
 *  @param flags [in]
 *      bitmask with only the following relevant flags:
 *      - #ZLX_UTF8_ENC_TWO_BYTE_NUL
 *      - #ZLX_UTF8_ENC_BREAK_SUPPLEM
 *  @param out [out]
 *      buffer to write the encoded character; the buffer must be large enough
 *      to hold the encoded character; the size needed should be obtained with
 *      a prior call to zlx_ucp_to_utf8_size()
 *  @note
 *      specifying @a flags as
 *      #ZLX_UTF8_ENC_TWO_BYTE_NUL | #ZLX_UTF8_ENC_BREAK_SUPPLEM
 *      correponds to MUTF8 encoding (modified utf8 encoding)
 */
ZLX_API size_t ZLX_CALL zlx_ucp_to_utf8
(
    uint32_t ucp,
    unsigned int flags,
    uint8_t * ZLX_RESTRICT out
);

/* zlx_ucp_to_utf16le *******************************************************/
/**
 *  Encodes a Unicode codepoint in UTF16LE.
 *  @param ucp [in]
 *      codepoint
 *  @param flags [ignored]
 *      ignored
 *  @param out [out]
 *      buffer to write the encoded character; the buffer must be large enough
 *      to hold the encoded character; the size needed should be obtained with
 *      a prior call to zlx_ucp_to_utf16_size()
 */
ZLX_API size_t ZLX_CALL zlx_ucp_to_utf16le
(
    uint32_t ucp,
    unsigned int flags,
    uint8_t * ZLX_RESTRICT out
);

/* zlx_ucp_to_utf32le *******************************************************/
/**
 *  Encodes a Unicode codepoint in UTF32.
 *  @param ucp [in]
 *      codepoint
 *  @param flags [in]
 *      0 or #ZLX_UTF32_ENC_BREAK_SUPPLEM
 *  @param out [out]
 *      buffer to write the encoded character; the buffer must be large enough
 *      to hold the encoded character; the size needed should be obtained with
 *      a prior call to zlx_ucp_to_utf32_size()
 */
ZLX_API size_t ZLX_CALL zlx_ucp_to_utf32le
(
    uint32_t ucp,
    unsigned int flags,
    uint8_t * ZLX_RESTRICT out
);

/* zlx_uconv ****************************************************************/
/**
 *  Converts unicode text from one encoding to another.
 *  @param flags [in]
 *      conversion flags; must contain just one of:
 *      - #ZLX_UTF8_DEC
 *      - #ZLX_UTF16_DEC
 *      - #ZLX_UTF32_DEC
 *      and just one of:
 *      - #ZLX_UTF8_ENC
 *      - #ZLX_UTF16_ENC
 *      - #ZLX_UTF32_ENC
 *      and optionally a meaningful combination of the following:
 *      - #ZLX_UTF8_DEC_NO_NUL_BYTE
 *      - #ZLX_UTF8_DEC_TWO_BYTE_NUL
 *      - #ZLX_UTF8_DEC_OVERLY_LONG
 *      - #ZLX_UTF8_DEC_SURROGATES
 *      - #ZLX_UTF8_DEC_SURROGATE_PAIRS
 *      - #ZLX_UTF16_DEC_UNPAIRED_SURROGATES
 *      - #ZLX_UTF16_DEC_NO_PAIRING
 *      - #ZLX_UTF8_ENC_TWO_BYTE_NUL
 *      - #ZLX_UTF8_ENC_BREAK_SUPPLEM
 *      - #ZLX_UTF32_ENC_BREAK_SUPPLEM
 *  @param in [in]
 *      input buffer
 *  @param in_size [in]
 *      size in bytes of input data
 *  @param out [out]
 *      output buffer
 *  @param out_size [in]
 *      size of output buffer
 *  @param in_pos [out, opt]
 *      if non-NULL stores the byte index in the input buffer where convesion
 *      stopped (on success and on error)
 *
 *  @returns the size needed to hold all converted input; on error a negative
 *      value is returned (see ZLX_UTF_ERR_[...])
 */
ZLX_API ptrdiff_t ZLX_CALL zlx_uconv
(
    unsigned int flags,
    uint8_t const * ZLX_RESTRICT in,
    size_t in_size,
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
 *  @retval 2 double width character
 */
ZLX_API int ZLX_CALL zlx_ucp_term_width (uint32_t ucp);

/* zlx_unicode_text_width_measure_func_t ************************************/
/**
 *  Function pointer type for measuring width of Unicode text.
 *  The encoding is not specified so it must be deduced from the actual
 *  function or from the context parameter.
 */
typedef int (ZLX_CALL * zlx_unicode_text_width_measure_func_t)
    (
        uint8_t const * ZLX_RESTRICT data,
        size_t size,
        size_t * ZLX_RESTRICT width,
        size_t * ZLX_RESTRICT parsed_size,
        void * ctx
    );

/**
 *  Computes the width in character cells for a typical terminal.
 *  @param data [in]
 *      printable UTF8 string
 *  @param size [in]
 *      size of UTF8 data in bytes
 *  @param width [out]
 *      receives the width of parsed text (on success and on error)
 *  @param parsed_size [out]
 *      receives how much from the input buffer was parsed successfully
 *  @param ctx [unused]
 *      ignored
 *
 *  @returns
 *      0 on success or negative for error
 */
ZLX_API int ZLX_CALL zlx_utf8_term_width
(
    uint8_t const * ZLX_RESTRICT data,
    size_t size,
    size_t * ZLX_RESTRICT width,
    size_t * ZLX_RESTRICT parsed_size,
    void * ctx
);


ZLX_C_DECL_END

/** @} */

#endif /* _ZLX_UNICODE_H */

