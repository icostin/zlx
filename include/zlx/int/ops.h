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

/* zlx_u8_log2_floor_soft ***************************************************/
/**
 *  Computes the smallest power of 2 whose increment is greater than the
 *  given number.
 */
ZLX_INLINE uint8_t zlx_u8_log2_floor_soft (uint8_t x)
{
    /* 0 0 1 1 2 2 2 2 3 3 3 3 3 3 3 3 => 0xFFFFAA50 */
    if (x < 0x10) return (0xFFFFAA50 >> (x + x)) & 3;
    x >>= 4;
    return (uint8_t) (4 | ((0xFFFFAA50 >> (x + x)) & 3));
}

/* zlx_u16_log2_floor_soft **************************************************/
/**
 *  Computes the smallest power of 2 whose increment is greater than the
 *  given number.
 */
ZLX_INLINE uint8_t zlx_u16_log2_floor_soft (uint16_t x)
{
    return x == (uint8_t) x
        ? zlx_u8_log2_floor_soft((uint8_t) x)
        : 8 | zlx_u8_log2_floor_soft((uint8_t) (x >> 8));
}

/* zlx_u32_log2_floor *******************************************************/
/**
 *  Computes the smallest power of 2 whose increment is greater than the
 *  given number.
 */
ZLX_INLINE uint8_t zlx_u32_log2_floor_soft (uint32_t x)
{
    return x == (uint16_t) x
        ? zlx_u16_log2_floor_soft((uint16_t) x)
        : 16 | zlx_u16_log2_floor_soft((uint16_t) (x >> 16));
}

/* zlx_u64_log2_floor_soft **************************************************/
/**
 *  Computes the smallest power of 2 whose increment is greater than the
 *  given number.
 */
ZLX_INLINE uint8_t zlx_u64_log2_floor_soft (uint64_t x)
{
    return x == (uint32_t) x
        ? zlx_u32_log2_floor_soft((uint32_t) x)
        : 32 | zlx_u32_log2_floor_soft((uint32_t) (x >> 32));
}

/* zlx_u8_log2_floor ********************************************************/
ZLX_INLINE uint8_t zlx_u8_log2_floor (uint8_t x)
{
    return zlx_u8_log2_floor_soft(x);
}

/* zlx_u16_log2_floor *******************************************************/
ZLX_INLINE uint8_t zlx_u16_log2_floor (uint16_t x)
{
    return zlx_u16_log2_floor_soft(x);
}

/* zlx_u32_log2_floor *******************************************************/
ZLX_INLINE uint8_t zlx_u32_log2_floor (uint32_t x)
{
    return zlx_u32_log2_floor_soft(x);
}

