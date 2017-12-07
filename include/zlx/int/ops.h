#ifndef _ZLX_INT_OPS_H
#define _ZLX_INT_OPS_H

/** @addtogroup int Integer
 *
 *  @{
 */

#include "../lib.h"
#include "../assert.h"
#include "type.h"

ZLX_C_DECL_BEGIN

/* zlx_u8_log2_ceil *********************************************************/
/**
 *  Computes the smallest power of 2 that is greater or equal to the given
 *  8-bit unsigned int.
 */
ZLX_INLINE uint8_t zlx_u8_log2_ceil (uint8_t x)
{
    return x <= 0x10
        ? (x <= 4
           ? (((2 << 8) | (2 << 6) | (1 << 4)) >> (x + x)) & 3
           : 3 + (x > 8))
        : (x <= 0x40 ? 5 + (x > 0x20) : 7 + (x > 0x80));
}

/* zlx_u16_log2_ceil ********************************************************/
/**
 *  Computes the smallest power of 2 that is greater or equal to the given
 *  16-bit unsigned int.
 */
ZLX_INLINE uint8_t zlx_u16_log2_ceil (uint16_t x)
{
    return (uint8_t) (x == (uint8_t) x
        ? zlx_u8_log2_ceil((uint8_t) x)
        : 8 + zlx_u8_log2_ceil((uint8_t) ((x >> 8) | !!(uint8_t) x)));
}

/* zlx_u32_log2_ceil ********************************************************/
/**
 *  Computes the smallest power of 2 that is greater or equal to the given
 *  32-bit unsigned int.
 */
ZLX_INLINE uint8_t zlx_u32_log2_ceil (uint32_t x)
{
    return (uint8_t) (x == (uint16_t) x
        ? zlx_u16_log2_ceil((uint16_t) x)
        : 16 + zlx_u16_log2_ceil((uint16_t) ((x >> 16) | !!(uint16_t) x)));
}

/* zlx_u64_log2_ceil ********************************************************/
/**
 *  Computes the smallest power of 2 that is greater or equal to the given
 *  64-bit unsigned int.
 */
ZLX_INLINE uint8_t zlx_u64_log2_ceil (uint64_t x)
{
    return (uint8_t) (x == (uint32_t) x
        ? zlx_u32_log2_ceil((uint32_t) x)
        : 32 + zlx_u32_log2_ceil((uint32_t) ((x >> 32) | !!(uint32_t) x)));
}

/* zlx_size_log2_ceil *******************************************************/
/**
 *  Computes the smallest power of 2 that is greater or equal to the given
 *  size_t unsigned int.
 */
ZLX_INLINE uint8_t zlx_size_log2_ceil (size_t x)
{
#if ZLX_BITS == 64
    return zlx_u64_log2_ceil((uint64_t) x);
#else
    return zlx_u32_log2_ceil((uint32_t) x);
#endif
}

/* zlx_uptr_log2_ceil */
/**
 *  Computes the smallest power of 2 that is greater or equal to the given
 *  pointer-sized unsigned int.
 */
ZLX_INLINE uint8_t zlx_uptr_log2_ceil (uintptr_t x)
{
#if ZLX_BITS == 64
    return zlx_u64_log2_ceil((uint64_t) x);
#else
    return zlx_u32_log2_ceil((uint32_t) x);
#endif
}

/* zlx_u8_log2_floor ********************************************************/
/**
 *  Computes the smallest power of 2 whose increment is greater than the
 *  given number.
 */
ZLX_INLINE uint8_t zlx_u8_log2_floor (uint8_t x)
{
    /* 0 0 1 1 2 2 2 2 3 3 3 3 3 3 3 3 => 0xFFFFAA50 */
    if (x < 0x10) return (0xFFFFAA50 >> (x + x)) & 3;
    x >>= 4;
    return (uint8_t) (4 | ((0xFFFFAA50 >> (x + x)) & 3));
}

/* zlx_u16_log2_floor *******************************************************/
/**
 *  Computes the smallest power of 2 whose increment is greater than the
 *  given number.
 */
ZLX_INLINE uint8_t zlx_u16_log2_floor (uint16_t x)
{
    return x == (uint8_t) x
        ? zlx_u8_log2_floor((uint8_t) x)
        : 8 | zlx_u8_log2_floor((uint8_t) (x >> 8));
}

/* zlx_u32_log2_floor *******************************************************/
/**
 *  Computes the smallest power of 2 whose increment is greater than the
 *  given number.
 */
ZLX_INLINE uint8_t zlx_u32_log2_floor (uint32_t x)
{
    return x == (uint16_t) x
        ? zlx_u16_log2_floor((uint16_t) x)
        : 16 | zlx_u16_log2_floor((uint16_t) (x >> 16));
}

/* zlx_u64_log2_floor *******************************************************/
/**
 *  Computes the smallest power of 2 whose increment is greater than the
 *  given number.
 */
ZLX_INLINE uint8_t zlx_u64_log2_floor (uint64_t x)
{
    return x == (uint32_t) x
        ? zlx_u32_log2_floor((uint32_t) x)
        : 32 | zlx_u32_log2_floor((uint32_t) (x >> 32));
}

/* zlx_seqbswap16 ***********************************************************/
/**
 *  Sequential 16-bit byte-swap.
 *  This function computes the byte-swap value by shifting each byte from the
 *  input value to its final bit location.
 */
ZLX_INLINE uint16_t zlx_seqbswap16 (uint16_t v)
{
    return (uint16_t) (((uint16_t) ((uint8_t) v) << 8) | (v >> 8));
}

