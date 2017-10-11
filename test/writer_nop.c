#include "../include/zlx/writer/nop.h"
#include "test.h"

int nop_writer_test (void)
{
    int x;
    uint8_t b;
    T(zlx_nop_writer(NULL, &b, 0) == 0);
    T(zlx_nop_writer(NULL, &b, 1) == 1);
    T(zlx_nop_writer(&x, &b, (size_t) (ptrdiff_t) -1) 
      == (size_t) (ptrdiff_t) -1);
    return 0;
}

