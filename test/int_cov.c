#include "../include/zlx/int/ops.h"
#include "../include/zlx/int/array.h"
#include "../include/zlx/int/fmt.h"

ZLX_LOCAL uint8_t u8_log2_ceil (uint8_t x) { return zlx_u8_log2_ceil(x); }
ZLX_LOCAL uint16_t u16_log2_ceil (uint16_t x) { return zlx_u16_log2_ceil(x); }
ZLX_LOCAL uint32_t u32_log2_ceil (uint32_t x) { return zlx_u32_log2_ceil(x); }
ZLX_LOCAL uint64_t u64_log2_ceil (uint64_t x) { return zlx_u64_log2_ceil(x); }
ZLX_LOCAL uint8_t size_log2_ceil (size_t x) { return zlx_size_log2_ceil(x); }
ZLX_LOCAL uint8_t uptr_log2_ceil (uintptr_t x) { return zlx_uptr_log2_ceil(x); }

ZLX_LOCAL uint8_t u8_log2_floor (uint8_t x) { return zlx_u8_log2_floor(x); }
ZLX_LOCAL uint8_t u16_log2_floor (uint16_t x) { return zlx_u16_log2_floor(x); }
ZLX_LOCAL uint8_t u32_log2_floor (uint32_t x) { return zlx_u32_log2_floor(x); }
ZLX_LOCAL uint8_t u64_log2_floor (uint64_t x) { return zlx_u64_log2_floor(x); }

uint16_t seqbswap16 (uint16_t v) { return zlx_seqbswap16(v); }
uint32_t seqbswap32 (uint32_t v) { return zlx_seqbswap32(v); }
uint64_t seqbswap64 (uint64_t v) { return zlx_seqbswap64(v); }

uint16_t bswap16 (uint16_t v) { return ZLX_BSWAP16(v); }
uint32_t bswap32 (uint32_t v) { return ZLX_BSWAP32(v); }
uint64_t bswap64 (uint64_t v) { return ZLX_BSWAP64(v); }

ZLX_LOCAL uint8_t read_u8 (uint8_t * p) { return ZLX_READ_U8(p); }
ZLX_LOCAL uint8_t aread_u8 (uint8_t * p) { return ZLX_AREAD_U8(p); }
ZLX_LOCAL uint8_t uread_u8 (uint8_t * p) { return ZLX_UREAD_U8(p); }
ZLX_LOCAL void write_u8 (uint8_t * p, uint8_t v) { ZLX_WRITE_U8(p, v); }
ZLX_LOCAL void awrite_u8 (uint8_t * p, uint8_t v) { ZLX_AWRITE_U8(p, v); }
ZLX_LOCAL void uwrite_u8 (uint8_t * p, uint8_t v) { ZLX_UWRITE_U8(p, v); }

ZLX_LOCAL uint8_t aread_u8le (uint8_t const * p) { return ZLX_AREAD_U8LE(p); }
ZLX_LOCAL uint8_t aread_u8be (uint8_t const * p) { return ZLX_AREAD_U8BE(p); }
ZLX_LOCAL uint8_t aread_u8ne (uint8_t const * p) { return ZLX_AREAD_U8NE(p); }
ZLX_LOCAL uint8_t aread_u8re (uint8_t const * p) { return ZLX_AREAD_U8RE(p); }
ZLX_LOCAL uint8_t uread_u8le (uint8_t const * p) { return ZLX_UREAD_U8LE(p); }
ZLX_LOCAL uint8_t uread_u8be (uint8_t const * p) { return ZLX_UREAD_U8BE(p); }
ZLX_LOCAL uint8_t uread_u8ne (uint8_t const * p) { return ZLX_UREAD_U8NE(p); }
ZLX_LOCAL uint8_t uread_u8re (uint8_t const * p) { return ZLX_UREAD_U8RE(p); }
ZLX_LOCAL void awrite_u8le (uint8_t * p, uint8_t v) { ZLX_AWRITE_U8LE(p, v); }
ZLX_LOCAL void awrite_u8be (uint8_t * p, uint8_t v) { ZLX_AWRITE_U8BE(p, v); }
ZLX_LOCAL void awrite_u8ne (uint8_t * p, uint8_t v) { ZLX_AWRITE_U8NE(p, v); }
ZLX_LOCAL void awrite_u8re (uint8_t * p, uint8_t v) { ZLX_AWRITE_U8RE(p, v); }
ZLX_LOCAL void uwrite_u8le (uint8_t * p, uint8_t v) { ZLX_UWRITE_U8LE(p, v); }
ZLX_LOCAL void uwrite_u8be (uint8_t * p, uint8_t v) { ZLX_UWRITE_U8BE(p, v); }
ZLX_LOCAL void uwrite_u8ne (uint8_t * p, uint8_t v) { ZLX_UWRITE_U8NE(p, v); }
ZLX_LOCAL void uwrite_u8re (uint8_t * p, uint8_t v) { ZLX_UWRITE_U8RE(p, v); }