/* zlx_seqbswap32 ***********************************************************/
/**
 *  Sequential 32-bit byte-swap.
 *  This function computes the byte-swap value by shifting each byte from the
 *  input value to its final bit location.
 */
ZLX_INLINE uint32_t zlx_seqbswap32 (uint32_t v)
{
    return ((uint32_t) ((uint8_t) v) << 24) | ((v & 0xFF00) << 8)
        | ((v >> 8) & 0xFF00) | (v >> 24);
}

/* zlx_seqbswap64 ***********************************************************/
/**
 *  Sequential 32-bit byte-swap.
 *  This function computes the byte-swap value by shifting each byte from the
 *  input value to its final bit location.
 */
ZLX_INLINE uint64_t zlx_seqbswap64 (uint64_t v)
{
    return ((uint64_t) ((uint8_t) v) << 56) | ((v & 0xFF00) << 40)
        | ((v & 0xFF0000) << 24) | ((v & 0xFF000000) << 8)
        | ((v >> 8) & 0xFF000000) | ((v >> 24) & 0xFF0000)
        | ((v >> 40) & 0xFF00) | (v >> 56);
}

#if __GNUC__
#define ZLX_BSWAP16(v) (__builtin_bswap16(v))
#define ZLX_BSWAP32(v) (__builtin_bswap32(v))
#define ZLX_BSWAP64(v) (__builtin_bswap64(v))
#else
/*  ZLX_BSWAP16  */
/**
 *  16-bit byte-swap.
 *  This macro expands to the fastest available function for byte-swapping.
 */
#define ZLX_BSWAP16(v) (zlx_seqbswap16((v)))

/*  ZLX_BSWAP32  */
/**
 *  32-bit byte-swap.
 *  This macro expands to the fastest available function for byte-swapping.
 */
#define ZLX_BSWAP32(v) (zlx_seqbswap32((v)))

/*  ZLX_BSWAP64  */
/**
 *  64-bit byte-swap.
 *  This macro expands to the fastest available function for byte-swapping.
 */
#define ZLX_BSWAP64(v) (zlx_seqbswap64((v)))
#endif

/* macros for reading bytes */

/** @def ZLX_READ_U8(p)
 *  Reads a uint8_t from the given pointer.
 */
#define ZLX_READ_U8(p) (*(uint8_t const *) (p))

/** @def ZLX_AREAD_U8(p)
 *  Reads a uint8_t from the given pointer.
 */
#define ZLX_AREAD_U8(p) (ZLX_READ_U8(p))

/** @def ZLX_UREAD_U8(p)
 *  Reads a uint8_t from the given pointer.
 */
#define ZLX_UREAD_U8(p) (ZLX_READ_U8(p))

/** @def ZLX_AREAD_U8LE(p)
 *  Reads a uint8_t from the given pointer (byte-aligned, little endian). */
#define ZLX_AREAD_U8LE(p) (ZLX_READ_U8(p))
/** @def ZLX_UREAD_U8LE(p)
 *  Reads a uint8_t from the given pointer (unaligned, little endian). */
#define ZLX_UREAD_U8LE(p) (ZLX_READ_U8(p))
/** @def ZLX_AREAD_U8BE(p)
 *  Reads a uint8_t from the given pointer (byte-aligned, big endian). */
#define ZLX_AREAD_U8BE(p) (ZLX_READ_U8(p))
/** @def ZLX_UREAD_U8BE(p)
 *  Reads a uint8_t from the given pointer (unaligned, big endian). */
#define ZLX_UREAD_U8BE(p) (ZLX_READ_U8(p))
/** @def ZLX_AREAD_U8NE(p)
 *  Reads a uint8_t from the given pointer (byte-aligned, native endian). */
#define ZLX_AREAD_U8NE(p) (ZLX_READ_U8(p))
/** @def ZLX_UREAD_U8NE(p)
 *  Reads a uint8_t from the given pointer (unaligned, native endian). */
#define ZLX_UREAD_U8NE(p) (ZLX_READ_U8(p))
/** @def ZLX_AREAD_U8RE(p)
 *  Reads a uint8_t from the given pointer (byte-aligned, reverse endian). */
#define ZLX_AREAD_U8RE(p) (ZLX_READ_U8(p))
/** @def ZLX_UREAD_U8RE(p)
 *  Reads a uint8_t from the given pointer (unaligned, reverse endian). */
#define ZLX_UREAD_U8RE(p) (ZLX_READ_U8(p))

/* macros for writing bytes */
/** @def ZLX_WRITE_U8(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_WRITE_U8(p, v) (*(uint8_t *) (p) = (v))
/** @def ZLX_AWRITE_U8(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_AWRITE_U8(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_UWRITE_U8(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_UWRITE_U8(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_AWRITE_U8LE(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_AWRITE_U8LE(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_UWRITE_U8LE(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_UWRITE_U8LE(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_AWRITE_U8BE(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_AWRITE_U8BE(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_UWRITE_U8BE(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_UWRITE_U8BE(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_AWRITE_U8NE(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_AWRITE_U8NE(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_UWRITE_U8NE(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_UWRITE_U8NE(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_AWRITE_U8RE(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_AWRITE_U8RE(p, v) (ZLX_WRITE_U8(p, v))
/** @def ZLX_UWRITE_U8RE(p, v)
 *  Writes a uint8_t value at a given pointer. */
#define ZLX_UWRITE_U8RE(p, v) (ZLX_WRITE_U8(p, v))

