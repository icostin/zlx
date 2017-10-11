#ifndef _ZLX_TEST_H
#define _ZLX_TEST_H

#include <stdio.h>

#define TE(cond, ...) \
    if ((cond)) ; \
    else { \
        fprintf(stderr, "%s:%u: test failed: %s.", \
                __FILE__, __LINE__, #cond); \
        fprintf(stderr, " " __VA_ARGS__); \
        fprintf(stderr, "\n"); \
        fflush(stderr); \
        return __LINE__; \
    }

#define T(cond) TE(cond, "")


#endif
