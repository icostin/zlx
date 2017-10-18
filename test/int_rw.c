#include <string.h>
#include "../include/zlx/lib.h"
#include "../include/zlx/int/ops.h"
#include "test.h"

ZLX_LOCAL uint8_t read_u8(uint8_t * p);
ZLX_LOCAL uint8_t aread_u8(uint8_t * p);
ZLX_LOCAL uint8_t uread_u8(uint8_t * p);
ZLX_LOCAL void write_u8(uint8_t * p, uint8_t v);
ZLX_LOCAL void awrite_u8(uint8_t * p, uint8_t v);
ZLX_LOCAL void uwrite_u8(uint8_t * p, uint8_t v);

ZLX_LOCAL uint8_t aread_u8le (uint8_t const * p);
ZLX_LOCAL uint8_t aread_u8be (uint8_t const * p);
ZLX_LOCAL uint8_t aread_u8ne (uint8_t const * p);
ZLX_LOCAL uint8_t aread_u8re (uint8_t const * p);
ZLX_LOCAL uint8_t uread_u8le (uint8_t const * p);
ZLX_LOCAL uint8_t uread_u8be (uint8_t const * p);
ZLX_LOCAL uint8_t uread_u8ne (uint8_t const * p);
ZLX_LOCAL uint8_t uread_u8re (uint8_t const * p);
ZLX_LOCAL void awrite_u8le (uint8_t * p, uint8_t v);
ZLX_LOCAL void awrite_u8be (uint8_t * p, uint8_t v);
ZLX_LOCAL void awrite_u8ne (uint8_t * p, uint8_t v);
ZLX_LOCAL void awrite_u8re (uint8_t * p, uint8_t v);
ZLX_LOCAL void uwrite_u8le (uint8_t * p, uint8_t v);
ZLX_LOCAL void uwrite_u8be (uint8_t * p, uint8_t v);
ZLX_LOCAL void uwrite_u8ne (uint8_t * p, uint8_t v);
ZLX_LOCAL void uwrite_u8re (uint8_t * p, uint8_t v);

ZLX_LOCAL uint16_t aread_u16le (uint8_t const * p);
ZLX_LOCAL uint16_t aread_u16be (uint8_t const * p);
ZLX_LOCAL uint16_t aread_u16ne (uint8_t const * p);
ZLX_LOCAL uint16_t aread_u16re (uint8_t const * p);
ZLX_LOCAL uint16_t uread_u16le (uint8_t const * p);
ZLX_LOCAL uint16_t uread_u16be (uint8_t const * p);
ZLX_LOCAL uint16_t uread_u16ne (uint8_t const * p);
ZLX_LOCAL uint16_t uread_u16re (uint8_t const * p);
ZLX_LOCAL void awrite_u16le (uint8_t * p, uint16_t v);
ZLX_LOCAL void awrite_u16be (uint8_t * p, uint16_t v);
ZLX_LOCAL void awrite_u16ne (uint8_t * p, uint16_t v);
ZLX_LOCAL void awrite_u16re (uint8_t * p, uint16_t v);
ZLX_LOCAL void uwrite_u16le (uint8_t * p, uint16_t v);
ZLX_LOCAL void uwrite_u16be (uint8_t * p, uint16_t v);
ZLX_LOCAL void uwrite_u16ne (uint8_t * p, uint16_t v);
ZLX_LOCAL void uwrite_u16re (uint8_t * p, uint16_t v);

