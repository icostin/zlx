#ifndef _ZLX_INT_FMT_H
#define _ZLX_INT_FMT_H

#include "base.h"
#include "int.h"

ZLX_API extern char const zlx_digit_char_table[37];

/* zlx_digit_to_char ********************************************************/
/**
 *  Converts an int in the range 0-36 to its corresponding character 
 *  representing the printable digit.
 */
ZLX_INLINE unsigned int zlx_digit_to_char (unsigned int digit)
{
    return (unsigned int) zlx_digit_char_table[digit];
}

ZLX_API unsigned int zlxni_digit_to_char (unsigned int digit);

/* zlx_digit_from_char ******************************************************/
/**
 *  Decodes the digit from a single ASCII/Unicode character
 *  @returns digit value
 *  @retval -1 character is not a valid digit
 */
ZLX_INLINE int zlx_digit_from_char (uint32_t ch, uint_fast8_t radix)
{
    unsigned int digit;
    digit = ch - '0';
    if (digit > 9)
    {
        digit = (ch | 0x20) - 'a';
        if (digit <= 25) digit += 10;
    }
    return (digit >= radix) ? -1 : (int) digit;
}

ZLX_API int zlxni_digit_from_char (uint32_t ch, uint_fast8_t radix);

/* zlx_u64_to_str ***********************************************************/
/**
 *  Converts the given unsigned 64-bit int to ASCII NUL-terminated string.
 *  @param [out]    str     output buffer; caller must ensure is large enough
 *                          for the result
 *  @param [in]     value   value to convert
 *  @param [in]     radix   numeration base; must be in range 2 - 36
 *  @param [in]     width   desired width of the output; this is a guaranteed
 *                          minimum size for the output, not a maximum;
 *                          if the value is large it may cause the output to
 *                          exceed this limit
 *  @param [in]     group   how many digits to group together before using a
 *                          separator char; use 64 or above to disable grouping
 *  @param [in]     sep     separator char
 *  @returns        N       number of chars outputted, not including the
 *                          NUL terminator
 *  @note on DEBUG builds if radix is not in the valid range it will output
 *  the string "BAD_RADIX"
 */
ZLX_API size_t ZLX_CALL zlx_u64_to_str
(
    uint8_t * str,
    uint64_t value,
    uint_fast8_t radix,
    uint32_t width,
    uint_fast8_t group,
    uint_fast8_t sep
);

#define ZLX_NO_SIGN 0
    /**< consider the value unsigned (flag for zlx_i64_to_str#sign_mode) */

#define ZLX_SIGN_NEG 1
    /**< only put the sign if the value is negative
     *  (flag for zlx_i64_to_str#sign_mode) */

#define ZLX_SIGN_ALWAYS 2
    /**< use '-' for negative, space for 0 and + for positive numbers
     *  (flag for zlx_i64_to_str#sign_mode) */

#define ZLX_SIGN_ALIGN 3
    /**< use '-' for negative, space for non-negative numbers
     *  (flag for zlx_i64_to_str#sign_mode) */

/* zlx_i64_to_str ***********************************************************/
/**
 *  Converts the given int to ASCII string.
 *  @param [out]    str     output buffer; caller must ensure is large enough
 *                          for the result
 *  @param [in]     value   value to convert
 *  @param [in]     sign_mode   one of: #ZLX_NO_SIGN, #ZLX_SIGN_NEG,
 *                          ZLX_SIGN_ALWAYS, #ZLX_SIGN_ALIGN
 *  @param [in]     radix   numeration base; must be in range 2 - 36
 *  @param [in]     prefix  NUL-terminated C-string to be put in front of the
 *                          digits (after the sign, if any); can be NULL
 *  @param [in]     width   desired width of the output; this is a guaranteed
 *                          minimum size for the output, not a maximum;
 *                          if the value is large it may cause the output to
 *                          exceed this limit
 *  @param [in]     group   how many digits to group together before using a
 *                          separator char; use 64 or above to disable grouping
 *  @param [in]     sep     separator char
 *  @returns                number of chars outputted, not including the
 *                          NUL terminator
 */
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
);

#define ZLX_U64_STOP 1 /**< non-digit encountered */
#define ZLX_U64_OVERFLOW 2 /**< number parsed exceeds uint64_t capacity */

/* zlx_u64_from_str *********************************************************/
/**
 *  Converts a string to unsigned int.
 *  @param [in]     str     pointer to data
 *  @param [in]     len     length of input string
 *  @param [in]     radix   numeration base; 0 is magic for autodetect
 *  @param [out]    value   will be filled with the parsed value
 *  @param [out]    used_len filled with number of bytes parsed; can be NULL
 *  @retval 0 parsing successful, full length used
 *  @retval ZLX_U64_STOP non-digit encountered
 *  @retval ZLX_U64_OVERFLOW
 */
ZLX_API uint_fast8_t ZLX_CALL zlx_u64_from_str
(
    uint8_t const * ZLX_RESTRICT str,
    size_t len,
    uint_fast8_t radix,
    uint64_t * ZLX_RESTRICT value,
    size_t * ZLX_RESTRICT used_len
);

#endif
