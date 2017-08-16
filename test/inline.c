#include <zlx/base.h>

ZLX_INLINE int afunc (int x)
{
    return x + 1;
}

ZLX_FORCE_INLINE char bfunc (char x)
{
    return x + 1;
}

int inline_test (void)
{
    return afunc(65) != bfunc('A');
}

