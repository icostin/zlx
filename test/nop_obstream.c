#include <zlx/obstream.h>

int nop_obstream_test (void)
{
    return zlx_nop_obstream.type->write(&zlx_nop_obstream, "bla bla", 7) != 7;
}

