#include "../include/zlx/int/fmt.h"
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

