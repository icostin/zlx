#ifndef _ZLX_FMT_H
#define _ZLX_FMT_H

#include <stdarg.h>
#include "base.h"
#include "writer.h"

/** @defgroup fmt Formatted printing
 *  @{
 */

typedef int (ZLX_CALL * zlx_width_compute_func_t)
    (
        uint8_t const * ZLX_RESTRICT text,
        size_t size,
        void * context
    );

#define ZLX_FMT_MALFORMED 1 /**< bad format string error code */
#define ZLX_FMT_WIDTH_ERROR 2 /**< width function returned error */
#define ZLX_FMT_WRITE_ERROR 3 /**< write error */
#define ZLX_FMT_CONV_ERROR 4 /**< conversion error during escaping of some string */
#define ZLX_FMT_NO_CODE 5 /**< feature not implemented */

/* zlx_vfmt *****************************************************************/
/**
 *  Writes formatted UTF-8 text (similar to printf formatting).
 *  @retval 0 success
 *  @retval ZLX_FMT_MALFORMED bad format string
 *  @retval ZLX_FMT_WIDTH_ERROR
 *  @retval ZLX_FMT_WRITE_ERROR
 *  Formatting: "$" [[ALIGN] WIDTH] ["." PREC] ["/" GROUP_LEN] [MOD] TYPE
 *  TYPE:
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
 *  * 's': string
 *  ALIGN:
 *  * '<' align left
 *  * '>' align right
 *  MOD:
 *  * 'e': escapes strings C-style
 *  * 'y': radix 2 (binary) and use '0b' prefix
 *  * 'Y': radix 2 (binary)
 *  * 'o': radix 8 (octal) and use '0o' prefix
 *  * 'O': radix 8 (octal)
 *  * 'n': radix 10 (decimal/normal) and use '0d' prefix
 *  * 'N': radix 10
 *  * 'x': radix 16 (hexadecimal) and use '0x' prefix
 *  * 'X': radix 16 (hexadecimal)
 */
ZLX_API unsigned int ZLX_CALL zlx_vfmt
(
    zlx_writer_func_t writer_func,
    void * writer_context,
    zlx_width_compute_func_t width_func,
    void * width_context,
    char const * fmt,
    va_list va
);

/* zlx_fmt ******************************************************************/
/**
 *  Writes formatted UTF-8 text.
 *  See zlx_vfmt.
 */
ZLX_API unsigned int ZLX_CALL zlx_fmt
(
    zlx_writer_func_t writer_func,
    void * writer_context,
    zlx_width_compute_func_t width_func,
    void * width_context,
    char const * fmt,
    ...
);

/** @} */

#endif

