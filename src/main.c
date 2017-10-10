#include "../include/zlx.h"

ZLX_API char const * const zlx_lib_id = "zlx";
char const * zlx_lib_compiler_name = ZLX_COMPILER_NAME;

#if defined(_WIN32) && defined(ZLXOPT_DYNAMIC)
unsigned int __stdcall DllMainCRTStartup
(
    void * instance,
    unsigned int reason,
    void * reserved
)
{
    (void) instance;
    (void) reason;
    (void) reserved;
    return 1;
}
#endif