/* macros for reading 16-bit ints */
/** Aligned read of a uint16_t with native endian */
#define ZLX_AREAD_U16NE(p) (*(uint16_t const *) (p))
#define ZLX_AREAD_U16RE(p) (ZLX_BSWAP16(ZLX_AREAD_U16NE(p)))
#if ZLX_LITTLE_ENDIAN
#define ZLX_AREAD_U16LE(p) (ZLX_AREAD_U16NE(p))
#define ZLX_AREAD_U16BE(p) (ZLX_AREAD_U16RE(p))
#else
#define ZLX_AREAD_U16LE(p) (ZLX_AREAD_U16RE(p))
#define ZLX_AREAD_U16BE(p) (ZLX_AREAD_U16NE(p))
#endif

ZLX_INLINE uint16_t zlx_seqread_u16le (void const * p)
{
    uint8_t const * b = p;
    return (uint16_t) (b[0] | (b[1] << 8));
}

ZLX_INLINE uint16_t zlx_seqread_u16be (void const * p)
{
    uint8_t const * b = p;
    return (uint16_t) (b[1] | (b[0] << 8));
}

#if ZLX_UNALIGNED_ACCESS
#define ZLX_UREAD_U16NE(p) (ZLX_AREAD_U16NE(p))
#define ZLX_UREAD_U16RE(p) (ZLX_AREAD_U16RE(p))
#define ZLX_UREAD_U16LE(p) (ZLX_AREAD_U16LE(p))
#define ZLX_UREAD_U16BE(p) (ZLX_AREAD_U16BE(p))
#else
#define ZLX_UREAD_U16LE(p) (zlx_seqread_u16le((p)))
#define ZLX_UREAD_U16BE(p) (zlx_seqread_u16be((p)))
#if ZLX_LITTLE_ENDIAN
#define ZLX_UREAD_U16NE(p) (ZLX_UREAD_U16LE(p))
#define ZLX_UREAD_U16RE(p) (ZLX_UREAD_U16BE(p))
#else /* big endian */
#define ZLX_UREAD_U16NE(p) (ZLX_UREAD_U16BE(p))
#define ZLX_UREAD_U16RE(p) (ZLX_UREAD_U16LE(p))
#endif
#endif

/* macros for writing 16-bit ints */
#define ZLX_AWRITE_U16NE(p, v) (*(uint16_t *) (p) = (v))
#define ZLX_AWRITE_U16RE(p, v) (*(uint16_t *) (p) = ZLX_BSWAP16(v))
#if ZLX_LITTLE_ENDIAN
#define ZLX_AWRITE_U16LE(p, v) (ZLX_AWRITE_U16NE((p), (v)))
#define ZLX_AWRITE_U16BE(p, v) (ZLX_AWRITE_U16RE((p), (v)))
#else
#define ZLX_AWRITE_U16LE(p, v) (ZLX_AWRITE_U16RE((p), (v)))
#define ZLX_AWRITE_U16BE(p, v) (ZLX_AWRITE_U16NE((p), (v)))
#endif

ZLX_INLINE void zlx_seqwrite_u16le (void * p, uint16_t v)
{
    uint8_t * b = p;
    b[0] = (uint8_t) v;
    b[1] = (uint8_t) (v >> 8);
}

ZLX_INLINE void zlx_seqwrite_u16be (void * p, uint16_t v)
{
    uint8_t * b = p;
    b[0] = (uint8_t) (v >> 8);
    b[1] = (uint8_t) v;
}

#if ZLX_UNALIGNED_ACCESS
#define ZLX_UWRITE_U16NE(p, v) (ZLX_AWRITE_U16NE((p), (v)))
#define ZLX_UWRITE_U16RE(p, v) (ZLX_AWRITE_U16RE((p), (v)))
#define ZLX_UWRITE_U16LE(p, v) (ZLX_AWRITE_U16LE((p), (v)))
#define ZLX_UWRITE_U16BE(p, v) (ZLX_AWRITE_U16BE((p), (v)))
#else
#define ZLX_UWRITE_U16LE(p, v) (zlx_seqwrite_u16le((p), (v)))
#define ZLX_UWRITE_U16BE(p, v) (zlx_seqwrite_u16be((p), (v)))
#if ZLX_LITTLE_ENDIAN
#define ZLX_UWRITE_U16NE(p, v) (ZLX_UWRITE_U16LE(((p), (v))))
#define ZLX_UWRITE_U16RE(p, v) (ZLX_UWRITE_U16BE(((p), (v))))
#else /* big endian */
#define ZLX_UWRITE_U16NE(p, v) (ZLX_UWRITE_U16BE(((p), (v))))
#define ZLX_UWRITE_U16RE(p, v) (ZLX_UWRITE_U16LE(((p), (v))))
#endif
#endif

/* macros for reading 32-bit ints */
#define ZLX_AREAD_U32NE(p) (*(uint32_t const *) (p))
#define ZLX_AREAD_U32RE(p) (ZLX_BSWAP32(ZLX_AREAD_U32NE(p)))
#if ZLX_LITTLE_ENDIAN
#define ZLX_AREAD_U32LE(p) (ZLX_AREAD_U32NE(p))
#define ZLX_AREAD_U32BE(p) (ZLX_AREAD_U32RE(p))
#else
#define ZLX_AREAD_U32LE(p) (ZLX_AREAD_U32RE(p))
#define ZLX_AREAD_U32BE(p) (ZLX_AREAD_U32NE(p))
#endif

ZLX_INLINE uint32_t zlx_seqread_u32le (void const * p)
{
    uint8_t const * b = p;
    return b[0] | ((uint32_t) b[1] << 8) |
        ((uint32_t) b[2] << 16) | ((uint32_t) b[3] << 24);
}

