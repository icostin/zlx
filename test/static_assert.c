#include "../include/zlx/language.h"
#include "../include/zlx/arch.h"
#include "../include/zlx/int/type.h"

ZLX_STATIC_ASSERT(ZLX_BITS >= 32); ZLX_STATIC_ASSERT(ZLX_BITS <= 64);
ZLX_STATIC_ASSERT(sizeof(size_t) * 8 == ZLX_BITS);