ZLX_LOCAL uint16_t aread_u16le (uint8_t const * p) { return ZLX_AREAD_U16LE(p); }
ZLX_LOCAL uint16_t aread_u16be (uint8_t const * p) { return ZLX_AREAD_U16BE(p); }
ZLX_LOCAL uint16_t aread_u16ne (uint8_t const * p) { return ZLX_AREAD_U16NE(p); }
ZLX_LOCAL uint16_t aread_u16re (uint8_t const * p) { return ZLX_AREAD_U16RE(p); }
ZLX_LOCAL uint16_t uread_u16le (uint8_t const * p) { return ZLX_UREAD_U16LE(p); }
ZLX_LOCAL uint16_t uread_u16be (uint8_t const * p) { return ZLX_UREAD_U16BE(p); }
ZLX_LOCAL uint16_t uread_u16ne (uint8_t const * p) { return ZLX_UREAD_U16NE(p); }
ZLX_LOCAL uint16_t uread_u16re (uint8_t const * p) { return ZLX_UREAD_U16RE(p); }
ZLX_LOCAL void awrite_u16le (uint8_t * p, uint16_t v) { ZLX_AWRITE_U16LE(p, v); }
ZLX_LOCAL void awrite_u16be (uint8_t * p, uint16_t v) { ZLX_AWRITE_U16BE(p, v); }
ZLX_LOCAL void awrite_u16ne (uint8_t * p, uint16_t v) { ZLX_AWRITE_U16NE(p, v); }
ZLX_LOCAL void awrite_u16re (uint8_t * p, uint16_t v) { ZLX_AWRITE_U16RE(p, v); }
ZLX_LOCAL void uwrite_u16le (uint8_t * p, uint16_t v) { ZLX_UWRITE_U16LE(p, v); }
ZLX_LOCAL void uwrite_u16be (uint8_t * p, uint16_t v) { ZLX_UWRITE_U16BE(p, v); }
ZLX_LOCAL void uwrite_u16ne (uint8_t * p, uint16_t v) { ZLX_UWRITE_U16NE(p, v); }
ZLX_LOCAL void uwrite_u16re (uint8_t * p, uint16_t v) { ZLX_UWRITE_U16RE(p, v); }

ZLX_LOCAL uint32_t aread_u32le (uint8_t const * p) { return ZLX_AREAD_U32LE(p); }
ZLX_LOCAL uint32_t aread_u32be (uint8_t const * p) { return ZLX_AREAD_U32BE(p); }
ZLX_LOCAL uint32_t aread_u32ne (uint8_t const * p) { return ZLX_AREAD_U32NE(p); }
ZLX_LOCAL uint32_t aread_u32re (uint8_t const * p) { return ZLX_AREAD_U32RE(p); }
ZLX_LOCAL uint32_t uread_u32le (uint8_t const * p) { return ZLX_UREAD_U32LE(p); }
ZLX_LOCAL uint32_t uread_u32be (uint8_t const * p) { return ZLX_UREAD_U32BE(p); }
ZLX_LOCAL uint32_t uread_u32ne (uint8_t const * p) { return ZLX_UREAD_U32NE(p); }
ZLX_LOCAL uint32_t uread_u32re (uint8_t const * p) { return ZLX_UREAD_U32RE(p); }
ZLX_LOCAL void awrite_u32le (uint8_t * p, uint32_t v) { ZLX_AWRITE_U32LE(p, v); }
ZLX_LOCAL void awrite_u32be (uint8_t * p, uint32_t v) { ZLX_AWRITE_U32BE(p, v); }
ZLX_LOCAL void awrite_u32ne (uint8_t * p, uint32_t v) { ZLX_AWRITE_U32NE(p, v); }
ZLX_LOCAL void awrite_u32re (uint8_t * p, uint32_t v) { ZLX_AWRITE_U32RE(p, v); }
ZLX_LOCAL void uwrite_u32le (uint8_t * p, uint32_t v) { ZLX_UWRITE_U32LE(p, v); }
ZLX_LOCAL void uwrite_u32be (uint8_t * p, uint32_t v) { ZLX_UWRITE_U32BE(p, v); }
ZLX_LOCAL void uwrite_u32ne (uint8_t * p, uint32_t v) { ZLX_UWRITE_U32NE(p, v); }
ZLX_LOCAL void uwrite_u32re (uint8_t * p, uint32_t v) { ZLX_UWRITE_U32RE(p, v); }