ZLX_INLINE uint32_t zlx_seqread_u32be (void const * p)
{
    uint8_t const * b = p;
    return b[3] | ((uint32_t) b[2] << 8) |
        ((uint32_t) b[1] << 16) | ((uint32_t) b[0] << 24);
}

#if ZLX_UNALIGNED_ACCESS
#define ZLX_UREAD_U32NE(p) (ZLX_AREAD_U32NE(p))
#define ZLX_UREAD_U32RE(p) (ZLX_AREAD_U32RE(p))
#define ZLX_UREAD_U32LE(p) (ZLX_AREAD_U32LE(p))
#define ZLX_UREAD_U32BE(p) (ZLX_AREAD_U32BE(p))
#else
#define ZLX_UREAD_U32LE(p) (zlx_seqread_u32le((p)))
#define ZLX_UREAD_U32BE(p) (zlx_seqread_u32be((p)))
#if ZLX_LITTLE_ENDIAN
#define ZLX_UREAD_U32NE(p) (ZLX_UREAD_U32LE(p))
#define ZLX_UREAD_U32RE(p) (ZLX_UREAD_U32BE(p))
#else /* big endian */
#define ZLX_UREAD_U32NE(p) (ZLX_UREAD_U32BE(p))
#define ZLX_UREAD_U32RE(p) (ZLX_UREAD_U32LE(p))
#endif
#endif

/* macros for writing 32-bit ints */
#define ZLX_AWRITE_U32NE(p, v) (*(uint32_t *) (p) = (v))
#define ZLX_AWRITE_U32RE(p, v) (*(uint32_t *) (p) = ZLX_BSWAP32(v))
#if ZLX_LITTLE_ENDIAN
#define ZLX_AWRITE_U32LE(p, v) (ZLX_AWRITE_U32NE((p), (v)))
#define ZLX_AWRITE_U32BE(p, v) (ZLX_AWRITE_U32RE((p), (v)))
#else
#define ZLX_AWRITE_U32LE(p, v) (ZLX_AWRITE_U32RE((p), (v)))
#define ZLX_AWRITE_U32BE(p, v) (ZLX_AWRITE_U32NE((p), (v)))
#endif

ZLX_INLINE void zlx_seqwrite_u32le (void * p, uint32_t v)
{
    uint8_t * b = p;
    b[0] = (uint8_t) v;
    b[1] = (uint8_t) (v >> 8);
    b[2] = (uint8_t) (v >> 16);
    b[3] = (uint8_t) (v >> 24);
}

ZLX_INLINE void zlx_seqwrite_u32be (void * p, uint32_t v)
{
    uint8_t * b = p;
    b[0] = (uint8_t) (v >> 24);
    b[1] = (uint8_t) (v >> 16);
    b[2] = (uint8_t) (v >> 8);
    b[3] = (uint8_t) v;
}

#if ZLX_UNALIGNED_ACCESS
#define ZLX_UWRITE_U32NE(p, v) (ZLX_AWRITE_U32NE((p), (v)))
#define ZLX_UWRITE_U32RE(p, v) (ZLX_AWRITE_U32RE((p), (v)))
#define ZLX_UWRITE_U32LE(p, v) (ZLX_AWRITE_U32LE((p), (v)))
#define ZLX_UWRITE_U32BE(p, v) (ZLX_AWRITE_U32BE((p), (v)))
#else
#define ZLX_UWRITE_U32LE(p, v) (zlx_seqwrite_u32le((p), (v)))
#define ZLX_UWRITE_U32BE(p, v) (zlx_seqwrite_u32be((p), (v)))
#if ZLX_LITTLE_ENDIAN
#define ZLX_UWRITE_U32NE(p, v) (ZLX_UWRITE_U32LE(((p), (v))))
#define ZLX_UWRITE_U32RE(p, v) (ZLX_UWRITE_U32BE(((p), (v))))
#else /* big endian */
#define ZLX_UWRITE_U32NE(p, v) (ZLX_UWRITE_U32BE(((p), (v))))
#define ZLX_UWRITE_U32RE(p, v) (ZLX_UWRITE_U32LE(((p), (v))))
#endif
#endif

/* macros for reading 64-bit ints */
#define ZLX_AREAD_U64NE(p) (*(uint64_t const *) (p))
#define ZLX_AREAD_U64RE(p) (ZLX_BSWAP64(ZLX_AREAD_U64NE(p)))
#if ZLX_LITTLE_ENDIAN
#define ZLX_AREAD_U64LE(p) (ZLX_AREAD_U64NE(p))
#define ZLX_AREAD_U64BE(p) (ZLX_AREAD_U64RE(p))
#else
#define ZLX_AREAD_U64LE(p) (ZLX_AREAD_U64RE(p))
#define ZLX_AREAD_U64BE(p) (ZLX_AREAD_U64NE(p))
#endif

ZLX_INLINE uint64_t zlx_seqread_u64le (void const * p)
{
    uint8_t const * b = p;
    return b[0] | ((uint64_t) b[1] << 8) |
        ((uint64_t) b[2] << 16) | ((uint64_t) b[3] << 24) |
        ((uint64_t) b[4] << 32) | ((uint64_t) b[5] << 40) |
        ((uint64_t) b[6] << 48) | ((uint64_t) b[7] << 56);
}

