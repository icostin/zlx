#include <string.h>
#include "../include/zlx/language.h"
#include "../include/zlx/int/sizerr.h"
#include "test.h"

ZLX_ENUM_DECL(color_t, color, 0,
              BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE);

ZLX_SIZERR_DECL(sizerr_color_t, sec, color_t);

int enum_test (void)
{
    sizerr_color_t sec;

    T(BLACK == 0);
    T(YELLOW == 6);
    T(!strcmp(color_as_str(RED), "RED"));
    T(!strcmp(color_as_str(9), "<unknown-color>"));

    sec = sec_from_size(0);
    T(sec_is_size(sec)); T(!sec_is_error(sec));
    T(sec_to_size(sec) == 0);

    sec = sec_from_size(1);
    T(sec_is_size(sec)); T(!sec_is_error(sec));
    T(sec_to_size(sec) == 1);

    sec = sec_from_error(BLUE);
    T(!sec_is_size(sec)); T(sec_is_error(sec));
    T(sec_to_error(sec) == BLUE);

    sec = sec_from_error(YELLOW);
    T(!sec_is_size(sec)); T(sec_is_error(sec));
    T(sec_to_error(sec) == YELLOW);

    return 0;
}

