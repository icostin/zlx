#include <zlx/assert.h>

int x = 40;
int y = 2;

int assert_test (void)
{
    ZLX_ASSERT(x + y != 42);
    return 0;
}