ZLX_INLINE uint64_t zlx_seqread_u64be (void const * p)
{
    uint8_t const * b = p;
    return b[7] | ((uint64_t) b[6] << 8) |
        ((uint64_t) b[5] << 16) | ((uint64_t) b[4] << 24) |
        ((uint64_t) b[3] << 32) | ((uint64_t) b[2] << 40) |
        ((uint64_t) b[1] << 48) | ((uint64_t) b[0] << 56);
}

#if ZLX_UNALIGNED_ACCESS
#define ZLX_UREAD_U64NE(p) (ZLX_AREAD_U64NE(p))
#define ZLX_UREAD_U64RE(p) (ZLX_AREAD_U64RE(p))
#define ZLX_UREAD_U64LE(p) (ZLX_AREAD_U64LE(p))
#define ZLX_UREAD_U64BE(p) (ZLX_AREAD_U64BE(p))
#else
#define ZLX_UREAD_U64LE(p) (zlx_seqread_u64le((p)))
#define ZLX_UREAD_U64BE(p) (zlx_seqread_u64be((p)))
#if ZLX_LITTLE_ENDIAN
#define ZLX_UREAD_U64NE(p) (ZLX_UREAD_U64LE(p))
#define ZLX_UREAD_U64RE(p) (ZLX_UREAD_U64BE(p))
#else /* big endian */
#define ZLX_UREAD_U64NE(p) (ZLX_UREAD_U64BE(p))
#define ZLX_UREAD_U64RE(p) (ZLX_UREAD_U64LE(p))
#endif
#endif

/* macros for writing 64-bit ints */
#define ZLX_AWRITE_U64NE(p, v) (*(uint64_t *) (p) = (v))
#define ZLX_AWRITE_U64RE(p, v) (*(uint64_t *) (p) = ZLX_BSWAP64(v))
#if ZLX_LITTLE_ENDIAN
#define ZLX_AWRITE_U64LE(p, v) (ZLX_AWRITE_U64NE((p), (v)))
#define ZLX_AWRITE_U64BE(p, v) (ZLX_AWRITE_U64RE((p), (v)))
#else
#define ZLX_AWRITE_U64LE(p, v) (ZLX_AWRITE_U64RE((p), (v)))
#define ZLX_AWRITE_U64BE(p, v) (ZLX_AWRITE_U64NE((p), (v)))
#endif

ZLX_INLINE void zlx_seqwrite_u64le (void * p, uint64_t v)
{
    uint8_t * b = p;
    b[0] = (uint8_t) v;
    b[1] = (uint8_t) (v >> 8);
    b[2] = (uint8_t) (v >> 16);
    b[3] = (uint8_t) (v >> 24);
    b[4] = (uint8_t) (v >> 32);
    b[5] = (uint8_t) (v >> 40);
    b[6] = (uint8_t) (v >> 48);
    b[7] = (uint8_t) (v >> 56);
}

ZLX_INLINE void zlx_seqwrite_u64be (void * p, uint64_t v)
{
    uint8_t * b = p;
    b[0] = (uint8_t) (v >> 56);
    b[1] = (uint8_t) (v >> 48);
    b[2] = (uint8_t) (v >> 40);
    b[3] = (uint8_t) (v >> 32);
    b[4] = (uint8_t) (v >> 24);
    b[5] = (uint8_t) (v >> 16);
    b[6] = (uint8_t) (v >> 8);
    b[7] = (uint8_t) v;
}

#if ZLX_UNALIGNED_ACCESS
# define ZLX_UWRITE_U64NE(p, v) (ZLX_AWRITE_U64NE((p), (v)))
# define ZLX_UWRITE_U64RE(p, v) (ZLX_AWRITE_U64RE((p), (v)))
# define ZLX_UWRITE_U64LE(p, v) (ZLX_AWRITE_U64LE((p), (v)))
# define ZLX_UWRITE_U64BE(p, v) (ZLX_AWRITE_U64BE((p), (v)))
#else
# define ZLX_UWRITE_U64LE(p, v) (zlx_seqwrite_u64le((p), (v)))
# define ZLX_UWRITE_U64BE(p, v) (zlx_seqwrite_u64be((p), (v)))
# if ZLX_LITTLE_ENDIAN
#  define ZLX_UWRITE_U64NE(p, v) (ZLX_UWRITE_U64LE(((p), (v))))
#  define ZLX_UWRITE_U64RE(p, v) (ZLX_UWRITE_U64BE(((p), (v))))
# else /* big endian */
#  define ZLX_UWRITE_U64NE(p, v) (ZLX_UWRITE_U64BE(((p), (v))))
#  define ZLX_UWRITE_U64RE(p, v) (ZLX_UWRITE_U64LE(((p), (v))))
# endif
#endif


/* zlx_u64_div_mod **********************************************************/
/**
 *  Portable 64-bit unsigned int division.
 *  @returns quotient
 */
ZLX_API uint64_t ZLX_CALL zlx_u64_div_mod
(
    uint64_t divisor,
    uint64_t dividend,
    uint64_t * ZLX_RESTRICT remainder
);

