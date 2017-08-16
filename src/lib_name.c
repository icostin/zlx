#include "../include/zlx/base.h"

char const * const zlx_lib_name = "zlx"
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
