#ifndef _ZLX_FMT_H
#define _ZLX_FMT_H

/** @defgroup fmt Formatted printing
 *  This module provides support for format-printing (printf-like but with
 *  different escapes).
 *
 *  @{
 */

#include <stdarg.h>
#include "lib.h"
#include "writer/interface.h"
#include "unicode.h"
#include "int/sizerr.h"

ZLX_C_DECL_BEGIN

/** @enum zlx_fmt_status_enum
 *  Status for formatting.
 *  @var zlx_fmt_status_t::ZLX_FMT_OK
 *      Formatting ok.
 *  @var zlx_fmt_status_t::ZLX_FMT_MALFORMED
 *      bad format string error code
 *  @var zlx_fmt_status_t::ZLX_FMT_WIDTH_ERROR
 *      width function returned error
 *  @var zlx_fmt_status_t::ZLX_FMT_WRITE_ERROR
 *      write error
 *  @var zlx_fmt_status_t::ZLX_FMT_CONV_ERROR
 *      conversion error during escaping of some string
 *  @var zlx_fmt_status_t::ZLX_FMT_NO_CODE
 *      feature not implemented
 *  @typedef zlx_fmt_status_t
 *      Type for enum zlx_fmt_status_enum.
 **/
ZLX_ENUM_DECL(zlx_fmt_status_t, zlx_fmt_status, 0,
    ZLX_FMT_OK,
    ZLX_FMT_MALFORMED,
    ZLX_FMT_WIDTH_ERROR,
    ZLX_FMT_WRITE_ERROR,
    ZLX_FMT_CONV_ERROR,
    ZLX_FMT_NO_CODE);

/** @struct zlx_fmt_sizerr_struct
 *  Struct representing a size returned by a successful formatting operation 
 *  or a status from zlx_fmt_status_t on error.
 *  @fn int zlx_fmt_sizerr_is_size (zlx_fmt_sizerr_t fse);
 *  @fn int zlx_fmt_sizerr_is_error (zlx_fmt_sizerr_t fse);
 *  @fn size_t zlx_fmt_sizerr_to_size (zlx_fmt_sizerr_t fse);
 *  @fn zlx_fmt_status_t zlx_fmt_sizerr_to_error (zlx_fmt_sizerr_t fse);
 */
ZLX_SIZERR_DECL(zlx_fmt_sizerr_t, zlx_fmt_sizerr, zlx_fmt_status_t);

/* zlx_vfmt *****************************************************************/
/**
 *  Writes formatted UTF-8 text (similar to printf formatting).
 *  @retval 0
 *      success
 *  @retval ZLX_FMT_MALFORMED
 *      bad format string
 *  @retval ZLX_FMT_WIDTH_ERROR
 *      output text contains non-printable characters (or characters rejected
 *      by the width measuring function)
 *  @retval ZLX_FMT_WRITE_ERROR
 *      the writer function reports error
 *  @retval ZLX_FMT_CONV_ERROR
 *      the escape converter reported an error
 *
 *  @b FORMAT: "$" [[ALIGN] WIDTH] ["." PREC] ["/" GROUP_LEN] [MOD] TYPE
 *
 *  @b TYPE:
 *  * 'b': unsigned 8-bit int (byte)
 *  * 'B': signed 8-bit int (char)
 *  * 'w': unsigned 16-bit int
 *  * 'W': signed 16-bit int
 *  * 'd': unsigned 32-bit int
 *  * 'D': signed 32-bit int
 *  * 'q': unsigned 64-bit int
 *  * 'Q': signed 64-bit int
 *  * 'i': unsigned int
 *  * 'I': signed int
 *  * 'l': unsigned long
 *  * 'L': signed long
 *  * 'h': unsigned short
 *  * 'H': signed short
 *  * 'z': size_t
 *  * 'Z': ptrdiff_t
 *  * 'p': uintptr_t
 *  * 'P': intptr_t
 *  * 'c': Unicode codepoint (32-bit int between 0-0x10FFFF minus 0xD800-0xDFFF)
 *  * 's': UTF8 encoded string with only printable characters
 *
 *  @b ALIGN:
 *  * '<' align left
 *  * '>' align right
 *
 *  @b MOD:
 *  * 'e': escapes strings C-style
 *  * 'E': custom string escaping - reads zlx_clconv_func_t and void * context
 *         from va args
 *  * 'y': radix 2 (binary) and use '0b' prefix
 *  * 'Y': radix 2 (binary)
 *  * 'o': radix 8 (octal) and use '0o' prefix
 *  * 'O': radix 8 (octal)
 *  * 'n': radix 10 (decimal/normal) and use '0d' prefix
 *  * 'N': radix 10
 *  * 'x': radix 16 (hexadecimal) and use '0x' prefix
 *  * 'X': radix 16 (hexadecimal)
 */
ZLX_API zlx_fmt_status_t ZLX_CALL zlx_vwfmt
(
    zlx_writer_func_t writer,
    void * writer_context,
    zlx_unicode_text_width_measure_func_t width_func,
    void * width_context,
    char const * ZLX_RESTRICT fmt,
    va_list va
);

/* zlx_vfmt *****************************************************************/
ZLX_API zlx_fmt_status_t ZLX_CALL zlx_vfmt
(
    zlx_writer_func_t writer,
    void * writer_context,
    char const * ZLX_RESTRICT fmt,
    va_list va
);

/* zlx_fmt ******************************************************************/
/**
 *  Writes formatted UTF-8 text.
 *  See zlx_vwfmt().
 */
ZLX_API zlx_fmt_status_t ZLX_CALL zlx_wfmt
(
    zlx_writer_func_t writer,
    void * writer_context,
    zlx_unicode_text_width_measure_func_t width_func,
    void * width_context,
    char const * ZLX_RESTRICT fmt,
    ...
);

/* zlx_fmt ******************************************************************/
ZLX_API zlx_fmt_status_t ZLX_CALL zlx_fmt
(
    zlx_writer_func_t writer,
    void * writer_context,
    char const * ZLX_RESTRICT fmt,
    ...
);

/* zlx_vfmt *****************************************************************/
ZLX_API zlx_fmt_sizerr_t ZLX_CALL zlx_vsfmt
(
    uint8_t * ZLX_RESTRICT out,
    size_t out_size,
    char const * ZLX_RESTRICT fmt,
    va_list va
);

/* zlx_sfmt *****************************************************************/
ZLX_API zlx_fmt_sizerr_t ZLX_CALL zlx_sfmt
(
    uint8_t * ZLX_RESTRICT out,
    size_t out_size,
    char const * ZLX_RESTRICT fmt,
    ...
);

ZLX_C_DECL_END

/** @} */

#endif

