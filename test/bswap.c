#include "../include/zlx/lib.h"
#include "../include/zlx/int/ops.h"
#include "test.h"

uint16_t seqbswap16 (uint16_t v);
uint32_t seqbswap32 (uint32_t v);
uint64_t seqbswap64 (uint64_t v);

uint16_t bswap16 (uint16_t v);
uint32_t bswap32 (uint32_t v);
uint64_t bswap64 (uint64_t v);

int bswap_test (void)
{
    T(bswap16(0xABCD) == 0xCDAB);
    T(bswap32(0x89ABCDEF) == 0xEFCDAB89);
    T(bswap64(UINT64_C(0x1122334455667788)) == UINT64_C(0x8877665544332211));

    T(seqbswap16(0xABCD) == 0xCDAB);
    T(seqbswap32(0x89ABCDEF) == 0xEFCDAB89);
    T(seqbswap64(UINT64_C(0x1122334455667788)) == UINT64_C(0x8877665544332211));

    return 0;
}

