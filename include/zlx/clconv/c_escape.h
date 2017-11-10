#ifndef _ZLX_CLCONV_C_ESCAPE_H
#define _ZLX_CLCONV_C_ESCAPE_H

/** @addtogroup clconv
 *  @{ */

#include "interface.h"

ZLX_C_DECL_BEGIN

/* zlx_clconv_c_escape_t ****************************************************/
/**
 *  Context structure for zlx_clconv_c_escape().
 */
typedef struct zlx_clconv_c_escape_s zlx_clconv_c_escape_t;
struct zlx_clconv_c_escape_s
{
    int crt_byte; /**< internal field. */
};

/* zlx_clconv_c_escape_force_hex ********************************************/
/**
 *  C-Escapes a byte array forcing 2 hex digit '\\x' escapes even when
 *  generating bad string literals.
 *  @param [in] in
 *      input data
 *  @param [in] in_len
 *      length of input data
 *  @param [out] in_used_len
 *      will be filled in with length of input data processed
 *  @param [in] out
 *      output buffer
 *  @param [in] out_len
 *      size of output buffer
 *  @param [out] out_used_len
 *      will be filled in with length of output produced
 *  @param ctx
 *      NULL
 *  @retval ZLX_CLCONV_OK
 *      all input got processed and output data produced
 *  @retval ZLX_CLCONV_FULL
 *      output buffer cannot hold another converted item
 *  @warning
 *      This does not produce strings literals conforming to the C standard;
 *      AB 34 35 will be converted to "\\xAB45" which according to C is a 1 char
 *      string (or invalid literal when char has under 16 bits).
 *  @note
 *      Python understands literals produced by this function (\\x is always
 *      followed by exactly two hex digits).
 *  @note
 *      For obtaining guaranteed valid C string literals one can use the
 *      function zlx_clconv_c_escape().
 */
ZLX_API zlx_clconv_status_t ZLX_CALL zlx_clconv_c_escape_force_hex
(
    uint8_t const * ZLX_RESTRICT in,
    size_t in_len,
    uint8_t * ZLX_RESTRICT out,
    size_t out_len,
    size_t * ZLX_RESTRICT in_used_len,
    size_t * ZLX_RESTRICT out_used_len,
    void * ctx
);

/* zlx_clconv_c_escape_init *************************************************/
/**
 *  Inits the state of the C-escape converter.
 */
ZLX_API void ZLX_CALL zlx_clconv_c_escape_init
(
    zlx_clconv_c_escape_t * ctx
);

/* zlx_clconv_c_escape ******************************************************/
/**
 *  C-escapes a string.
 *  @param in [in]
 *      input buffer, or NULL when finishing the conversion
 *  @param in_len [in]
 *      input length
 *  @param out [out]
 *      output buffer
 *  @param out_len [in]
 *      output buffer available size
 *  @param in_used_len [out]
 *      size of input successfully processed
 *  @param out_used_len [out]
 *      size of output successfully emitted
 *  @param ctx [in|out]
 *      an initialized instance of #zlx_clconv_c_escape_t
 *  @retval ZLX_CLCONV_OK
 *  @retval ZLX_CLCONV_OK
 *      all input got processed and output data produced
 *  @retval ZLX_CLCONV_FULL
 *      output buffer cannot hold another converted item
 */
ZLX_API zlx_clconv_status_t ZLX_CALL zlx_clconv_c_escape
(
    uint8_t const * ZLX_RESTRICT in,
    size_t in_len,
    uint8_t * ZLX_RESTRICT out,
    size_t out_len,
    size_t * ZLX_RESTRICT in_used_len,
    size_t * ZLX_RESTRICT out_used_len,
    void * ctx
);

ZLX_C_DECL_END
/** @} */

#endif /* _ZLX_CLCONV_C_ESCAPE_H */
