#include "enum_sizerr.h"

char const * tcolor_as_str (color_t c)
{
    return color_as_str(c);
}

int sec_is_size (sizerr_color_t sec)
{
    return sizerr_color_is_size(sec);
}

int sec_is_error (sizerr_color_t sec)
{
    return sizerr_color_is_error(sec);
}

size_t sec_to_size (sizerr_color_t sec)
{
    return sizerr_color_to_size(sec);
}

color_t sec_to_error (sizerr_color_t sec)
{
    return sizerr_color_to_error(sec);
}

sizerr_color_t sec_from_size (size_t sec)
{
    return sizerr_color_from_size(sec);
}

sizerr_color_t sec_from_error (color_t sec)
{
    return sizerr_color_from_error(sec);
}