#define _ZLX_INT_SHRINK(src_type, dest_type) \
    _ZLX_INT_CONV(src_type, dest_type, \
                  (zlx_##src_type##_t) dest_value == src_value)
#define _ZLX_INT_ENLARGE(src_type, dest_type) \
    _ZLX_INT_CONV(src_type, dest_type, 1)
#define _ZLX_INT_MIX_CONV(src_type, dest_type) \
    _ZLX_INT_CONV(src_type, dest_type, \
        (src_value >> (_ZLX_MIN_SIZEOF(src_type, dest_type) * 8 - 1)) == 0)

#define _ZLX_INT_CONV(src_type, dest_type, check) \
    ZLX_INLINE zlx_##dest_type##_t zlx_##dest_type##_from_##src_type \
        (zlx_##src_type##_t src_value) { \
        zlx_##dest_type##_t dest_value = (zlx_##dest_type##_t) src_value; \
        ZLX_ASSERT((check)); \
        return dest_value; \
    } \
    ZLX_INLINE zlx_##dest_type##_t zlx_##src_type##_to_##dest_type \
        (zlx_##src_type##_t src_value) { \
        return zlx_##dest_type##_from_##src_type(src_value); \
    } \
    ZLX_INLINE zlx_##dest_type##_t zlx_cast_##dest_type##_from_##src_type \
        (zlx_##src_type##_t src_value) { \
        return  (zlx_##dest_type##_t) src_value; \
    } \
    ZLX_INLINE zlx_##dest_type##_t zlx_cast_##src_type##_to_##dest_type \
        (zlx_##src_type##_t src_value) { \
        return (zlx_##dest_type##_t) src_value; \
    } \
    typedef zlx_##dest_type##_t zlx_##dest_type##_from_##src_type##_ret_t

#define _ZLX_MIN_SIZEOF(src_type, dest_type) \
    ((sizeof(zlx_##src_type##_t) <= sizeof(zlx_##dest_type##_t)) \
       * sizeof(zlx_##src_type##_t) \
     + (sizeof(zlx_##src_type##_t) > sizeof(zlx_##dest_type##_t)) \
       * sizeof(zlx_##dest_type##_t))


#if ZLXOPT_DOXYGEN
/** Lossless conversion from 16-bit unsigned int to 8-bit unsigned int.
 *  @param value [in] value
 *  @return same @a value
 *  @note The function asserts that the value can fit the output type. */
uint8_t zlx_u16_to_u8 (uint16_t value);

/** Lossless conversion from 16-bit unsigned int to 8-bit unsigned int.
 *  @param value [in] value
 *  @return same @a value
 *  @note The function asserts that the value can fit the output type. */
uint8_t zlx_u8_from_u16 (uint16_t value);

/** Truncation from 16-bit unsigned int to 8-bit unsigned int.
 *  @param value [in] value
 *  @return same @a value */
uint8_t zlx_cast_u16_to_u8 (uint16_t value);

/** Truncation from 16-bit unsigned int to 8-bit unsigned int.
 *  @param value [in] value
 *  @return same @a value */
uint8_t zlx_cast_u8_from_u16 (uint16_t value);
#endif

_ZLX_INT_ENLARGE    (u8     , u8);
_ZLX_INT_SHRINK     (u16    , u8);
_ZLX_INT_SHRINK     (u32    , u8);
_ZLX_INT_SHRINK     (u64    , u8);
_ZLX_INT_SHRINK     (uptr   , u8);
_ZLX_INT_SHRINK     (usize  , u8);
_ZLX_INT_SHRINK     ( size  , u8);
_ZLX_INT_MIX_CONV   (s8     , u8);
_ZLX_INT_MIX_CONV   (s16    , u8);
_ZLX_INT_MIX_CONV   (s32    , u8);
_ZLX_INT_MIX_CONV   (s64    , u8);
_ZLX_INT_MIX_CONV   (ssize  , u8);
_ZLX_INT_MIX_CONV   (ptrdiff, u8);
_ZLX_INT_MIX_CONV   (sptr   , u8);

_ZLX_INT_ENLARGE    (u8     , u16);
_ZLX_INT_ENLARGE    (u16    , u16);
_ZLX_INT_SHRINK     (u32    , u16);
_ZLX_INT_SHRINK     (u64    , u16);
_ZLX_INT_SHRINK     (uptr   , u16);
_ZLX_INT_SHRINK     (usize  , u16);
_ZLX_INT_SHRINK     ( size  , u16);
_ZLX_INT_MIX_CONV   (s8     , u16);
_ZLX_INT_MIX_CONV   (s16    , u16);
_ZLX_INT_MIX_CONV   (s32    , u16);
_ZLX_INT_MIX_CONV   (s64    , u16);
_ZLX_INT_MIX_CONV   (ssize  , u16);
_ZLX_INT_MIX_CONV   (ptrdiff, u16);
_ZLX_INT_MIX_CONV   (sptr   , u16);

_ZLX_INT_ENLARGE    (u8     , u32);
_ZLX_INT_ENLARGE    (u16    , u32);
_ZLX_INT_ENLARGE    (u32    , u32);
_ZLX_INT_SHRINK     (u64    , u32);
#if ZLX_BITS == 32
_ZLX_INT_ENLARGE    (uptr   , u32);
_ZLX_INT_ENLARGE    (usize  , u32);
_ZLX_INT_ENLARGE    ( size  , u32);
#else
_ZLX_INT_SHRINK     (uptr   , u32);
_ZLX_INT_SHRINK     (usize  , u32);
_ZLX_INT_SHRINK     ( size  , u32);
#endif
_ZLX_INT_MIX_CONV   (s8     , u32);
_ZLX_INT_MIX_CONV   (s16    , u32);
_ZLX_INT_MIX_CONV   (s32    , u32);
_ZLX_INT_MIX_CONV   (s64    , u32);
_ZLX_INT_MIX_CONV   (ssize  , u32);
_ZLX_INT_MIX_CONV   (ptrdiff, u32);
_ZLX_INT_MIX_CONV   (sptr   , u32);

_ZLX_INT_ENLARGE    (u8     , u64);
_ZLX_INT_ENLARGE    (u16    , u64);
_ZLX_INT_ENLARGE    (u32    , u64);
_ZLX_INT_ENLARGE    (u64    , u64);
_ZLX_INT_ENLARGE    (uptr   , u64);
_ZLX_INT_ENLARGE    (usize  , u64);
_ZLX_INT_ENLARGE    ( size  , u64);
_ZLX_INT_MIX_CONV   (s8     , u64);
_ZLX_INT_MIX_CONV   (s16    , u64);
_ZLX_INT_MIX_CONV   (s32    , u64);
_ZLX_INT_MIX_CONV   (s64    , u64);
_ZLX_INT_MIX_CONV   (ssize  , u64);
_ZLX_INT_MIX_CONV   (ptrdiff, u64);
_ZLX_INT_MIX_CONV   (sptr   , u64);

_ZLX_INT_ENLARGE    (u8     , usize);
_ZLX_INT_ENLARGE    (u16    , usize);
_ZLX_INT_ENLARGE    (u32    , usize);
#if ZLX_BITS == 32
_ZLX_INT_SHRINK     (u64    , usize);
#else
_ZLX_INT_ENLARGE    (u64    , usize);
#endif
_ZLX_INT_ENLARGE    (uptr   , usize);
_ZLX_INT_ENLARGE    (usize  , usize);
_ZLX_INT_ENLARGE    ( size  , usize);
_ZLX_INT_MIX_CONV   (s8     , usize);
_ZLX_INT_MIX_CONV   (s16    , usize);
_ZLX_INT_MIX_CONV   (s32    , usize);
_ZLX_INT_MIX_CONV   (s64    , usize);
_ZLX_INT_MIX_CONV   (ssize  , usize);
_ZLX_INT_MIX_CONV   (ptrdiff, usize);
_ZLX_INT_MIX_CONV   (sptr   , usize);

_ZLX_INT_ENLARGE    (u8     ,  size);
_ZLX_INT_ENLARGE    (u16    ,  size);
_ZLX_INT_ENLARGE    (u32    ,  size);
#if ZLX_BITS == 32
_ZLX_INT_SHRINK     (u64    ,  size);
#else
_ZLX_INT_ENLARGE    (u64    ,  size);
#endif
_ZLX_INT_ENLARGE    (uptr   ,  size);
_ZLX_INT_ENLARGE    (usize  ,  size);
_ZLX_INT_ENLARGE    ( size  ,  size);
_ZLX_INT_MIX_CONV   (s8     ,  size);
_ZLX_INT_MIX_CONV   (s16    ,  size);
_ZLX_INT_MIX_CONV   (s32    ,  size);
_ZLX_INT_MIX_CONV   (s64    ,  size);
_ZLX_INT_MIX_CONV   (ssize  ,  size);
_ZLX_INT_MIX_CONV   (ptrdiff,  size);
_ZLX_INT_MIX_CONV   (sptr   ,  size);

_ZLX_INT_ENLARGE    (u8     , uptr);
_ZLX_INT_ENLARGE    (u16    , uptr);
_ZLX_INT_ENLARGE    (u32    , uptr);
#if ZLX_BITS == 32
_ZLX_INT_SHRINK     (u64    , uptr);
#else
_ZLX_INT_ENLARGE    (u64    , uptr);
#endif
_ZLX_INT_ENLARGE    (uptr   , uptr);
_ZLX_INT_ENLARGE    (usize  , uptr);
_ZLX_INT_ENLARGE    ( size  , uptr);
_ZLX_INT_MIX_CONV   (s8     , uptr);
_ZLX_INT_MIX_CONV   (s16    , uptr);
_ZLX_INT_MIX_CONV   (s32    , uptr);
_ZLX_INT_MIX_CONV   (s64    , uptr);
_ZLX_INT_MIX_CONV   (ssize  , uptr);
_ZLX_INT_MIX_CONV   (ptrdiff, uptr);
_ZLX_INT_MIX_CONV   (sptr   , uptr);


_ZLX_INT_MIX_CONV   (u8     , s8);
_ZLX_INT_MIX_CONV   (u16    , s8);
_ZLX_INT_MIX_CONV   (u32    , s8);
_ZLX_INT_MIX_CONV   (u64    , s8);
_ZLX_INT_MIX_CONV   (uptr   , s8);
_ZLX_INT_MIX_CONV   (usize  , s8);
_ZLX_INT_MIX_CONV   ( size  , s8);
_ZLX_INT_ENLARGE    (s8     , s8);
_ZLX_INT_SHRINK     (s16    , s8);
_ZLX_INT_SHRINK     (s32    , s8);
_ZLX_INT_SHRINK     (s64    , s8);
_ZLX_INT_SHRINK     (ssize  , s8);
_ZLX_INT_SHRINK     (ptrdiff, s8);
_ZLX_INT_SHRINK     (sptr   , s8);

_ZLX_INT_MIX_CONV   (u8     , s16);
_ZLX_INT_MIX_CONV   (u16    , s16);
_ZLX_INT_MIX_CONV   (u32    , s16);
_ZLX_INT_MIX_CONV   (u64    , s16);
_ZLX_INT_MIX_CONV   (uptr   , s16);
_ZLX_INT_MIX_CONV   (usize  , s16);
_ZLX_INT_MIX_CONV   ( size  , s16);
_ZLX_INT_ENLARGE    (s8     , s16);
_ZLX_INT_ENLARGE    (s16    , s16);
_ZLX_INT_SHRINK     (s32    , s16);
_ZLX_INT_SHRINK     (s64    , s16);
_ZLX_INT_SHRINK     (ssize  , s16);
_ZLX_INT_SHRINK     (ptrdiff, s16);
_ZLX_INT_SHRINK     (sptr   , s16);

_ZLX_INT_MIX_CONV   (u8     , s32);
_ZLX_INT_MIX_CONV   (u16    , s32);
_ZLX_INT_MIX_CONV   (u32    , s32);
_ZLX_INT_MIX_CONV   (u64    , s32);
_ZLX_INT_MIX_CONV   (uptr   , s32);
_ZLX_INT_MIX_CONV   (usize  , s32);
_ZLX_INT_MIX_CONV   ( size  , s32);
_ZLX_INT_ENLARGE    (s8     , s32);
_ZLX_INT_ENLARGE    (s16    , s32);
_ZLX_INT_ENLARGE    (s32    , s32);
_ZLX_INT_SHRINK     (s64    , s32);
#if ZLX_BITS == 32
_ZLX_INT_ENLARGE    (ssize  , s32);
_ZLX_INT_ENLARGE    (ptrdiff, s32);
_ZLX_INT_ENLARGE    (sptr   , s32);
#else
_ZLX_INT_SHRINK     (ssize  , s32);
_ZLX_INT_SHRINK     (ptrdiff, s32);
_ZLX_INT_SHRINK     (sptr   , s32);
#endif

_ZLX_INT_MIX_CONV   (u8     , s64);
_ZLX_INT_MIX_CONV   (u16    , s64);
_ZLX_INT_MIX_CONV   (u32    , s64);
_ZLX_INT_MIX_CONV   (u64    , s64);
_ZLX_INT_MIX_CONV   (uptr   , s64);
_ZLX_INT_MIX_CONV   (usize  , s64);
_ZLX_INT_MIX_CONV   ( size  , s64);
_ZLX_INT_ENLARGE    (s8     , s64);
_ZLX_INT_ENLARGE    (s16    , s64);
_ZLX_INT_ENLARGE    (s32    , s64);
_ZLX_INT_ENLARGE    (s64    , s64);
_ZLX_INT_ENLARGE    (ssize  , s64);
_ZLX_INT_ENLARGE    (ptrdiff, s64);
_ZLX_INT_ENLARGE    (sptr   , s64);

_ZLX_INT_MIX_CONV   (u8     , ssize);
_ZLX_INT_MIX_CONV   (u16    , ssize);
_ZLX_INT_MIX_CONV   (u32    , ssize);
_ZLX_INT_MIX_CONV   (u64    , ssize);
_ZLX_INT_MIX_CONV   (uptr   , ssize);
_ZLX_INT_MIX_CONV   (usize  , ssize);
_ZLX_INT_MIX_CONV   ( size  , ssize);
_ZLX_INT_ENLARGE    (s8     , ssize);
_ZLX_INT_ENLARGE    (s16    , ssize);
_ZLX_INT_ENLARGE    (s32    , ssize);
#if ZLX_BITS == 32
_ZLX_INT_SHRINK     (s64    , ssize);
#else
_ZLX_INT_ENLARGE    (s64    , ssize);
#endif
_ZLX_INT_ENLARGE    (ssize  , ssize);
_ZLX_INT_ENLARGE    (ptrdiff, ssize);
_ZLX_INT_ENLARGE    (sptr   , ssize);

_ZLX_INT_MIX_CONV   (u8     , ptrdiff);
_ZLX_INT_MIX_CONV   (u16    , ptrdiff);
_ZLX_INT_MIX_CONV   (u32    , ptrdiff);
_ZLX_INT_MIX_CONV   (u64    , ptrdiff);
_ZLX_INT_MIX_CONV   (uptr   , ptrdiff);
_ZLX_INT_MIX_CONV   (usize  , ptrdiff);
_ZLX_INT_MIX_CONV   ( size  , ptrdiff);
_ZLX_INT_ENLARGE    (s8     , ptrdiff);
_ZLX_INT_ENLARGE    (s16    , ptrdiff);
_ZLX_INT_ENLARGE    (s32    , ptrdiff);
#if ZLX_BITS == 32
_ZLX_INT_SHRINK     (s64    , ptrdiff);
#else
_ZLX_INT_ENLARGE    (s64    , ptrdiff);
#endif
_ZLX_INT_ENLARGE    (ssize  , ptrdiff);
_ZLX_INT_ENLARGE    (ptrdiff, ptrdiff);
_ZLX_INT_ENLARGE    (sptr   , ptrdiff);

_ZLX_INT_MIX_CONV   (u8     , sptr);
_ZLX_INT_MIX_CONV   (u16    , sptr);
_ZLX_INT_MIX_CONV   (u32    , sptr);
_ZLX_INT_MIX_CONV   (u64    , sptr);
_ZLX_INT_MIX_CONV   (uptr   , sptr);
_ZLX_INT_MIX_CONV   (usize  , sptr);
_ZLX_INT_MIX_CONV   ( size  , sptr);
_ZLX_INT_ENLARGE    (s8     , sptr);
_ZLX_INT_ENLARGE    (s16    , sptr);
_ZLX_INT_ENLARGE    (s32    , sptr);
#if ZLX_BITS == 32
_ZLX_INT_ENLARGE    (s64    , sptr);
#else
_ZLX_INT_SHRINK     (s64    , sptr);
#endif
_ZLX_INT_ENLARGE    (ssize  , sptr);
_ZLX_INT_ENLARGE    (ptrdiff, sptr);
_ZLX_INT_ENLARGE    (sptr   , sptr);


ZLX_C_DECL_END
/** @} */

#endif /* _ZLX_INT_OPS_H */
