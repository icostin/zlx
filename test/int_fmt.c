#include "../include/zlx/int_fmt.h"

#define T(cond) if ((cond)) ; else return __LINE__
int digit_from_char_test (void)
{
    T(zlx_digit_from_char('0', 1) == 0);
    T(zlx_digit_from_char('1', 1) == -1);
    T(zlx_digit_from_char('0', 10) == 0);
    T(zlx_digit_from_char('7', 8) == 7);
    T(zlx_digit_from_char('8', 8) == -1);
    T(zlx_digit_from_char('a', 10) == -1);
    T(zlx_digit_from_char('a', 11) == 10);
    T(zlx_digit_from_char('A', 11) == 10);
    T(zlx_digit_from_char('z', 35) == -1);
    T(zlx_digit_from_char('z', 36) == 35);
    T(zlx_digit_from_char('Z', 36) == 35);
    T(zlx_digit_from_char(-1, 36) == -1);
    T(zlx_digit_from_char('@', 36) == -1);
    T(zlx_digit_from_char(0x3A, 36) == -1);
    T(zlx_digit_to_char(9) == '9');
    T(zlx_digit_to_char(10) == 'A');
    T(zlx_digit_to_char(35) == 'Z');
    return 0;
}

