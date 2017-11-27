#include <string.h>
#include "enum_sizerr.h"
#include "soft_abort.h"

int bad_sec_from_size (void * ctx)
{
    (void) ctx;
    sec_from_size((size_t) (ptrdiff_t) -1);
    return 0;
}

int bad_sec_from_error (void * ctx)
{
    (void) ctx;
    sec_from_error(BLACK);
    return 0;
}

int bad_sec_to_size (void * ctx)
{
    (void) ctx;
    sec_to_size(sec_from_error(GREEN));
    return 0;
}

int bad_sec_to_error (void * ctx)
{
    (void) ctx;
    sec_to_error(sec_from_size(1));
    return 0;
}


int enum_test (void)
{
    sizerr_color_t sec;

    T(BLACK == 0);
    T(YELLOW == 6);
    T(!strcmp(tcolor_as_str(RED), "RED"));
    T(!strcmp(tcolor_as_str(9), "<unknown-color>"));

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

    T(run_catching_aborts(bad_sec_from_size, NULL, 1) == 1);
    T(run_catching_aborts(bad_sec_from_error, NULL, 1) == 1);
    T(run_catching_aborts(bad_sec_to_size, NULL, 1) == 1);
    T(run_catching_aborts(bad_sec_to_error, NULL, 1) == 1);

    return 0;
}

