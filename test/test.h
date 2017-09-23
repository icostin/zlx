#ifndef _ZLX_TEST_H
#define _ZLX_TEST_H

#define TE(cond, ...) \
    if ((cond)) ; \
    else { \
        fprintf(stderr, "%s:%u: test failed: %s. ", \
                __FILE__, __LINE__, #cond); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
        fflush(stderr); \
        return __LINE__; \
    }

#endif
