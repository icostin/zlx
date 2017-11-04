#ifndef _ZLX_CLCONV_HEX_H
#define _ZLX_CLCONV_HEX_H

/** @addtogroup clconv
 *  @{ */

#include "interface.h"

ZLX_C_DECL_BEGIN

/* zlx_clconv_bin_to_hex ****************************************************/
/**
 *  Bin to hex (all output on one line without any whitespace).
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
 *      this should be NULL
 */
ZLX_API zlx_clconv_status_t ZLX_CALL zlx_clconv_bin_to_hex
(
    uint8_t const * ZLX_RESTRICT in,
    size_t in_len,
    uint8_t * ZLX_RESTRICT out,
    size_t out_len,
    size_t * ZLX_RESTRICT in_used_len,
    size_t * ZLX_RESTRICT out_used_len,
    void * ctx
);

/* zlx_clconv_hex_to_bin ****************************************************/
/**
 *  Hex to bin converter.
 *  @param [in] in input data
 *  @param [in] in_len length of input data
 *  @param [out] in_used_len will be filled in with length of input data
 *              processed
 *  @param [in] out output buffer
 *  @param [in] out_len size of output buffer
 *  @param [out] out_used_len   will be filled in with length of output produced
 *  @param [in] ctx this can be NULL for strict conversion or it should
 *      point to a NUL-terminated byte string containing bytes to skip
 */
ZLX_API zlx_clconv_status_t ZLX_CALL zlx_clconv_hex_to_bin
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

#endif /* _ZLX_CLCONV_HEX_H */
