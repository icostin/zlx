#include "../include/zlx/obstream/nop.h"

int nop_obstream_test (void)
{
    return zlx_obstream_write(&zlx_nop_obstream, (uint8_t const *) "bla bla", 7) != 7;
}

