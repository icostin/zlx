#include <string.h>
#include "../include/zlx/stdarray.h"
#include "test.h"

/* stdarray_test ************************************************************/
int stdarray_test (void)
{
    uint8_t d[8];
    static uint8_t s[] = { 0x12, 0x34, 0x56 };

    memset(d, 0, sizeof(d));
    zlx_u8a_copy(d, s, sizeof(s));
    T(!memcmp(d, s, sizeof(s)));

    return 0;
}