ZLX_LOCAL uint32_t aread_u32le (uint8_t const * p);
ZLX_LOCAL uint32_t aread_u32be (uint8_t const * p);
ZLX_LOCAL uint32_t aread_u32ne (uint8_t const * p);
ZLX_LOCAL uint32_t aread_u32re (uint8_t const * p);
ZLX_LOCAL uint32_t uread_u32le (uint8_t const * p);
ZLX_LOCAL uint32_t uread_u32be (uint8_t const * p);
ZLX_LOCAL uint32_t uread_u32ne (uint8_t const * p);
ZLX_LOCAL uint32_t uread_u32re (uint8_t const * p);
ZLX_LOCAL void awrite_u32le (uint8_t * p, uint32_t v);
ZLX_LOCAL void awrite_u32be (uint8_t * p, uint32_t v);
ZLX_LOCAL void awrite_u32ne (uint8_t * p, uint32_t v);
ZLX_LOCAL void awrite_u32re (uint8_t * p, uint32_t v);
ZLX_LOCAL void uwrite_u32le (uint8_t * p, uint32_t v);
ZLX_LOCAL void uwrite_u32be (uint8_t * p, uint32_t v);
ZLX_LOCAL void uwrite_u32ne (uint8_t * p, uint32_t v);
ZLX_LOCAL void uwrite_u32re (uint8_t * p, uint32_t v);

ZLX_LOCAL uint64_t aread_u64le (uint8_t const * p);
ZLX_LOCAL uint64_t aread_u64be (uint8_t const * p);
ZLX_LOCAL uint64_t aread_u64ne (uint8_t const * p);
ZLX_LOCAL uint64_t aread_u64re (uint8_t const * p);
ZLX_LOCAL uint64_t uread_u64le (uint8_t const * p);
ZLX_LOCAL uint64_t uread_u64be (uint8_t const * p);
ZLX_LOCAL uint64_t uread_u64ne (uint8_t const * p);
ZLX_LOCAL uint64_t uread_u64re (uint8_t const * p);
ZLX_LOCAL void awrite_u64le (uint8_t * p, uint64_t v);
ZLX_LOCAL void awrite_u64be (uint8_t * p, uint64_t v);
ZLX_LOCAL void awrite_u64ne (uint8_t * p, uint64_t v);
ZLX_LOCAL void awrite_u64re (uint8_t * p, uint64_t v);
ZLX_LOCAL void uwrite_u64le (uint8_t * p, uint64_t v);
ZLX_LOCAL void uwrite_u64be (uint8_t * p, uint64_t v);
ZLX_LOCAL void uwrite_u64ne (uint8_t * p, uint64_t v);
ZLX_LOCAL void uwrite_u64re (uint8_t * p, uint64_t v);

ZLX_LOCAL uint16_t seqread_u16le (uint8_t const * p);
ZLX_LOCAL uint16_t seqread_u16be (uint8_t const * p);
ZLX_LOCAL void seqwrite_u16le (uint8_t * p, uint16_t v);
ZLX_LOCAL void seqwrite_u16be (uint8_t * p, uint16_t v);
ZLX_LOCAL uint32_t seqread_u32le (uint8_t const * p);
ZLX_LOCAL uint32_t seqread_u32be (uint8_t const * p);
ZLX_LOCAL void seqwrite_u32le (uint8_t * p, uint32_t v);
ZLX_LOCAL void seqwrite_u32be (uint8_t * p, uint32_t v);
ZLX_LOCAL uint64_t seqread_u64le (uint8_t const * p);
ZLX_LOCAL uint64_t seqread_u64be (uint8_t const * p);
ZLX_LOCAL void seqwrite_u64le (uint8_t * p, uint64_t v);
ZLX_LOCAL void seqwrite_u64be (uint8_t * p, uint64_t v);

#if ZLX_LITTLE_ENDIAN
# define EV(le, be) le
#else
# define EV(le, be) be
#endif

