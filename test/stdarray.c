#include <string.h>
#include "../include/zlx/stdarray.h"
#include "test.h"

/* stdarray_test ************************************************************/
int stdarray_test (void)
{
    uint8_t d[8];
    static uint8_t s[] = { 0x12, 0x34, 0x56, 0, 0xCD };
    static uint8_t sz[] = { 0x12, 0x34, 0x56, 0, 0xFE };
    uint8_t * dr;

    memset(d, 0, sizeof(d));
    zlx_u8a_copy(d, s, 3);
    T(!memcmp(d, s, 3));

    memset(d, 0xFE, sizeof(d));
    dr = zlx_u8a_zcopy(d, s);
    TE(dr == &d[3], "dr-d: %u", (int) (dr - d));
    T(!memcmp(d, sz, sizeof(sz)));

    return 0;
}

