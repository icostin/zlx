#ifndef _ZLX_CLCONV_INTERFACE_H
#define _ZLX_CLCONV_INTERFACE_H

/** @defgroup clconv Chunked linear convertor
 *  This component defines a common interface for linear convertors of byte
 *  streams.
 *  @{ */

#define ZLX_CLCONV_OK 0
/**< entire input used (return code for #zlx_clconv_func_t) */

#define ZLX_CLCONV_MALFORMED 1
/**< malformed input; error code for #zlx_clconv_func_t */

#define ZLX_CLCONV_INCOMPLETE 2
/**< input needs more data for decoding a valid item */

#define ZLX_CLCONV_FULL 3
/**< output full (error code for #zlx_clconv_func_t) */

/* zlx_clconv_func_t ********************************************************/
/**
 *  Chunked linear converter function.
 *  Implementations must consume as much as possible from input, either
 *  until the entire input is used, either until the output buffer cannot
 *  hold an extra item.
 *  Param @a ctx can be used to store the state if the conversion requires
 *  keeping a state.
 *  After feeding the entire desired input to the converter, users should
 *  call this function again with @a in set to NULL and @a in_len set to 0
 *  to tell the converter to append any terminating sequence.
 */
typedef uint_fast8_t (ZLX_CALL * zlx_clconv_func_t)
    (
        uint8_t const * in,
        size_t in_len,
        size_t * in_used_len,
        uint8_t * out,
        size_t out_len,
        size_t * out_used_len,
        void * ctx
    );

#endif /* _ZLX_CLCONV_INTERFACE_H */

