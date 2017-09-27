#include <string.h>
#include "../include/zlx/int.h"
#include "test.h"

int seqbswap_test (void)
{
    T(zlxni_seqbswap16(0xABCD) == 0xCDAB);
    T(zlxni_seqbswap32(0x12345678) == 0x78563412);
    T(zlxni_seqbswap64(UINT64_C(0x123456789ABCDEF0)) == UINT64_C(0xF0DEBC9A78563412));
    return 0;
}

int seqrw_test (void)
{
    static uint8_t s[] = { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 };
    uint8_t d[9];

    T(zlxni_seqread_u16le(s) == 0x3412);
    T(zlxni_seqread_u16be(s) == 0x1234);
    T(zlxni_seqread_u32le(s) == 0x78563412);
    T(zlxni_seqread_u32be(s) == 0x12345678);
    T(zlxni_seqread_u64le(s) == UINT64_C(0xF0DEBC9A78563412));
    T(zlxni_seqread_u64be(s) == UINT64_C(0x123456789ABCDEF0));

    memset(d, 0xAA, 9); zlxni_seqwrite_u16le(d, 0x3412);
    T(!memcmp(d, s, 2)); T(d[2] == 0xAA);
    memset(d, 0xAA, 9); zlxni_seqwrite_u16be(d, 0x1234);
    T(!memcmp(d, s, 2)); T(d[2] == 0xAA);

    memset(d, 0xAA, 9); zlxni_seqwrite_u32le(d, 0x78563412);
    T(!memcmp(d, s, 4)); T(d[4] == 0xAA);
    memset(d, 0xAA, 9); zlxni_seqwrite_u32be(d, 0x12345678);
    T(!memcmp(d, s, 4)); T(d[4] == 0xAA);

    memset(d, 0xAA, 9); zlxni_seqwrite_u64le(d, UINT64_C(0xF0DEBC9A78563412));
    T(!memcmp(d, s, 8)); T(d[8] == 0xAA);
    memset(d, 0xAA, 9); zlxni_seqwrite_u64be(d, UINT64_C(0x123456789ABCDEF0));
    T(!memcmp(d, s, 8)); T(d[8] == 0xAA);

    return 0;
}

