#define ZLXOPT_USE_NON_INLINED_API
#undef _DEBUG
#undef _CHECKED
#undef NDEBUG
#define NDEBUG

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

void ZLX_CALL test_check
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


/* memalloc_release_test ****************************************************/
int memalloc_release_test (void)
{
    T(zlx_alloc(&test_ma, 123, "cucu") == (void *) (uintptr_t) 123);
    T(zlx_realloc(&test_ma, (void *) (uintptr_t) 123, 123, 456) == (void *) (uintptr_t) 456);
    check_ptr = (void *) (uintptr_t) 789;
    check_size = 234;
    zlx_free(&test_ma, (void *) (uintptr_t) 456, 456);
    T(check_ptr == (void *) (uintptr_t) 789);
    T(check_size == 234);
    return 0;
}

