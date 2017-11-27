#ifndef _ENUM_SIZERR_TEST_H
#define _ENUM_SIZERR_TEST_H

#include "../include/zlx/language.h"
#include "../include/zlx/int/sizerr.h"
#include "test.h"

ZLX_ENUM_DECL(color_t, color, 0,
              BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE);

ZLX_SIZERR_DECL(sizerr_color_t, sizerr_color, color_t);

char const * tcolor_as_str (color_t c);

int sec_is_size (sizerr_color_t sec);
int sec_is_error (sizerr_color_t sec);

size_t sec_to_size (sizerr_color_t sec);
color_t sec_to_error (sizerr_color_t sec);

sizerr_color_t sec_from_size (size_t sec);
sizerr_color_t sec_from_error (color_t sec);




#endif /* _ENUM_SIZERR_TEST_H */
