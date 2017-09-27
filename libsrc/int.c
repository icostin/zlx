#include "../include/zlx/int.h"

ZLX_API uint8_t zlxni_u8_log2_ceil (uint8_t x)
{
    return zlx_u8_log2_ceil(x);
}

ZLX_API uint8_t zlxni_u16_log2_ceil (uint16_t x)
{
    return zlx_u16_log2_ceil(x);
}

ZLX_API uint8_t zlxni_u32_log2_ceil (uint32_t x)
{
    return zlx_u32_log2_ceil(x);
}

ZLX_API uint8_t zlxni_u64_log2_ceil (uint64_t x)
{
    return zlx_u64_log2_ceil(x);
}

ZLX_API uint8_t zlxni_size_log2_ceil (size_t x)
{
    return zlx_size_log2_ceil(x);
}

ZLX_API uint8_t zlxni_uptr_log2_ceil (uintptr_t x)
{
    return zlx_uptr_log2_ceil(x);
}

ZLX_API uint16_t zlxni_seqbswap16 (uint16_t v)
{
    return zlx_seqbswap16(v);
}

ZLX_API uint32_t zlxni_seqbswap32 (uint32_t v)
{
    return zlx_seqbswap32(v);
}

ZLX_API uint64_t zlxni_seqbswap64 (uint64_t v)
{
    return zlx_seqbswap64(v);
}

ZLX_API uint16_t zlxni_seqread_u16le (void const * p)
{
    return zlx_seqread_u16le(p);
}

ZLX_API uint32_t zlxni_seqread_u32le (void const * p)
{
    return zlx_seqread_u32le(p);
}

ZLX_API uint64_t zlxni_seqread_u64le (void const * p)
{
    return zlx_seqread_u64le(p);
}

ZLX_API uint16_t zlxni_seqread_u16be (void const * p)
{
    return zlx_seqread_u16be(p);
}

ZLX_API uint32_t zlxni_seqread_u32be (void const * p)
{
    return zlx_seqread_u32be(p);
}

ZLX_API uint64_t zlxni_seqread_u64be (void const * p)
{
    return zlx_seqread_u64be(p);
}

ZLX_API void zlxni_seqwrite_u16le (void * p, uint16_t v)
{
    zlx_seqwrite_u16le(p, v);
}

ZLX_API void zlxni_seqwrite_u32le (void * p, uint32_t v)
{
    zlx_seqwrite_u32le(p, v);
}

ZLX_API void zlxni_seqwrite_u64le (void * p, uint64_t v)
{
    zlx_seqwrite_u64le(p, v);
}

ZLX_API void zlxni_seqwrite_u16be (void * p, uint16_t v)
{
    zlx_seqwrite_u16be(p, v);
}

ZLX_API void zlxni_seqwrite_u32be (void * p, uint32_t v)
{
    zlx_seqwrite_u32be(p, v);
}

ZLX_API void zlxni_seqwrite_u64be (void * p, uint64_t v)
{
    zlx_seqwrite_u64be(p, v);
}

