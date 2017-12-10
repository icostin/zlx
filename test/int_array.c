#include <string.h>
#include "../include/zlx/int/array.h"
#include "../include/zlx/memalloc/nop.h"

#include "test.h"

void * ZLX_CALL u8a_ma_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
)
{
    (void) old_size;
    (void) new_size;
    (void) ma;
    return new_size == 128 ? NULL : old_ptr;
}

zlx_ma_t u8a_ma = 
{
    u8a_ma_realloc,
    zlx_ma_nop_contains,
    zlx_ma_nop_info_set,
    zlx_ma_nop_check
};

ZLX_LOCAL void u8a_zero (uint8_t * a, size_t n);

/* u8a_test *****************************************************************/
int u8a_test (void)
{
    uint8_t a[8];
    uint8_t b[8];
    uint8_t * c;
    size_t n, m;

    memset(a, 'a', 8);
    T(memcmp(a, "aaaaaaaa", 8) == 0);
    u8a_zero(a, 7);
    T(memcmp(a, "\000\000\000\000\000\000\000a", 8) == 0);
    zlx_u8a_fill(a, 6, 'x');
    T(memcmp(a, "xxxxxx\000a", 8) == 0);

    memset(b, 'b', 8);
    zlx_u8a_copy(b, a + 1, 7);
    T(memcmp(b, "xxxxx\000ab", 8) == 0);

    memset(b, 'c', 8);
    c = zlx_u8a_zcopy(b + 1, a + 4);
    TE(c == b + 3, "c-b: %u", (int) (c - &b[0]));
    T(memcmp(b, "cxx\000cccc", 8) == 0);

    T(zlx_u8a_cmp(b, (uint8_t const *) "cxx\000cccc", 8) == 0);
    T(zlx_u8a_cmp(b, (uint8_t const *) "cxx\000ccdc", 8) < 0);
    T(zlx_u8a_cmp(b, (uint8_t const *) "cxx\000cccb", 8) > 0);
    T(zlx_u8a_zcmp(b, (uint8_t const *) "cxx") == 0);
    T(zlx_u8a_zcmp(b, (uint8_t const *) "cxxa") < 0);
    T(zlx_u8a_zcmp(b, (uint8_t const *) "cy") < 0);

    T(zlx_u8a_scan(b + 1, 'c') == b + 4);
    T(zlx_u8a_zlen(b) == 3);
    T(zlx_u8a_search(b + 1, b + 5, 'c') == b + 4);
    T(zlx_u8a_search(b + 1, b + 4, 'c') == b + 4);
    T(zlx_u8a_search(b + 1, b + 4, 'q') == b + 4);
    T(zlx_u8a_search(b + 1, b + 3, 'c') == b + 3);

    c = b; n = 5; m = 7;
    T(zlx_u8a_insert(&c, &n, &m, 4, (size_t) -4, &u8a_ma) == NULL);
    T(zlx_u8a_insert(&c, &n, &m, 4, PTRDIFF_MAX - 4, &u8a_ma) == NULL);
    T(zlx_u8a_insert(&c, &n, &m, 3, 3, &u8a_ma) == b + 3); T(n == 8); T(m == 8);
    T(memcmp(b, "cxx\000cc\000c", 8) == 0);
    T(zlx_u8a_insert(&c, &n, &m, 4, 100, &u8a_ma) == NULL);
    n = 2;
    T(zlx_u8a_insert(&c, &n, &m, 2, 2, &u8a_ma) == b + 2); T(n == 4); T(m == 8);


    return 0;
}

