#include <string.h>
#include "../include/zlx/stdarray.h"
#include "test.h"

/* test_realloc *************************************************************/
void * ZLX_CALL test_realloc
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
    if (new_size > 8) return NULL;
    return old_ptr;
}

/* stdarray_test ************************************************************/
int stdarray_test (void)
{
    uint8_t d[8];
    static uint8_t s[] = { 0x12, 0x34, 0x56, 0, 0xCD };
    static uint8_t sz[] = { 0x12, 0x34, 0x56, 0, 0xFE };
    static uint8_t ab[] = { 0xAB, 0xAB, 0xAB, 0xAB, 0xAB, 0xAB, 0xAB, 0xAB };
    static uint8_t ins[] = { 0xAB, 0xAB, 0xCD, 0xAB, 0xCD, 0xEF, 0xAB, 0xAB };
    uint8_t * dr;
    uint8_t * p;
    int cr;
    size_t n, m;
    zlx_ma_t ma;

    memset(d, 0, sizeof(d));
    zlx_u8a_copy(d, s, 3);
    T(!memcmp(d, s, 3));

    memset(d, 0xFE, sizeof(d));
    dr = zlx_u8a_zcopy(d, s);
    TE(dr == &d[3], "dr-d: %u", (int) (dr - d));
    T(!memcmp(d, sz, sizeof(sz)));

    T(zlx_u8a_cmp(d, sz, sizeof(sz)) == 0);
    TE((cr = zlx_u8a_cmp(d, s, sizeof(sz))) > 0, "cr: %d", cr);
    T(zlx_u8a_cmp(s, sz, sizeof(sz)) < 0);
    T(zlx_u8a_zcmp(d, sz) == 0);
    d[2] = 0xFF;
    T(zlx_u8a_zcmp(d, sz) > 0);
    d[2] = 0x01;
    T(zlx_u8a_zcmp(d, sz) < 0);
    T(zlx_u8a_zlen(d) == 3);
    T(zlx_u8a_scan(s, 0xCD) == s + 4);
    T(zlx_u8a_search(sz, &sz[sizeof(sz)], 0xCD) == &sz[sizeof(sz)]);
    T(zlx_u8a_search(sz, &sz[sizeof(sz)], 0xFE) == &sz[4]);

    zlx_u8a_set(d, sizeof(d), 0xAB);
    T(memcmp(d, ab, sizeof(ab)) == 0);

    memset(&ma, 0, sizeof(ma));
    ma.realloc = test_realloc;

    dr = &d[0];
    d[2] = 0xCD;
    d[3] = 0xEF;
    n = 4; m = 5;

    p = zlx_u8a_insert(&dr, &n, &m, 1, SIZE_MAX, &ma);
    T(p == NULL);

    p = zlx_u8a_insert(&dr, &n, &m, 1, SIZE_MAX / 2, &ma);
    T(p == NULL);

    p = zlx_u8a_insert(&dr, &n, &m, 1, 8, &ma);
    T(p == NULL);

    p = zlx_u8a_insert(&dr, &n, &m, 1, 2, &ma);
    T(dr == &d[0]); T(p == &d[1]); T(n == 6); T(m == 8);
    T(!memcmp(dr, ins, sizeof(d)));

    p = zlx_u8a_insert(&dr, &n, &m, 6, 2, &ma);
    T(dr == &d[0]); T(p == &d[6]); T(n == 8); T(m == 8);
    T(!memcmp(dr, ins, sizeof(d)));

    return 0;
}

