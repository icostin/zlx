#include <zlx/fmt.h>

#define T(cond) if ((cond)) ; else return 1
int digit_from_char_test (void)
{
    T(zlx_digit_from_char('0', 1) == 0);
    T(zlx_digit_from_char('1', 1) == -1);
    T(zlx_digit_from_char('0', 10) == 0);
    T(zlx_digit_from_char('7', 8) == 7);
    T(zlx_digit_from_char('8', 8) == -1);
    return 0;
}

