#define ZLXOPT_USE_NON_INLINED_API
#undef _DEBUG
#undef _CHECKED
#undef NDEBUG
#define _CHECKED

#include "../include/zlx/memalloc.h"
#include "test.h"

static void * check_ptr;
static size_t check_size;

static void * ZLX_CALL test_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
)
{
    (void) old_ptr;
    (void) old_size;
    (void) new_size;
    (void) ma;
    if (new_size == 12345 * 4) return NULL;
    return (void *) ((uintptr_t) old_ptr - old_size + new_size);
}

static void ZLX_CALL test_info_set
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    char const * src,
    unsigned int line,
    char const * func,
    char const * info
)
{
    (void) ma;
    (void) ptr;
    (void) src;
    (void) line;
    (void) func;
    (void) info;
}

static void ZLX_CALL test_check
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size,
    char const * src,
    unsigned int line,
    char const * func
)
{
    (void) ma;
    (void) ptr;
    (void) src;
    (void) line;
    (void) func;
    (void) size;
    check_ptr = ptr;
    check_size = size;
}

static zlx_ma_t test_ma =
{
    test_realloc,
    test_info_set,
    test_check
};

int memalloc_checked_test (void)
{
    uint32_t * p;
    size_t n, xlarge;
    int r;

    xlarge = (size_t) (UINT64_C(1) << (ZLX_BITS - 2));

    check_ptr = (void *) (uintptr_t) 789;
    check_size = 678;

    T(zlx_alloc(&test_ma, 123, "cucu") == (void *) (uintptr_t) 123);
    T(check_ptr == (void *) (uintptr_t) 789); T(check_size == 678);

    T(zlx_realloc(&test_ma, (void *) (uintptr_t) 123, 123, 456) 
      == (void *) (uintptr_t) 456);
    T(check_ptr == (void *) (uintptr_t) 123); T(check_size == 123);

    zlx_free(&test_ma, (void *) (uintptr_t) 456, 456);
    T(check_ptr == (void *) (uintptr_t) 456); T(check_size = 456);

    r = zlx_array_alloc(&test_ma, p, n, 12345, "boo");
    T(r);

    T(zlx_array_alloc(&test_ma, p, n, 25, "bla") == 0);
    T(p == (uint32_t *) (uintptr_t) 100); T(n == 25);

    T(zlx_array_realloc(&test_ma, p, n, 30) == 0);
    T(p == (uint32_t *) (uintptr_t) 120); T(n == 30);

    T(zlx_array_realloc(&test_ma, p, n, xlarge) == 1);
    TE(p == (uint32_t *) (uintptr_t) 120, "p=%p", (void *) p); T(n == 30);

    T(zlx_array_realloc(&test_ma, p, n, 12345) == 1);
    T(p == (uint32_t *) (uintptr_t) 120); T(n == 30);
    return 0;
}

