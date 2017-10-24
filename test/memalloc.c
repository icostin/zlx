/* force asserts on */
#undef ZLXOPT_ASSERT
#define ZLXOPT_ASSERT 1

#include <string.h>
#include "../include/zlx/assert.h"
#include "../include/zlx/memalloc/interface.h"
#include "test.h"

typedef struct test_ma test_ma_t;
struct test_ma
{
    zlx_ma_t ma;
    void * last_ptr;
    size_t last_size;
    char const * last_src;
    unsigned int last_line;
    char const * last_func;
    char const * last_info;
};

void * cov_dbg_alloc 
(
    zlx_ma_t * ZLX_RESTRICT ma,
    size_t size,
    char const * info
);

void * cov_dbg_realloc 
(
    zlx_ma_t * ZLX_RESTRICT ma, 
    void * old_ptr,
    size_t old_size,
    size_t new_size
);

void cov_dbg_free
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size
);

void * ZLX_CALL tma_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
);

void ZLX_CALL tma_info_set
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    char const * src,
    unsigned int line,
    char const * func,
    char const * info
);

void ZLX_CALL tma_check
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size,
    char const * src,
    unsigned int line,
    char const * func
);

int memalloc_test (void)
{
    test_ma_t tma;
    memset(&tma, 0, sizeof(tma));
    tma.ma.realloc = tma_realloc;
    tma.ma.info_set = tma_info_set;
    tma.ma.check = tma_check;

    T((intptr_t) cov_dbg_alloc(&tma.ma, 123, "123") == 123);
    T(tma.last_size == 123);
    T(!strcmp(tma.last_src, "test/memalloc_debug_cov.c")); T(tma.last_line > 0);

    T((intptr_t) cov_dbg_realloc(&tma.ma, (void *) (intptr_t) 123, 123, 234) == 234);
    T(tma.last_size == 234);
    T(!strcmp(tma.last_src, "test/memalloc_debug_cov.c")); T(tma.last_line > 0);

    cov_dbg_free(&tma.ma, (void *) (intptr_t) 234, 234);
    puts("grr");
    T(tma.last_size == 0);
    T(tma.last_src == NULL); T(tma.last_line == 0);

    return 0;
}

void * ZLX_CALL tma_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
)
{
    test_ma_t * tma = (test_ma_t *) ma;
    ZLX_ASSERT((intptr_t) old_ptr == (intptr_t) old_size);
    ZLX_ASSERT(old_ptr == tma->last_ptr);
    tma->last_ptr = (void *) (intptr_t) new_size;
    tma->last_size = new_size;
    tma->last_src = NULL;
    tma->last_line = 0;
    tma->last_func = NULL;
    puts("tma_realloc");
    return (void *) (intptr_t) new_size;
}

void ZLX_CALL tma_info_set
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    char const * src,
    unsigned int line,
    char const * func,
    char const * info
)
{
    test_ma_t * tma = (test_ma_t *) ma;
    ZLX_ASSERT(tma->last_ptr == ptr);
    tma->last_src = src;
    tma->last_line = line;
    tma->last_func = func;
    tma->last_info = info;
}


void ZLX_CALL tma_check
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size,
    char const * src,
    unsigned int line,
    char const * func
)
{
    test_ma_t * tma = (test_ma_t *) ma;
    ZLX_ASSERT((intptr_t) ptr == (intptr_t) size);
    ZLX_ASSERT(ptr == tma->last_ptr);
    (void) src;
    (void) line;
    (void) func;
    puts("check");
}


