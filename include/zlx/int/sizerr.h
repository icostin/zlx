#ifndef _ZLX_SIZERR_H
#define _ZLX_SIZERR_H

#include "type.h"
#include "../assert.h"

#define ZLX_SIZERR_DECL(sizerr_type, prefix, error_enum_type) \
    struct prefix##_struct { ptrdiff_t _raw; }; \
    ZLX_INLINE struct prefix##_struct prefix##_from_size (size_t size) { \
        struct prefix##_struct se = { (ptrdiff_t) size }; \
        ZLX_ASSERT(se._raw >= 0); \
        return se; \
    } \
    ZLX_INLINE struct prefix##_struct prefix##_from_error (error_enum_type error) { \
        struct prefix##_struct se = { -(ptrdiff_t) error }; \
        ZLX_ASSERT(se._raw < 0); \
        return se; \
    } \
    ZLX_INLINE int prefix##_is_size (struct prefix##_struct se) { \
        return (se._raw >= 0); \
    } \
    ZLX_INLINE int prefix##_is_error (struct prefix##_struct se) { \
        return (se._raw < 0); \
    } \
    ZLX_INLINE size_t prefix##_to_size (struct prefix##_struct se) { \
        ZLX_ASSERT(prefix##_is_size(se)); \
        return (size_t) (se._raw); \
    } \
    ZLX_INLINE error_enum_type prefix##_to_error (struct prefix##_struct se) { \
        ZLX_ASSERT(prefix##_is_error(se)); \
        return (error_enum_type) (-se._raw); \
    } \
    typedef struct prefix##_struct sizerr_type

#endif
