#include "../include/zlx.h"

ZLX_API char const * const zlx_lib_id = "zlx"
#if ZLX_IA32
    "-ia32"
#elif ZLX_AMD64
    "-amd64"
#else
    "-some_arch"
#endif
#if ZLX_ABI_MS
    "-ms_abi"
#elif ZLX_ABI_SYSV
    "-sysv_abi"
#else
    "-some_abi"
#endif
#if ZLXOPT_DEBUG
    "-debug"
#elif ZLXOPT_ASSERT
    "-checked"
#else
    "-release"
#endif
#if ZLXOPT_DYNAMIC
    "-dynamic"
#elif ZLXOPT_STATIC
    "-static"
#else
    "-some_lib_model"
#endif
;
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