/* zlx_u64_log2_floor *******************************************************/
ZLX_INLINE uint8_t zlx_u64_log2_floor (uint64_t x)
{
    return zlx_u64_log2_floor_soft(x);
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

/* zlx_u32_ffs **************************************************************/
/**
 *  Finds least significant set bit in a non-zero 32-bit int.
 *  @param n non-zero int to analyze
 */
ZLX_INLINE unsigned int zlx_u32_ffs (uint32_t n)
{
    ZLX_ASSERT(n != 0);
#if ZLX_MSC
    unsigned long b;
    _BitScanForward(&b, n);
    return (unsigned int) b;
#elif ZLX_GCC || ZLX_CLANG
    return (unsigned int) __builtin_ctz(n);
#else
    zlx_uint_t b = 0;
    while (n) ++b, n >>= 1;
    return b;
#endif
}

/* Int conversion ***********************************************************/
ZLX_STATIC_ASSERT(1 == sizeof(char)); // this is guaranteed by the language
ZLX_STATIC_ASSERT(2 == sizeof(short));
ZLX_STATIC_ASSERT(4 == sizeof(int));
ZLX_STATIC_ASSERT(ZLX_BITS / 8 == sizeof(void *));
ZLX_STATIC_ASSERT(ZLX_BITS / 8 == sizeof(intptr_t));
ZLX_STATIC_ASSERT(ZLX_BITS / 8 == sizeof(uintptr_t));
ZLX_STATIC_ASSERT(ZLX_BITS / 8 == sizeof(size_t));
ZLX_STATIC_ASSERT(ZLX_BITS / 8 == sizeof(ptrdiff_t));

#define _ZLX_INT_SHRINK(src_type, dest_type) \
    _ZLX_INT_CONV(src_type, dest_type, \
                  (zlx_##src_type##_t) dest_value == src_value)
#define _ZLX_INT_ENLARGE(src_type, dest_type) \
    _ZLX_INT_CONV(src_type, dest_type, 1)
#define _ZLX_INT_MIX_CONV(src_type, dest_type) \
    _ZLX_INT_CONV(src_type, dest_type, \
        (src_value >> (_ZLX_MIN_SIZEOF(src_type, dest_type) * 8 - 1)) == 0)

#if ZLX_BITS == 32
# define _ZLX_INT_A_TO_32 _ZLX_INT_ENLARGE
# define _ZLX_INT_64_TO_A _ZLX_INT_SHRINK
#elif ZLX_BITS == 64
# define _ZLX_INT_A_TO_32 _ZLX_INT_SHRINK
# define _ZLX_INT_64_TO_A _ZLX_INT_ENLARGE
#else
# error unsupported ZLX_BITS
#endif

#if LONG_MAX > INT32_MAX
ZLX_STATIC_ASSERT(8 == sizeof(long));
# define _ZLX_INT_L_TO_32 _ZLX_INT_SHRINK
# define _ZLX_INT_64_TO_L _ZLX_INT_ENLARGE
#else 
# define _ZLX_INT_L_TO_32 _ZLX_INT_ENLARGE
# define _ZLX_INT_64_TO_L _ZLX_INT_SHRINK
# if ZLX_BITS > 32
#  define _ZLX_INT_A_TO_L _ZLX_INT_SHRINK
# else
#  define _ZLX_INT_A_TO_L _ZLX_INT_ENLARGE
# endif
#endif



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
    ZLX_INLINE int zlx_try_##dest_type##_from_##src_type \
        (zlx_##dest_type##_t * ZLX_RESTRICT dest_ptr, \
         zlx_##src_type##_t src_value) { \
        zlx_##dest_type##_t dest_value = (zlx_##dest_type##_t) src_value; \
        if (!(check)) return 1; \
        *dest_ptr = dest_value; \
        return 0; \
    } \
    ZLX_INLINE int zlx_try_##src_type##_to_##dest_type \
        (zlx_##src_type##_t src_value, \
         zlx_##dest_type##_t * ZLX_RESTRICT dest_ptr) { \
            return zlx_try_##dest_type##_from_##src_type(dest_ptr, src_value); \
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
_ZLX_INT_SHRINK     (ulong  , u8);
_ZLX_INT_SHRINK     (uint   , u8);
_ZLX_INT_SHRINK     (ushort , u8);
_ZLX_INT_ENLARGE    (uchar  , u8);
_ZLX_INT_MIX_CONV   (s8     , u8);
_ZLX_INT_MIX_CONV   (s16    , u8);
_ZLX_INT_MIX_CONV   (s32    , u8);
_ZLX_INT_MIX_CONV   (s64    , u8);
_ZLX_INT_MIX_CONV   (sptr   , u8);
_ZLX_INT_MIX_CONV   (ssize  , u8);
_ZLX_INT_MIX_CONV   (ptrdiff, u8);
_ZLX_INT_MIX_CONV   (slong  , u8);
_ZLX_INT_MIX_CONV   ( long  , u8);
_ZLX_INT_MIX_CONV   (sint   , u8);
_ZLX_INT_MIX_CONV   ( int   , u8);
_ZLX_INT_MIX_CONV   (sshort , u8);
_ZLX_INT_MIX_CONV   ( short , u8);
_ZLX_INT_MIX_CONV   (schar  , u8);
_ZLX_INT_MIX_CONV   ( char  , u8);

_ZLX_INT_ENLARGE    (u8     , u16);
_ZLX_INT_ENLARGE    (u16    , u16);
_ZLX_INT_SHRINK     (u32    , u16);
_ZLX_INT_SHRINK     (u64    , u16);
_ZLX_INT_SHRINK     (uptr   , u16);
_ZLX_INT_SHRINK     (usize  , u16);
_ZLX_INT_SHRINK     ( size  , u16);
_ZLX_INT_SHRINK     (ulong  , u16);
_ZLX_INT_SHRINK     (uint   , u16);
_ZLX_INT_ENLARGE    (ushort , u16);
_ZLX_INT_ENLARGE    (uchar  , u16);
_ZLX_INT_MIX_CONV   (s8     , u16);
_ZLX_INT_MIX_CONV   (s16    , u16);
_ZLX_INT_MIX_CONV   (s32    , u16);
_ZLX_INT_MIX_CONV   (s64    , u16);
_ZLX_INT_MIX_CONV   (sptr   , u16);
_ZLX_INT_MIX_CONV   (ssize  , u16);
_ZLX_INT_MIX_CONV   (ptrdiff, u16);
_ZLX_INT_MIX_CONV   (slong  , u16);
_ZLX_INT_MIX_CONV   ( long  , u16);
_ZLX_INT_MIX_CONV   (sint   , u16);
_ZLX_INT_MIX_CONV   ( int   , u16);
_ZLX_INT_MIX_CONV   (sshort , u16);
_ZLX_INT_MIX_CONV   ( short , u16);
_ZLX_INT_MIX_CONV   (schar  , u16);
_ZLX_INT_MIX_CONV   ( char  , u16);

_ZLX_INT_ENLARGE    (u8     , u32);
_ZLX_INT_ENLARGE    (u16    , u32);
_ZLX_INT_ENLARGE    (u32    , u32);
_ZLX_INT_SHRINK     (u64    , u32);
_ZLX_INT_A_TO_32    (uptr   , u32);
_ZLX_INT_A_TO_32    (usize  , u32);
_ZLX_INT_A_TO_32    ( size  , u32);
_ZLX_INT_L_TO_32    (ulong  , u32);
_ZLX_INT_ENLARGE    (uint   , u32);
_ZLX_INT_ENLARGE    (ushort , u32);
_ZLX_INT_ENLARGE    (uchar  , u32);
_ZLX_INT_MIX_CONV   (s8     , u32);
_ZLX_INT_MIX_CONV   (s16    , u32);
_ZLX_INT_MIX_CONV   (s32    , u32);
_ZLX_INT_MIX_CONV   (s64    , u32);
_ZLX_INT_MIX_CONV   (sptr   , u32);
_ZLX_INT_MIX_CONV   (ssize  , u32);
_ZLX_INT_MIX_CONV   (ptrdiff, u32);
_ZLX_INT_MIX_CONV   (slong  , u32);
_ZLX_INT_MIX_CONV   ( long  , u32);
_ZLX_INT_MIX_CONV   (sint   , u32);
_ZLX_INT_MIX_CONV   ( int   , u32);
_ZLX_INT_MIX_CONV   (sshort , u32);
_ZLX_INT_MIX_CONV   ( short , u32);
_ZLX_INT_MIX_CONV   (schar  , u32);
_ZLX_INT_MIX_CONV   ( char  , u32);

_ZLX_INT_ENLARGE    (u8     , u64);
_ZLX_INT_ENLARGE    (u16    , u64);
_ZLX_INT_ENLARGE    (u32    , u64);
_ZLX_INT_ENLARGE    (u64    , u64);
_ZLX_INT_ENLARGE    (uptr   , u64);
_ZLX_INT_ENLARGE    (usize  , u64);
_ZLX_INT_ENLARGE    ( size  , u64);
_ZLX_INT_ENLARGE    (ulong  , u64);
_ZLX_INT_ENLARGE    (uint   , u64);
_ZLX_INT_ENLARGE    (ushort , u64);
_ZLX_INT_ENLARGE    (uchar  , u64);
_ZLX_INT_MIX_CONV   (s8     , u64);
_ZLX_INT_MIX_CONV   (s16    , u64);
_ZLX_INT_MIX_CONV   (s32    , u64);
_ZLX_INT_MIX_CONV   (s64    , u64);
_ZLX_INT_MIX_CONV   (sptr   , u64);
_ZLX_INT_MIX_CONV   (ssize  , u64);
_ZLX_INT_MIX_CONV   (ptrdiff, u64);
_ZLX_INT_MIX_CONV   (slong  , u64);
_ZLX_INT_MIX_CONV   ( long  , u64);
_ZLX_INT_MIX_CONV   (sint   , u64);
_ZLX_INT_MIX_CONV   ( int   , u64);
_ZLX_INT_MIX_CONV   (sshort , u64);
_ZLX_INT_MIX_CONV   ( short , u64);
_ZLX_INT_MIX_CONV   (schar  , u64);
_ZLX_INT_MIX_CONV   ( char  , u64);

_ZLX_INT_ENLARGE    (u8     , uptr);
_ZLX_INT_ENLARGE    (u16    , uptr);
_ZLX_INT_ENLARGE    (u32    , uptr);
_ZLX_INT_64_TO_A    (u64    , uptr);
_ZLX_INT_ENLARGE    (uptr   , uptr);
_ZLX_INT_ENLARGE    (usize  , uptr);
_ZLX_INT_ENLARGE    ( size  , uptr);
_ZLX_INT_ENLARGE    (ulong  , uptr);
_ZLX_INT_ENLARGE    (uint   , uptr);
_ZLX_INT_ENLARGE    (ushort , uptr);
_ZLX_INT_ENLARGE    (uchar  , uptr);
_ZLX_INT_MIX_CONV   (s8     , uptr);
_ZLX_INT_MIX_CONV   (s16    , uptr);
_ZLX_INT_MIX_CONV   (s32    , uptr);
_ZLX_INT_MIX_CONV   (s64    , uptr);
_ZLX_INT_MIX_CONV   (sptr   , uptr);
_ZLX_INT_MIX_CONV   (ssize  , uptr);
_ZLX_INT_MIX_CONV   (ptrdiff, uptr);
_ZLX_INT_MIX_CONV   (slong  , uptr);
_ZLX_INT_MIX_CONV   ( long  , uptr);
_ZLX_INT_MIX_CONV   (sint   , uptr);
_ZLX_INT_MIX_CONV   ( int   , uptr);
_ZLX_INT_MIX_CONV   (sshort , uptr);
_ZLX_INT_MIX_CONV   ( short , uptr);
_ZLX_INT_MIX_CONV   (schar  , uptr);
_ZLX_INT_MIX_CONV   ( char  , uptr);

_ZLX_INT_ENLARGE    (u8     , usize);
_ZLX_INT_ENLARGE    (u16    , usize);
_ZLX_INT_ENLARGE    (u32    , usize);
_ZLX_INT_64_TO_A    (u64    , usize);
_ZLX_INT_ENLARGE    (uptr   , usize);
_ZLX_INT_ENLARGE    (usize  , usize);
_ZLX_INT_ENLARGE    ( size  , usize);
_ZLX_INT_ENLARGE    (ulong  , usize);
_ZLX_INT_ENLARGE    (uint   , usize);
_ZLX_INT_ENLARGE    (ushort , usize);
_ZLX_INT_ENLARGE    (uchar  , usize);
_ZLX_INT_MIX_CONV   (s8     , usize);
_ZLX_INT_MIX_CONV   (s16    , usize);
_ZLX_INT_MIX_CONV   (s32    , usize);
_ZLX_INT_MIX_CONV   (s64    , usize);
_ZLX_INT_MIX_CONV   (sptr   , usize);
_ZLX_INT_MIX_CONV   (ssize  , usize);
_ZLX_INT_MIX_CONV   (ptrdiff, usize);
_ZLX_INT_MIX_CONV   (slong  , usize);
_ZLX_INT_MIX_CONV   ( long  , usize);
_ZLX_INT_MIX_CONV   (sint   , usize);
_ZLX_INT_MIX_CONV   ( int   , usize);
_ZLX_INT_MIX_CONV   (sshort , usize);
_ZLX_INT_MIX_CONV   ( short , usize);
_ZLX_INT_MIX_CONV   (schar  , usize);
_ZLX_INT_MIX_CONV   ( char  , usize);

_ZLX_INT_ENLARGE    (u8     ,  size);
_ZLX_INT_ENLARGE    (u16    ,  size);
_ZLX_INT_ENLARGE    (u32    ,  size);
_ZLX_INT_64_TO_A    (u64    ,  size);
_ZLX_INT_ENLARGE    (uptr   ,  size);
_ZLX_INT_ENLARGE    (usize  ,  size);
_ZLX_INT_ENLARGE    ( size  ,  size);
_ZLX_INT_ENLARGE    (ulong  ,  size);
_ZLX_INT_ENLARGE    (uint   ,  size);
_ZLX_INT_ENLARGE    (ushort ,  size);
_ZLX_INT_ENLARGE    (uchar  ,  size);
_ZLX_INT_MIX_CONV   (s8     ,  size);
_ZLX_INT_MIX_CONV   (s16    ,  size);
_ZLX_INT_MIX_CONV   (s32    ,  size);
_ZLX_INT_MIX_CONV   (s64    ,  size);
_ZLX_INT_MIX_CONV   (sptr   ,  size);
_ZLX_INT_MIX_CONV   (ssize  ,  size);
_ZLX_INT_MIX_CONV   (ptrdiff,  size);
_ZLX_INT_MIX_CONV   (slong  ,  size);
_ZLX_INT_MIX_CONV   ( long  ,  size);
_ZLX_INT_MIX_CONV   (sint   ,  size);
_ZLX_INT_MIX_CONV   ( int   ,  size);
_ZLX_INT_MIX_CONV   (sshort ,  size);
_ZLX_INT_MIX_CONV   ( short ,  size);
_ZLX_INT_MIX_CONV   (schar  ,  size);
_ZLX_INT_MIX_CONV   ( char  ,  size);

_ZLX_INT_ENLARGE    (u8     , ulong);
_ZLX_INT_ENLARGE    (u16    , ulong);
_ZLX_INT_ENLARGE    (u32    , ulong);
_ZLX_INT_64_TO_L    (u64    , ulong);
_ZLX_INT_A_TO_L     (uptr   , ulong);
_ZLX_INT_A_TO_L     (usize  , ulong);
_ZLX_INT_A_TO_L     ( size  , ulong);
_ZLX_INT_ENLARGE    (ulong  , ulong);
_ZLX_INT_ENLARGE    (uint   , ulong);
_ZLX_INT_ENLARGE    (ushort , ulong);
_ZLX_INT_ENLARGE    (uchar  , ulong);
_ZLX_INT_MIX_CONV   (s8     , ulong);
_ZLX_INT_MIX_CONV   (s16    , ulong);
_ZLX_INT_MIX_CONV   (s32    , ulong);
_ZLX_INT_MIX_CONV   (s64    , ulong);
_ZLX_INT_MIX_CONV   (sptr   , ulong);
_ZLX_INT_MIX_CONV   (ssize  , ulong);
_ZLX_INT_MIX_CONV   (ptrdiff, ulong);
_ZLX_INT_MIX_CONV   (slong  , ulong);
_ZLX_INT_MIX_CONV   ( long  , ulong);
_ZLX_INT_MIX_CONV   (sint   , ulong);
_ZLX_INT_MIX_CONV   ( int   , ulong);
_ZLX_INT_MIX_CONV   (sshort , ulong);
_ZLX_INT_MIX_CONV   ( short , ulong);
_ZLX_INT_MIX_CONV   (schar  , ulong);
_ZLX_INT_MIX_CONV   ( char  , ulong);

_ZLX_INT_ENLARGE    (u8     , uint);
_ZLX_INT_ENLARGE    (u16    , uint);
_ZLX_INT_ENLARGE    (u32    , uint);
_ZLX_INT_SHRINK     (u64    , uint);
_ZLX_INT_A_TO_32    (uptr   , uint);
_ZLX_INT_A_TO_32    (usize  , uint);
_ZLX_INT_A_TO_32    ( size  , uint);
_ZLX_INT_L_TO_32    (ulong  , uint);
_ZLX_INT_ENLARGE    (uint   , uint);
_ZLX_INT_ENLARGE    (ushort , uint);
_ZLX_INT_ENLARGE    (uchar  , uint);
_ZLX_INT_MIX_CONV   (s8     , uint);
_ZLX_INT_MIX_CONV   (s16    , uint);
_ZLX_INT_MIX_CONV   (s32    , uint);
_ZLX_INT_MIX_CONV   (s64    , uint);
_ZLX_INT_MIX_CONV   (sptr   , uint);
_ZLX_INT_MIX_CONV   (ssize  , uint);
_ZLX_INT_MIX_CONV   (ptrdiff, uint);
_ZLX_INT_MIX_CONV   (slong  , uint);
_ZLX_INT_MIX_CONV   ( long  , uint);
_ZLX_INT_MIX_CONV   (sint   , uint);
_ZLX_INT_MIX_CONV   ( int   , uint);
_ZLX_INT_MIX_CONV   (sshort , uint);
_ZLX_INT_MIX_CONV   ( short , uint);
_ZLX_INT_MIX_CONV   (schar  , uint);
_ZLX_INT_MIX_CONV   ( char  , uint);

_ZLX_INT_ENLARGE    (u8     , ushort);
_ZLX_INT_ENLARGE    (u16    , ushort);
_ZLX_INT_SHRINK     (u32    , ushort);
_ZLX_INT_SHRINK     (u64    , ushort);
_ZLX_INT_SHRINK     (uptr   , ushort);
_ZLX_INT_SHRINK     (usize  , ushort);
_ZLX_INT_SHRINK     ( size  , ushort);
_ZLX_INT_SHRINK     (ulong  , ushort);
_ZLX_INT_SHRINK     (uint   , ushort);
_ZLX_INT_ENLARGE    (ushort , ushort);
_ZLX_INT_ENLARGE    (uchar  , ushort);
_ZLX_INT_MIX_CONV   (s8     , ushort);
_ZLX_INT_MIX_CONV   (s16    , ushort);
_ZLX_INT_MIX_CONV   (s32    , ushort);
_ZLX_INT_MIX_CONV   (s64    , ushort);
_ZLX_INT_MIX_CONV   (sptr   , ushort);
_ZLX_INT_MIX_CONV   (ssize  , ushort);
_ZLX_INT_MIX_CONV   (ptrdiff, ushort);
_ZLX_INT_MIX_CONV   (slong  , ushort);
_ZLX_INT_MIX_CONV   ( long  , ushort);
_ZLX_INT_MIX_CONV   (sint   , ushort);
_ZLX_INT_MIX_CONV   ( int   , ushort);
_ZLX_INT_MIX_CONV   (sshort , ushort);
_ZLX_INT_MIX_CONV   ( short , ushort);
_ZLX_INT_MIX_CONV   (schar  , ushort);
_ZLX_INT_MIX_CONV   ( char  , ushort);

_ZLX_INT_ENLARGE    (u8     , uchar);
_ZLX_INT_SHRINK     (u16    , uchar);
_ZLX_INT_SHRINK     (u32    , uchar);
_ZLX_INT_SHRINK     (u64    , uchar);
_ZLX_INT_SHRINK     (uptr   , uchar);
_ZLX_INT_SHRINK     (usize  , uchar);
_ZLX_INT_SHRINK     ( size  , uchar);
_ZLX_INT_SHRINK     (ulong  , uchar);
_ZLX_INT_SHRINK     (uint   , uchar);
_ZLX_INT_SHRINK     (ushort , uchar);
_ZLX_INT_ENLARGE    (uchar  , uchar);
_ZLX_INT_MIX_CONV   (s8     , uchar);
_ZLX_INT_MIX_CONV   (s16    , uchar);
_ZLX_INT_MIX_CONV   (s32    , uchar);
_ZLX_INT_MIX_CONV   (s64    , uchar);
_ZLX_INT_MIX_CONV   (sptr   , uchar);
_ZLX_INT_MIX_CONV   (ssize  , uchar);
_ZLX_INT_MIX_CONV   (ptrdiff, uchar);
_ZLX_INT_MIX_CONV   (slong  , uchar);
_ZLX_INT_MIX_CONV   ( long  , uchar);
_ZLX_INT_MIX_CONV   (sint   , uchar);
_ZLX_INT_MIX_CONV   ( int   , uchar);
_ZLX_INT_MIX_CONV   (sshort , uchar);
_ZLX_INT_MIX_CONV   ( short , uchar);
_ZLX_INT_MIX_CONV   (schar  , uchar);
_ZLX_INT_MIX_CONV   ( char  , uchar);


_ZLX_INT_MIX_CONV   (u8     , s8);
_ZLX_INT_MIX_CONV   (u16    , s8);
_ZLX_INT_MIX_CONV   (u32    , s8);
_ZLX_INT_MIX_CONV   (u64    , s8);
_ZLX_INT_MIX_CONV   (uptr   , s8);
_ZLX_INT_MIX_CONV   (usize  , s8);
_ZLX_INT_MIX_CONV   ( size  , s8);
_ZLX_INT_MIX_CONV   (ulong  , s8);
_ZLX_INT_MIX_CONV   (uint   , s8);
_ZLX_INT_MIX_CONV   (ushort , s8);
_ZLX_INT_MIX_CONV   (uchar  , s8);
_ZLX_INT_ENLARGE    (s8     , s8);
_ZLX_INT_SHRINK     (s16    , s8);
_ZLX_INT_SHRINK     (s32    , s8);
_ZLX_INT_SHRINK     (s64    , s8);
_ZLX_INT_SHRINK     (sptr   , s8);
_ZLX_INT_SHRINK     (ssize  , s8);
_ZLX_INT_SHRINK     (ptrdiff, s8);
_ZLX_INT_SHRINK     (slong  , s8);
_ZLX_INT_SHRINK     ( long  , s8);
_ZLX_INT_SHRINK     (sint   , s8);
_ZLX_INT_SHRINK     ( int   , s8);
_ZLX_INT_SHRINK     (sshort , s8);
_ZLX_INT_SHRINK     ( short , s8);
_ZLX_INT_ENLARGE    (schar  , s8);
_ZLX_INT_ENLARGE    ( char  , s8);

_ZLX_INT_MIX_CONV   (u8     , s16);
_ZLX_INT_MIX_CONV   (u16    , s16);
_ZLX_INT_MIX_CONV   (u32    , s16);
_ZLX_INT_MIX_CONV   (u64    , s16);
_ZLX_INT_MIX_CONV   (uptr   , s16);
_ZLX_INT_MIX_CONV   (usize  , s16);
_ZLX_INT_MIX_CONV   ( size  , s16);
_ZLX_INT_MIX_CONV   (ulong  , s16);
_ZLX_INT_MIX_CONV   (uint   , s16);
_ZLX_INT_MIX_CONV   (ushort , s16);
_ZLX_INT_MIX_CONV   (uchar  , s16);
_ZLX_INT_ENLARGE    (s8     , s16);
_ZLX_INT_ENLARGE    (s16    , s16);
_ZLX_INT_SHRINK     (s32    , s16);
_ZLX_INT_SHRINK     (s64    , s16);
_ZLX_INT_SHRINK     (sptr   , s16);
_ZLX_INT_SHRINK     (ssize  , s16);
_ZLX_INT_SHRINK     (ptrdiff, s16);
_ZLX_INT_SHRINK     (slong  , s16);
_ZLX_INT_SHRINK     ( long  , s16);
_ZLX_INT_SHRINK     (sint   , s16);
_ZLX_INT_SHRINK     ( int   , s16);
_ZLX_INT_ENLARGE    (sshort , s16);
_ZLX_INT_ENLARGE    ( short , s16);
_ZLX_INT_ENLARGE    (schar  , s16);
_ZLX_INT_ENLARGE    ( char  , s16);

_ZLX_INT_MIX_CONV   (u8     , s32);
_ZLX_INT_MIX_CONV   (u16    , s32);
_ZLX_INT_MIX_CONV   (u32    , s32);
_ZLX_INT_MIX_CONV   (u64    , s32);
_ZLX_INT_MIX_CONV   (uptr   , s32);
_ZLX_INT_MIX_CONV   (usize  , s32);
_ZLX_INT_MIX_CONV   ( size  , s32);
_ZLX_INT_MIX_CONV   (ulong  , s32);
_ZLX_INT_MIX_CONV   (uint   , s32);
_ZLX_INT_MIX_CONV   (ushort , s32);
_ZLX_INT_MIX_CONV   (uchar  , s32);
_ZLX_INT_ENLARGE    (s8     , s32);
_ZLX_INT_ENLARGE    (s16    , s32);
_ZLX_INT_ENLARGE    (s32    , s32);
_ZLX_INT_SHRINK     (s64    , s32);
_ZLX_INT_A_TO_32    (sptr   , s32);
_ZLX_INT_A_TO_32    (ssize  , s32);
_ZLX_INT_A_TO_32    (ptrdiff, s32);
_ZLX_INT_L_TO_32    (slong  , s32);
_ZLX_INT_L_TO_32    ( long  , s32);
_ZLX_INT_ENLARGE    (sint   , s32);
_ZLX_INT_ENLARGE    ( int   , s32);
_ZLX_INT_ENLARGE    (sshort , s32);
_ZLX_INT_ENLARGE    ( short , s32);
_ZLX_INT_ENLARGE    (schar  , s32);
_ZLX_INT_ENLARGE    ( char  , s32);

_ZLX_INT_MIX_CONV   (u8     , s64);
_ZLX_INT_MIX_CONV   (u16    , s64);
_ZLX_INT_MIX_CONV   (u32    , s64);
_ZLX_INT_MIX_CONV   (u64    , s64);
_ZLX_INT_MIX_CONV   (uptr   , s64);
_ZLX_INT_MIX_CONV   (usize  , s64);
_ZLX_INT_MIX_CONV   ( size  , s64);
_ZLX_INT_MIX_CONV   (ulong  , s64);
_ZLX_INT_MIX_CONV   (uint   , s64);
_ZLX_INT_MIX_CONV   (ushort , s64);
_ZLX_INT_MIX_CONV   (uchar  , s64);
_ZLX_INT_ENLARGE    (s8     , s64);
_ZLX_INT_ENLARGE    (s16    , s64);
_ZLX_INT_ENLARGE    (s32    , s64);
_ZLX_INT_ENLARGE    (s64    , s64);
_ZLX_INT_ENLARGE    (sptr   , s64);
_ZLX_INT_ENLARGE    (ssize  , s64);
_ZLX_INT_ENLARGE    (ptrdiff, s64);
_ZLX_INT_ENLARGE    (slong  , s64);
_ZLX_INT_ENLARGE    ( long  , s64);
_ZLX_INT_ENLARGE    (sint   , s64);
_ZLX_INT_ENLARGE    ( int   , s64);
_ZLX_INT_ENLARGE    (sshort , s64);
_ZLX_INT_ENLARGE    ( short , s64);
_ZLX_INT_ENLARGE    (schar  , s64);
_ZLX_INT_ENLARGE    ( char  , s64);

_ZLX_INT_MIX_CONV   (u8     , sptr);
_ZLX_INT_MIX_CONV   (u16    , sptr);
_ZLX_INT_MIX_CONV   (u32    , sptr);
_ZLX_INT_MIX_CONV   (u64    , sptr);
_ZLX_INT_MIX_CONV   (uptr   , sptr);
_ZLX_INT_MIX_CONV   (usize  , sptr);
_ZLX_INT_MIX_CONV   ( size  , sptr);
_ZLX_INT_MIX_CONV   (ulong  , sptr);
_ZLX_INT_MIX_CONV   (uint   , sptr);
_ZLX_INT_MIX_CONV   (ushort , sptr);
_ZLX_INT_MIX_CONV   (uchar  , sptr);
_ZLX_INT_ENLARGE    (s8     , sptr);
_ZLX_INT_ENLARGE    (s16    , sptr);
_ZLX_INT_ENLARGE    (s32    , sptr);
_ZLX_INT_64_TO_A    (s64    , sptr);
_ZLX_INT_ENLARGE    (sptr   , sptr);
_ZLX_INT_ENLARGE    (ssize  , sptr);
_ZLX_INT_ENLARGE    (ptrdiff, sptr);
_ZLX_INT_ENLARGE    (slong  , sptr);
_ZLX_INT_ENLARGE    ( long  , sptr);
_ZLX_INT_ENLARGE    (sint   , sptr);
_ZLX_INT_ENLARGE    ( int   , sptr);
_ZLX_INT_ENLARGE    (sshort , sptr);
_ZLX_INT_ENLARGE    ( short , sptr);
_ZLX_INT_ENLARGE    (schar  , sptr);
_ZLX_INT_ENLARGE    ( char  , sptr);

_ZLX_INT_MIX_CONV   (u8     , ssize);
_ZLX_INT_MIX_CONV   (u16    , ssize);
_ZLX_INT_MIX_CONV   (u32    , ssize);
_ZLX_INT_MIX_CONV   (u64    , ssize);
_ZLX_INT_MIX_CONV   (uptr   , ssize);
_ZLX_INT_MIX_CONV   (usize  , ssize);
_ZLX_INT_MIX_CONV   ( size  , ssize);
_ZLX_INT_MIX_CONV   (ulong  , ssize);
_ZLX_INT_MIX_CONV   (uint   , ssize);
_ZLX_INT_MIX_CONV   (ushort , ssize);
_ZLX_INT_MIX_CONV   (uchar  , ssize);
_ZLX_INT_ENLARGE    (s8     , ssize);
_ZLX_INT_ENLARGE    (s16    , ssize);
_ZLX_INT_ENLARGE    (s32    , ssize);
_ZLX_INT_64_TO_A    (s64    , ssize);
_ZLX_INT_ENLARGE    (sptr   , ssize);
_ZLX_INT_ENLARGE    (ssize  , ssize);
_ZLX_INT_ENLARGE    (ptrdiff, ssize);
_ZLX_INT_ENLARGE    (slong  , ssize);
_ZLX_INT_ENLARGE    ( long  , ssize);
_ZLX_INT_ENLARGE    (sint   , ssize);
_ZLX_INT_ENLARGE    ( int   , ssize);
_ZLX_INT_ENLARGE    (sshort , ssize);
_ZLX_INT_ENLARGE    ( short , ssize);
_ZLX_INT_ENLARGE    (schar  , ssize);
_ZLX_INT_ENLARGE    ( char  , ssize);

_ZLX_INT_MIX_CONV   (u8     , ptrdiff);
_ZLX_INT_MIX_CONV   (u16    , ptrdiff);
_ZLX_INT_MIX_CONV   (u32    , ptrdiff);
_ZLX_INT_MIX_CONV   (u64    , ptrdiff);
_ZLX_INT_MIX_CONV   (uptr   , ptrdiff);
_ZLX_INT_MIX_CONV   (usize  , ptrdiff);
_ZLX_INT_MIX_CONV   ( size  , ptrdiff);
_ZLX_INT_MIX_CONV   (ulong  , ptrdiff);
_ZLX_INT_MIX_CONV   (uint   , ptrdiff);
_ZLX_INT_MIX_CONV   (ushort , ptrdiff);
_ZLX_INT_MIX_CONV   (uchar  , ptrdiff);
_ZLX_INT_ENLARGE    (s8     , ptrdiff);
_ZLX_INT_ENLARGE    (s16    , ptrdiff);
_ZLX_INT_ENLARGE    (s32    , ptrdiff);
_ZLX_INT_64_TO_A    (s64    , ptrdiff);
_ZLX_INT_ENLARGE    (sptr   , ptrdiff);
_ZLX_INT_ENLARGE    (ssize  , ptrdiff);
_ZLX_INT_ENLARGE    (ptrdiff, ptrdiff);
_ZLX_INT_ENLARGE    (slong  , ptrdiff);
_ZLX_INT_ENLARGE    ( long  , ptrdiff);
_ZLX_INT_ENLARGE    (sint   , ptrdiff);
_ZLX_INT_ENLARGE    ( int   , ptrdiff);
_ZLX_INT_ENLARGE    (sshort , ptrdiff);
_ZLX_INT_ENLARGE    ( short , ptrdiff);
_ZLX_INT_ENLARGE    (schar  , ptrdiff);
_ZLX_INT_ENLARGE    ( char  , ptrdiff);

_ZLX_INT_MIX_CONV   (u8     , slong);
_ZLX_INT_MIX_CONV   (u16    , slong);
_ZLX_INT_MIX_CONV   (u32    , slong);
_ZLX_INT_MIX_CONV   (u64    , slong);
_ZLX_INT_MIX_CONV   (uptr   , slong);
_ZLX_INT_MIX_CONV   (usize  , slong);
_ZLX_INT_MIX_CONV   ( size  , slong);
_ZLX_INT_MIX_CONV   (ulong  , slong);
_ZLX_INT_MIX_CONV   (uint   , slong);
_ZLX_INT_MIX_CONV   (ushort , slong);
_ZLX_INT_MIX_CONV   (uchar  , slong);
_ZLX_INT_ENLARGE    (s8     , slong);
_ZLX_INT_ENLARGE    (s16    , slong);
_ZLX_INT_ENLARGE    (s32    , slong);
_ZLX_INT_64_TO_L    (s64    , slong);
_ZLX_INT_A_TO_L     (sptr   , slong);
_ZLX_INT_A_TO_L     (ssize  , slong);
_ZLX_INT_A_TO_L     (ptrdiff, slong);
_ZLX_INT_ENLARGE    (slong  , slong);
_ZLX_INT_ENLARGE    ( long  , slong);
_ZLX_INT_ENLARGE    (sint   , slong);
_ZLX_INT_ENLARGE    ( int   , slong);
_ZLX_INT_ENLARGE    (sshort , slong);
_ZLX_INT_ENLARGE    ( short , slong);
_ZLX_INT_ENLARGE    (schar  , slong);
_ZLX_INT_ENLARGE    ( char  , slong);

_ZLX_INT_MIX_CONV   (u8     ,  long);
_ZLX_INT_MIX_CONV   (u16    ,  long);
_ZLX_INT_MIX_CONV   (u32    ,  long);
_ZLX_INT_MIX_CONV   (u64    ,  long);
_ZLX_INT_MIX_CONV   (uptr   ,  long);
_ZLX_INT_MIX_CONV   (usize  ,  long);
_ZLX_INT_MIX_CONV   ( size  ,  long);
_ZLX_INT_MIX_CONV   (ulong  ,  long);
_ZLX_INT_MIX_CONV   (uint   ,  long);
_ZLX_INT_MIX_CONV   (ushort ,  long);
_ZLX_INT_MIX_CONV   (uchar  ,  long);
_ZLX_INT_ENLARGE    (s8     ,  long);
_ZLX_INT_ENLARGE    (s16    ,  long);
_ZLX_INT_ENLARGE    (s32    ,  long);
_ZLX_INT_64_TO_L    (s64    ,  long);
_ZLX_INT_A_TO_L     (sptr   ,  long);
_ZLX_INT_A_TO_L     (ssize  ,  long);
_ZLX_INT_A_TO_L     (ptrdiff,  long);
_ZLX_INT_ENLARGE    (slong  ,  long);
_ZLX_INT_ENLARGE    ( long  ,  long);
_ZLX_INT_ENLARGE    (sint   ,  long);
_ZLX_INT_ENLARGE    ( int   ,  long);
_ZLX_INT_ENLARGE    (sshort ,  long);
_ZLX_INT_ENLARGE    ( short ,  long);
_ZLX_INT_ENLARGE    (schar  ,  long);
_ZLX_INT_ENLARGE    ( char  ,  long);

_ZLX_INT_MIX_CONV   (u8     , sint);
_ZLX_INT_MIX_CONV   (u16    , sint);
_ZLX_INT_MIX_CONV   (u32    , sint);
_ZLX_INT_MIX_CONV   (u64    , sint);
_ZLX_INT_MIX_CONV   (uptr   , sint);
_ZLX_INT_MIX_CONV   (usize  , sint);
_ZLX_INT_MIX_CONV   ( size  , sint);
_ZLX_INT_MIX_CONV   (ulong  , sint);
_ZLX_INT_MIX_CONV   (uint   , sint);
_ZLX_INT_MIX_CONV   (ushort , sint);
_ZLX_INT_MIX_CONV   (uchar  , sint);
_ZLX_INT_ENLARGE    (s8     , sint);
_ZLX_INT_ENLARGE    (s16    , sint);
_ZLX_INT_ENLARGE    (s32    , sint);
_ZLX_INT_SHRINK     (s64    , sint);
_ZLX_INT_A_TO_32    (sptr   , sint);
_ZLX_INT_A_TO_32    (ssize  , sint);
_ZLX_INT_A_TO_32    (ptrdiff, sint);
_ZLX_INT_L_TO_32    (slong  , sint);
_ZLX_INT_L_TO_32    ( long  , sint);
_ZLX_INT_ENLARGE    (sint   , sint);
_ZLX_INT_ENLARGE    ( int   , sint);
_ZLX_INT_ENLARGE    (sshort , sint);
_ZLX_INT_ENLARGE    ( short , sint);
_ZLX_INT_ENLARGE    (schar  , sint);
_ZLX_INT_ENLARGE    ( char  , sint);

_ZLX_INT_MIX_CONV   (u8     ,  int);
_ZLX_INT_MIX_CONV   (u16    ,  int);
_ZLX_INT_MIX_CONV   (u32    ,  int);
_ZLX_INT_MIX_CONV   (u64    ,  int);
_ZLX_INT_MIX_CONV   (uptr   ,  int);
_ZLX_INT_MIX_CONV   (usize  ,  int);
_ZLX_INT_MIX_CONV   ( size  ,  int);
_ZLX_INT_MIX_CONV   (ulong  ,  int);
_ZLX_INT_MIX_CONV   (uint   ,  int);
_ZLX_INT_MIX_CONV   (ushort ,  int);
_ZLX_INT_MIX_CONV   (uchar  ,  int);
_ZLX_INT_ENLARGE    (s8     ,  int);
_ZLX_INT_ENLARGE    (s16    ,  int);
_ZLX_INT_ENLARGE    (s32    ,  int);
_ZLX_INT_SHRINK     (s64    ,  int);
_ZLX_INT_A_TO_32    (sptr   ,  int);
_ZLX_INT_A_TO_32    (ssize  ,  int);
_ZLX_INT_A_TO_32    (ptrdiff,  int);
_ZLX_INT_L_TO_32    (slong  ,  int);
_ZLX_INT_L_TO_32    ( long  ,  int);
_ZLX_INT_ENLARGE    (sint   ,  int);
_ZLX_INT_ENLARGE    ( int   ,  int);
_ZLX_INT_ENLARGE    (sshort ,  int);
_ZLX_INT_ENLARGE    ( short ,  int);
_ZLX_INT_ENLARGE    (schar  ,  int);
_ZLX_INT_ENLARGE    ( char  ,  int);

_ZLX_INT_MIX_CONV   (u8     , sshort);
_ZLX_INT_MIX_CONV   (u16    , sshort);
_ZLX_INT_MIX_CONV   (u32    , sshort);
_ZLX_INT_MIX_CONV   (u64    , sshort);
_ZLX_INT_MIX_CONV   (uptr   , sshort);
_ZLX_INT_MIX_CONV   (usize  , sshort);
_ZLX_INT_MIX_CONV   ( size  , sshort);
_ZLX_INT_MIX_CONV   (ulong  , sshort);
_ZLX_INT_MIX_CONV   (uint   , sshort);
_ZLX_INT_MIX_CONV   (ushort , sshort);
_ZLX_INT_MIX_CONV   (uchar  , sshort);
_ZLX_INT_ENLARGE    (s8     , sshort);
_ZLX_INT_ENLARGE    (s16    , sshort);
_ZLX_INT_SHRINK     (s32    , sshort);
_ZLX_INT_SHRINK     (s64    , sshort);
_ZLX_INT_SHRINK     (sptr   , sshort);
_ZLX_INT_SHRINK     (ssize  , sshort);
_ZLX_INT_SHRINK     (ptrdiff, sshort);
_ZLX_INT_SHRINK     (slong  , sshort);
_ZLX_INT_SHRINK     ( long  , sshort);
_ZLX_INT_SHRINK     (sint   , sshort);
_ZLX_INT_SHRINK     ( int   , sshort);
_ZLX_INT_ENLARGE    (sshort , sshort);
_ZLX_INT_ENLARGE    ( short , sshort);
_ZLX_INT_ENLARGE    (schar  , sshort);
_ZLX_INT_ENLARGE    ( char  , sshort);

_ZLX_INT_MIX_CONV   (u8     ,  short);
_ZLX_INT_MIX_CONV   (u16    ,  short);
_ZLX_INT_MIX_CONV   (u32    ,  short);
_ZLX_INT_MIX_CONV   (u64    ,  short);
_ZLX_INT_MIX_CONV   (uptr   ,  short);
_ZLX_INT_MIX_CONV   (usize  ,  short);
_ZLX_INT_MIX_CONV   ( size  ,  short);
_ZLX_INT_MIX_CONV   (ulong  ,  short);
_ZLX_INT_MIX_CONV   (uint   ,  short);
_ZLX_INT_MIX_CONV   (ushort ,  short);
_ZLX_INT_MIX_CONV   (uchar  ,  short);
_ZLX_INT_ENLARGE    (s8     ,  short);
_ZLX_INT_ENLARGE    (s16    ,  short);
_ZLX_INT_SHRINK     (s32    ,  short);
_ZLX_INT_SHRINK     (s64    ,  short);
_ZLX_INT_SHRINK     (sptr   ,  short);
_ZLX_INT_SHRINK     (ssize  ,  short);
_ZLX_INT_SHRINK     (ptrdiff,  short);
_ZLX_INT_SHRINK     (slong  ,  short);
_ZLX_INT_SHRINK     ( long  ,  short);
_ZLX_INT_SHRINK     (sint   ,  short);
_ZLX_INT_SHRINK     ( int   ,  short);
_ZLX_INT_ENLARGE    (sshort ,  short);
_ZLX_INT_ENLARGE    ( short ,  short);
_ZLX_INT_ENLARGE    (schar  ,  short);
_ZLX_INT_ENLARGE    ( char  ,  short);

_ZLX_INT_MIX_CONV   (u8     , schar);
_ZLX_INT_MIX_CONV   (u16    , schar);
_ZLX_INT_MIX_CONV   (u32    , schar);
_ZLX_INT_MIX_CONV   (u64    , schar);
_ZLX_INT_MIX_CONV   (uptr   , schar);
_ZLX_INT_MIX_CONV   (usize  , schar);
_ZLX_INT_MIX_CONV   ( size  , schar);
_ZLX_INT_MIX_CONV   (ulong  , schar);
_ZLX_INT_MIX_CONV   (uint   , schar);
_ZLX_INT_MIX_CONV   (ushort , schar);
_ZLX_INT_MIX_CONV   (uchar  , schar);
_ZLX_INT_ENLARGE    (s8     , schar);
_ZLX_INT_SHRINK     (s16    , schar);
_ZLX_INT_SHRINK     (s32    , schar);
_ZLX_INT_SHRINK     (s64    , schar);
_ZLX_INT_SHRINK     (sptr   , schar);
_ZLX_INT_SHRINK     (ssize  , schar);
_ZLX_INT_SHRINK     (ptrdiff, schar);
_ZLX_INT_SHRINK     (slong  , schar);
_ZLX_INT_SHRINK     ( long  , schar);
_ZLX_INT_SHRINK     (sint   , schar);
_ZLX_INT_SHRINK     ( int   , schar);
_ZLX_INT_SHRINK     (sshort , schar);
_ZLX_INT_SHRINK     ( short , schar);
_ZLX_INT_ENLARGE    (schar  , schar);
_ZLX_INT_ENLARGE    ( char  , schar);

_ZLX_INT_MIX_CONV   (u8     ,  char);
_ZLX_INT_MIX_CONV   (u16    ,  char);
_ZLX_INT_MIX_CONV   (u32    ,  char);
_ZLX_INT_MIX_CONV   (u64    ,  char);
_ZLX_INT_MIX_CONV   (uptr   ,  char);
_ZLX_INT_MIX_CONV   (usize  ,  char);
_ZLX_INT_MIX_CONV   ( size  ,  char);
_ZLX_INT_MIX_CONV   (ulong  ,  char);
_ZLX_INT_MIX_CONV   (uint   ,  char);
_ZLX_INT_MIX_CONV   (ushort ,  char);
_ZLX_INT_MIX_CONV   (uchar  ,  char);
_ZLX_INT_ENLARGE    (s8     ,  char);
_ZLX_INT_SHRINK     (s16    ,  char);
_ZLX_INT_SHRINK     (s32    ,  char);
_ZLX_INT_SHRINK     (s64    ,  char);
_ZLX_INT_SHRINK     (sptr   ,  char);
_ZLX_INT_SHRINK     (ssize  ,  char);
_ZLX_INT_SHRINK     (ptrdiff,  char);
_ZLX_INT_SHRINK     (slong  ,  char);
_ZLX_INT_SHRINK     ( long  ,  char);
_ZLX_INT_SHRINK     (sint   ,  char);
_ZLX_INT_SHRINK     ( int   ,  char);
_ZLX_INT_SHRINK     (sshort ,  char);
_ZLX_INT_SHRINK     ( short ,  char);
_ZLX_INT_ENLARGE    (schar  ,  char);
_ZLX_INT_ENLARGE    ( char  ,  char);



ZLX_C_DECL_END
/** @} */

#endif /* _ZLX_INT_OPS_H */
