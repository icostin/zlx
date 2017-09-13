#include "../include/zlx/base.h"
#include "../include/zlx/int.h"


ZLX_API char const * const zlx_lib_name = "zlx"
#if defined(ZLX_STATIC)
    "-static"
#elif defined(ZLX_DYNAMIC)
    "-dynamic"
#else
#error weird build
#endif
#if _DEBUG
    "-debug"
#elif NDEBUG
    "-release"
#else
    "-checked"
#endif

#if ZLX_IA32
    "-ia32"
#elif ZLX_AMD64
    "-amd64"
#else
    "-some_arch"
#endif
    ;

#if defined(ZLX_ABI_MS)

uint32_t __stdcall DllMainCRTStartup
(
    void * instance,
    uint32_t reason,
    void * reserved
)
{
    (void) instance;
    (void) reason;
    (void) reserved;
    return 1;
}

#endif

