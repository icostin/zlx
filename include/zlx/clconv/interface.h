#ifndef _ZLX_CLCONV_INTERFACE_H
#define _ZLX_CLCONV_INTERFACE_H

/** @defgroup clconv Chunked linear converter
 *  This component defines a common interface for linear convertors of byte
 *  streams.
 *  @{ */

#include "../lib.h"
#include "../int/type.h"

ZLX_C_DECL_BEGIN

/** @typedef zlx_clconv_status_t
 *  Status of a chunked linear converter */
typedef enum zlx_clconv_status
{
    ZLX_CLCONV_OK = 0, /**< entire input used */
    ZLX_CLCONV_MALFORMED, /**< malformed input */
    ZLX_CLCONV_INCOMPLETE, /**< more input needed for decoding a valid item */
    ZLX_CLCONV_FULL, /**< output full */
} zlx_clconv_status_t;

/* zlx_clconv_func_t ********************************************************/
/**
 *  Chunked linear converter function.
 *  Implementations must consume as much as possible from input, either
 *  until the entire input is used, either until the output buffer cannot
 *  hold an extra item.
 *  Param @a ctx can be used to store the state if the conversion requires
 *  keeping a state.
 *  @param in [in]
 *      input data chunk
 *  @param in_len [in]
 *      length of input data chunk
 *  @param out [out]
 *      output buffer for converted data
 *  @param out_len [in]
 *      size of output buffer available for writing
 *  @param in_used_len [out]
 *      size of consumed input data
 *  @param out_used_len [out]
 *      size of written data
 *  @note
 *      After feeding the entire desired input to the converter, users should
 *      call this function again with @a in set to NULL and @a in_len set to 0
 *      to tell the converter to append any terminating sequence.
 */
typedef zlx_clconv_status_t (ZLX_CALL * zlx_clconv_func_t)
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

#endif /* _ZLX_CLCONV_INTERFACE_H */

