#ifndef _ZLX_DEBUG_H
#define _ZLX_DEBUG_H

#include "language.h"
#include "log.h"

#if ZLXOPT_DEBUG
#define ZLX_DFMT(...) (zlx_log_fmt(zlx_default_log, ZLX_LL_DEBUG, __VA_ARGS__))
static char const _zlx_dmsg_tail[] = "????????";
#define _ZLX_DMSG(fmt, ...) \
    (zlx_log_fmt(zlx_default_log, ZLX_LL_DEBUG, "$s:$i:$s(): " fmt "\n", \
                 __FILE__, __LINE__, __func__, __VA_ARGS__))
#define ZLX_DMSG(...) \
    (ZLX_VA_EXPAND(_ZLX_DMSG(__VA_ARGS__, _zlx_dmsg_tail, _zlx_dmsg_tail, _zlx_dmsg_tail)))
#else
#define ZLX_DFMT(...) ((void) 0)
#define ZLX_DMSG(...) ((void) 0)
#endif


#endif /* _ZLX_DEBUG_H */