ZLX_LOCAL uint64_t aread_u64le (uint8_t const * p) { return ZLX_AREAD_U64LE(p); }
ZLX_LOCAL uint64_t aread_u64be (uint8_t const * p) { return ZLX_AREAD_U64BE(p); }
ZLX_LOCAL uint64_t aread_u64ne (uint8_t const * p) { return ZLX_AREAD_U64NE(p); }
ZLX_LOCAL uint64_t aread_u64re (uint8_t const * p) { return ZLX_AREAD_U64RE(p); }
ZLX_LOCAL uint64_t uread_u64le (uint8_t const * p) { return ZLX_UREAD_U64LE(p); }
ZLX_LOCAL uint64_t uread_u64be (uint8_t const * p) { return ZLX_UREAD_U64BE(p); }
ZLX_LOCAL uint64_t uread_u64ne (uint8_t const * p) { return ZLX_UREAD_U64NE(p); }
ZLX_LOCAL uint64_t uread_u64re (uint8_t const * p) { return ZLX_UREAD_U64RE(p); }
ZLX_LOCAL void awrite_u64le (uint8_t * p, uint64_t v) { ZLX_AWRITE_U64LE(p, v); }
ZLX_LOCAL void awrite_u64be (uint8_t * p, uint64_t v) { ZLX_AWRITE_U64BE(p, v); }
ZLX_LOCAL void awrite_u64ne (uint8_t * p, uint64_t v) { ZLX_AWRITE_U64NE(p, v); }
ZLX_LOCAL void awrite_u64re (uint8_t * p, uint64_t v) { ZLX_AWRITE_U64RE(p, v); }
ZLX_LOCAL void uwrite_u64le (uint8_t * p, uint64_t v) { ZLX_UWRITE_U64LE(p, v); }
ZLX_LOCAL void uwrite_u64be (uint8_t * p, uint64_t v) { ZLX_UWRITE_U64BE(p, v); }
ZLX_LOCAL void uwrite_u64ne (uint8_t * p, uint64_t v) { ZLX_UWRITE_U64NE(p, v); }
ZLX_LOCAL void uwrite_u64re (uint8_t * p, uint64_t v) { ZLX_UWRITE_U64RE(p, v); }

ZLX_LOCAL uint16_t seqread_u16le (uint8_t const * p) { return zlx_seqread_u16le(p); }
ZLX_LOCAL uint16_t seqread_u16be (uint8_t const * p) { return zlx_seqread_u16be(p); }
ZLX_LOCAL void seqwrite_u16le (uint8_t * p, uint16_t v) { zlx_seqwrite_u16le(p, v); }
ZLX_LOCAL void seqwrite_u16be (uint8_t * p, uint16_t v) { zlx_seqwrite_u16be(p, v); }
ZLX_LOCAL uint32_t seqread_u32le (uint8_t const * p) { return zlx_seqread_u32le(p); }
ZLX_LOCAL uint32_t seqread_u32be (uint8_t const * p) { return zlx_seqread_u32be(p); }
ZLX_LOCAL void seqwrite_u32le (uint8_t * p, uint32_t v) { zlx_seqwrite_u32le(p, v); }
ZLX_LOCAL void seqwrite_u32be (uint8_t * p, uint32_t v) { zlx_seqwrite_u32be(p, v); }
ZLX_LOCAL uint64_t seqread_u64le (uint8_t const * p) { return zlx_seqread_u64le(p); }
ZLX_LOCAL uint64_t seqread_u64be (uint8_t const * p) { return zlx_seqread_u64be(p); }
ZLX_LOCAL void seqwrite_u64le (uint8_t * p, uint64_t v) { zlx_seqwrite_u64le(p, v); }
ZLX_LOCAL void seqwrite_u64be (uint8_t * p, uint64_t v) { zlx_seqwrite_u64be(p, v); }

ZLX_LOCAL void u8a_zero (uint8_t * a, size_t n)
{
    zlx_u8a_zero(a, n);
}

ZLX_LOCAL unsigned int digit_to_char (unsigned int digit)
{
    return zlx_digit_to_char(digit);
}

ZLX_LOCAL int digit_from_char (uint32_t ch, uint_fast8_t radix)
{
    return zlx_digit_from_char(ch, radix);
}

#define IC(s, d) \
ZLX_LOCAL zlx_##d##_t s##_to_##d (zlx_##s##_t v) { return zlx_##s##_to_##d(v); } \
ZLX_LOCAL zlx_##d##_t d##_from_##s (zlx_##s##_t v) { return zlx_##d##_from_##s(v); } \
ZLX_LOCAL zlx_##d##_t cast_##s##_to_##d (zlx_##s##_t v) { return zlx_cast_##s##_to_##d(v); } \
ZLX_LOCAL zlx_##d##_t cast_##d##_from_##s (zlx_##s##_t v) { return zlx_cast_##d##_from_##s(v); } \
typedef int ic_tag_##s##_##_d

#define ICA(d) \
    IC(u8, d); IC(u16, d); IC(u32, d); IC(u64, d); \
    IC(usize, d); IC(size, d); IC(uptr, d); \
    IC(s8, d); IC(s16, d); IC(s32, d); IC(s64, d); \
    IC(ssize, d); IC(ptrdiff, d); IC(sptr, d)

ICA(u8); ICA(u16); ICA(u32); ICA(u64); ICA(uptr); ICA(usize); ICA(size);
ICA(s8); ICA(s16); ICA(s32); ICA(s64); ICA(sptr); ICA(ssize); ICA(ptrdiff);

