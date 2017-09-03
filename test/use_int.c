#include "../include/zlx/int.h"

int use_int_test (void)
{
    size_t z = 1;
    ptrdiff_t d = 1;

    return !((ptrdiff_t) z == d);
}

