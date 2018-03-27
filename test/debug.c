#undef _DEBUG
#undef NDEBUG
#define _DEBUG 1
#include "../include/zlx/debug.h"

int debug_test (void)
{
    zlx_log_t * l = zlx_default_log;
    zlx_default_log = &zlx_nop_log;
    ZLX_DMSG("ding");
    ZLX_DMSG("dong $i", 1);
    ZLX_DFMT("ding\n");
    ZLX_DFMT("ding $i\n", 2);
    zlx_default_log = l;
    return 0;
}