int int_rw_test (void)
{
    static uint8_t b[] = { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                           0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
    uint8_t o[16];

    T(read_u8(b) == 0x12);
    T(aread_u8(b + 1) == 0x34);
    T(uread_u8(b + 2) == 0x56);
    write_u8(o, 0xAA); T(o[0] == 0xAA);
    awrite_u8(o, 0xBB); T(o[0] == 0xBB);
    uwrite_u8(o, 0xCC); T(o[0] == 0xCC);

    T(aread_u8le(b) == 0x12);
    T(aread_u8be(b) == 0x12);
    T(aread_u8ne(b) == 0x12);
    T(aread_u8re(b) == 0x12);
    T(uread_u8le(b) == 0x12);
    T(uread_u8be(b) == 0x12);
    T(uread_u8ne(b) == 0x12);
    T(uread_u8re(b) == 0x12);
    awrite_u8le(o, 0x01); T(o[0] == 0x01);
    awrite_u8be(o, 0x12); T(o[0] == 0x12);
    awrite_u8ne(o, 0x23); T(o[0] == 0x23);
    awrite_u8re(o, 0x34); T(o[0] == 0x34);
    uwrite_u8le(o, 0x01); T(o[0] == 0x01);
    uwrite_u8be(o, 0x12); T(o[0] == 0x12);
    uwrite_u8ne(o, 0x23); T(o[0] == 0x23);
    uwrite_u8re(o, 0x34); T(o[0] == 0x34);

#define FF() memset(o, 0xFF, sizeof(o))
    T(aread_u16le(b) == 0x3412);
    T(aread_u16be(b) == 0x1234);
    T(aread_u16ne(b) == EV(0x3412, 0x1234));
    T(aread_u16re(b) == EV(0x1234, 0x3412));
    T(uread_u16le(b + 7) == 0x11F0);
    T(uread_u16be(b + 7) == 0xF011);
    T(uread_u16ne(b + 7) == EV(0x11F0, 0xF011));
    T(uread_u16re(b + 7) == EV(0xF011, 0x11F0));
    T(seqread_u16le(b + 7) == 0x11F0);
    T(seqread_u16be(b + 7) == 0xF011);
    FF(); awrite_u16le(o, 0xAABB); T(!memcmp(o, "\xBB\xAA", 2));
    FF(); awrite_u16be(o, 0xAABB); T(!memcmp(o, "\xAA\xBB", 2));
    FF(); awrite_u16ne(o, 0xAABB); T(!memcmp(o, EV("\xBB\xAA", "\xAA\xBB"), 2));
    FF(); awrite_u16re(o, 0xAABB); T(!memcmp(o, EV("\xAA\xBB", "\xBB\xAA"), 2));
    FF(); uwrite_u16le(o + 1, 0xAABB); T(!memcmp(o + 1, "\xBB\xAA", 2));
    FF(); uwrite_u16be(o + 1, 0xAABB); T(!memcmp(o + 1, "\xAA\xBB", 2));
    FF(); uwrite_u16ne(o + 1, 0xAABB); T(!memcmp(o + 1, EV("\xBB\xAA", "\xAA\xBB"), 2));
    FF(); uwrite_u16re(o + 1, 0xAABB); T(!memcmp(o + 1, EV("\xAA\xBB", "\xBB\xAA"), 2));
    FF(); seqwrite_u16le(o + 1, 0xAABB); T(!memcmp(o + 1, "\xBB\xAA", 2));
    FF(); seqwrite_u16be(o + 1, 0xAABB); T(!memcmp(o + 1, "\xAA\xBB", 2));

    T(aread_u32le(b) == 0x78563412);
    T(aread_u32be(b) == 0x12345678);
    T(aread_u32ne(b) == EV(0x78563412, 0x12345678));
    T(aread_u32re(b) == EV(0x12345678, 0x78563412));
    T(uread_u32le(b + 7) == 0x332211F0);
    T(uread_u32be(b + 7) == 0xF0112233);
    T(uread_u32ne(b + 7) == EV(0x332211F0, 0xF0112233));
    T(uread_u32re(b + 7) == EV(0xF0112233, 0x332211F0));
    T(seqread_u32le(b + 7) == 0x332211F0);
    T(seqread_u32be(b + 7) == 0xF0112233);
    FF(); awrite_u32le(o, 0xAABBCCDD); T(!memcmp(o, "\xDD\xCC\xBB\xAA", 4));
    FF(); awrite_u32be(o, 0xAABBCCDD); T(!memcmp(o, "\xAA\xBB\xCC\xDD", 4));
    FF(); awrite_u32ne(o, 0xAABBCCDD); T(!memcmp(o, EV("\xDD\xCC\xBB\xAA", "\xAA\xBB\xCC\xDD"), 4));
    FF(); awrite_u32re(o, 0xAABBCCDD); T(!memcmp(o, EV("\xAA\xBB\xCC\xDD", "\xDD\xCC\xBB\xAA"), 4));
    FF(); uwrite_u32le(o + 1, 0xAABBCCDD); T(!memcmp(o + 1, "\xDD\xCC\xBB\xAA", 4));
    FF(); uwrite_u32be(o + 1, 0xAABBCCDD); T(!memcmp(o + 1, "\xAA\xBB\xCC\xDD", 4));
    FF(); uwrite_u32ne(o + 1, 0xAABBCCDD); T(!memcmp(o + 1, EV("\xDD\xCC\xBB\xAA", "\xAA\xBB\xCC\xDD"), 4));
    FF(); uwrite_u32re(o + 1, 0xAABBCCDD); T(!memcmp(o + 1, EV("\xAA\xBB\xCC\xDD", "\xDD\xCC\xBB\xAA"), 4));
    FF(); seqwrite_u32le(o + 1, 0xAABBCCDD); T(!memcmp(o + 1, "\xDD\xCC\xBB\xAA", 4));
    FF(); seqwrite_u32be(o + 1, 0xAABBCCDD); T(!memcmp(o + 1, "\xAA\xBB\xCC\xDD", 4));

    T(aread_u64le(b) == UINT64_C(0xF0DEBC9A78563412));
    T(aread_u64be(b) == UINT64_C(0x123456789ABCDEF0));
    T(aread_u64ne(b) == EV(UINT64_C(0xF0DEBC9A78563412), UINT64_C(0x123456789ABCDEF0)));
    T(aread_u64re(b) == EV(UINT64_C(0x123456789ABCDEF0), UINT64_C(0xF0DEBC9A78563412)));
    T(uread_u64le(b + 7) == UINT64_C(0x77665544332211F0));
    T(uread_u64be(b + 7) == UINT64_C(0xF011223344556677));
    T(uread_u64ne(b + 7) == EV(UINT64_C(0x77665544332211F0), UINT64_C(0xF011223344556677)));
    T(uread_u64re(b + 7) == EV(UINT64_C(0xF011223344556677), UINT64_C(0x77665544332211F0)));
    T(seqread_u64le(b + 7) == UINT64_C(0x77665544332211F0));
    T(seqread_u64be(b + 7) == UINT64_C(0xF011223344556677));
    FF(); awrite_u64le(o, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o, "\x44\x33\x22\x11\xDD\xCC\xBB\xAA", 8));
    FF(); awrite_u64be(o, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o, "\xAA\xBB\xCC\xDD\x11\x22\x33\x44", 8));
    FF(); awrite_u64ne(o, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o, EV("\x44\x33\x22\x11\xDD\xCC\xBB\xAA", "\xAA\xBB\xCC\xDD\x11\x22\x33\x44"), 8));
    FF(); awrite_u64re(o, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o, EV("\xAA\xBB\xCC\xDD\x11\x22\x33\x44", "\x44\x33\x22\x11\xDD\xCC\xBB\xAA"), 8));
    FF(); uwrite_u64le(o + 1, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o + 1, "\x44\x33\x22\x11\xDD\xCC\xBB\xAA", 8));
    FF(); uwrite_u64be(o + 1, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o + 1, "\xAA\xBB\xCC\xDD\x11\x22\x33\x44", 8));
    FF(); uwrite_u64ne(o + 1, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o + 1, EV("\x44\x33\x22\x11\xDD\xCC\xBB\xAA", "\xAA\xBB\xCC\xDD\x11\x22\x33\x44"), 8));
    FF(); uwrite_u64re(o + 1, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o + 1, EV("\xAA\xBB\xCC\xDD\x11\x22\x33\x44", "\x44\x33\x22\x11\xDD\xCC\xBB\xAA"), 8));
    FF(); seqwrite_u64le(o + 1, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o + 1, "\x44\x33\x22\x11\xDD\xCC\xBB\xAA", 8));
    FF(); seqwrite_u64be(o + 1, UINT64_C(0xAABBCCDD11223344)); T(!memcmp(o + 1, "\xAA\xBB\xCC\xDD\x11\x22\x33\x44", 8));

    return 0;
}

