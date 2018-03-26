#undef _DEBUG
#undef NDEBUG
#define _DEBUG 1
#include "../include/zlx/debug.h"

int debug_test (void)
{
    ZLX_DMSG("ding");
    ZLX_DFMT("dong\n");
    return 0;
}

