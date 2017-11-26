#include <string.h>
#include "../include/zlx/language.h"
#include "test.h"

ZLX_ENUM_DECL(color, 0, BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE);

int enum_test (void)
{
    T(BLACK == 0);
    T(YELLOW == 6);
    T(!strcmp(color_as_str(RED), "RED"));
    T(!strcmp(color_as_str(9), "<unknown-color>"));

    return 0;
}

