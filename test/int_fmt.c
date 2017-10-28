#include <string.h>
#include <stdio.h>
#include "../include/zlx/int/fmt.h"
#include "soft_abort.h"
#include "test.h"

ZLX_LOCAL void u8a_zero (uint8_t * a, size_t n);
ZLX_LOCAL unsigned int digit_to_char (unsigned int digit);
ZLX_LOCAL int digit_from_char (uint32_t ch, uint_fast8_t radix);

int digit_char_test (void)
{
    T(digit_to_char(0) == '0');
    T(digit_to_char(9) == '9');
    T(digit_to_char(10) == 'A');
    T(digit_to_char(35) == 'Z');

    T(digit_from_char(0, 2) == -1);
    T(digit_from_char(0x2F, 2) == -1);
    T(digit_from_char('0', 2) == 0);
    T(digit_from_char('9', 10) == 9);
    T(digit_from_char('9', 9) == -1);
    T(digit_from_char(0x3A, 36) == -1);
    T(digit_from_char(0x40, 36) == -1);
    T(digit_from_char('A', 36) == 10);
    T(digit_from_char('a', 36) == 10);
    T(digit_from_char('A', 10) == -1);
    T(digit_from_char('a', 10) == -1);
    T(digit_from_char('Z', 36) == 35);
    T(digit_from_char('z', 36) == 35);
    T(digit_from_char(0x5B, 36) == -1);
    T(digit_from_char(0x7B, 36) == -1);
    T(digit_from_char(0x80, 36) == -1);
    T(digit_from_char(0x131, 36) == -1);
    T(digit_from_char(0xFFFFFFFF, 36) == -1);

    return 0;
}

/* u64_from_str_test ********************************************************/
int u64_from_str_test (void)
{
    uint64_t v;
    size_t l;

    v = 1; l = 1;
    T(zlx_u64_from_str(NULL, 0, 12, &v, &l) == ZLX_U64_STOP);
    T(v == 0);
    T(l == 0);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "Fd2dDF", 6, 16, &v, &l) == 0);
    T(v == 0xFD2DDF);
    T(l == 6);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "0b0101", 6, 0, &v, &l) == 0);
    T(v == 5);
    T(l == 6);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "0o0101", 6, 0, &v, &l) == 0);
    T(v == 65);
    T(l == 6);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "0d0101", 6, 0, &v, &l) == 0);
    T(v == 101);
    T(l == 6);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "0x0101", 6, 0, &v, &l) == 0);
    T(v == 257);
    T(l == 6);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "101", 3, 0, &v, &l) == 0);
    T(v == 101);
    T(l == 3);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "0101", 4, 0, &v, &l) == 0);
    T(v == 101);
    T(l == 4);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "Zza", 2, 36, &v, &l) == 0);
    T(v == 1295);
    T(l == 2);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "99a", 3, 0, &v, &l) == ZLX_U64_STOP);
    T(v == 99);
    T(l == 2);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "8-a", 2, 0, &v, &l) == ZLX_U64_STOP);
    T(v == 8);
    T(l == 1);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "18446744073709551615", 20, 0, &v, &l) == 0);
    T(v == UINT64_C(0xFFFFFFFFFFFFFFFF));
    T(l == 20);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "18446744073709551616", 20, 0, &v, &l) == ZLX_U64_OVERFLOW);
    T(v == UINT64_C(1844674407370955161));
    T(l == 19);

    v = 0; l = 0;
    T(zlx_u64_from_str((uint8_t const *) "184467440737095516150", 21, 0, &v, &l) == ZLX_U64_OVERFLOW);
    T(v == UINT64_C(18446744073709551615));
    T(l == 20);

    return 0;
}

/* i64_to_str_test **********************************************************/
int i64_to_str_test (void)
{
    uint8_t s[0x100];
    size_t n;
    
    n = zlx_i64_to_str(s, 123, ZLX_NO_SIGN, 10, NULL, 1, 64, 0);
    T(n == 3); T(!strcmp((char const *) s, "123"));

    n = zlx_i64_to_str(s, 123, ZLX_NO_SIGN, 10, (uint8_t const *) "abc", 1, 64, 0);
    T(n == 6); T(!strcmp((char const *) s, "abc123"));

    n = zlx_i64_to_str(s, 123, ZLX_NO_SIGN, 10, (uint8_t const *) "abc", 7, 64, 0);
    T(n == 7); T(!strcmp((char const *) s, "abc0123"));

    n = zlx_i64_to_str(s, -123, ZLX_SIGN_NEG, 10, NULL, 1, 64, 0);
    T(n == 4); T(!strcmp((char const *) s, "-123"));

    n = zlx_i64_to_str(s, 123, ZLX_SIGN_NEG, 10, NULL, 1, 64, 0);
    T(n == 3); T(!strcmp((char const *) s, "123"));

    n = zlx_i64_to_str(s, 123, ZLX_SIGN_ALWAYS, 10, NULL, 1, 64, 0);
    T(n == 4); T(!strcmp((char const *) s, "+123"));

    n = zlx_i64_to_str(s, -123, ZLX_SIGN_ALWAYS, 10, NULL, 1, 64, 0);
    T(n == 4); T(!strcmp((char const *) s, "-123"));

    n = zlx_i64_to_str(s, 0, ZLX_SIGN_ALWAYS, 10, NULL, 1, 64, 0);
    T(n == 2); T(!strcmp((char const *) s, " 0"));

    n = zlx_i64_to_str(s, 123, ZLX_SIGN_ALIGN, 10, NULL, 1, 64, 0);
    T(n == 4); T(!strcmp((char const *) s, " 123"));

    n = zlx_i64_to_str(s, -123, ZLX_SIGN_ALIGN, 10, NULL, 1, 64, 0);
    T(n == 4); T(!strcmp((char const *) s, "-123"));

    return 0;
}

/* u64_to_str_bad_radix *****************************************************/
int u64_to_str_bad_radix (void * radix)
{
    uint8_t s[0x100];

    zlx_u64_to_str(s, 123, (uint8_t) (uintptr_t) radix, 0, 0, 0);
    return 0;
}

/* u64_to_str_test **********************************************************/
int u64_to_str_test (void)
{
    uint8_t s[0x100];
    size_t l;

    l = zlx_u64_to_str(s, 123, 10, 0, 0, 0);
    T(zlx_u64_to_str(s, 123, 10, 0, 0, 0) == 3);
    T(!strcmp((char const *) s, "123"));

    l = zlx_u64_to_str(s, 0, 2, 0, 0, 0);
    T(l == 1);
    T(!strcmp((char const *) s, "0"));

    l = zlx_u64_to_str(s, 12345, 10, 7, 3, ',');
    T(l == 7);
    T(!strcmp((char const *) s, "012,345"));

    l = zlx_u64_to_str(s, 12345, 10, 8, 3, ',');
    TE(l == 8, "str: '%s' len: %u", s, (int) l);
    T(!strcmp((char const *) s, ",012,345"));

    T(run_catching_aborts(u64_to_str_bad_radix, (void *) (uintptr_t) 1, 1));
    T(run_catching_aborts(u64_to_str_bad_radix, (void *) (uintptr_t) 37, 1));

    return 0;
}
