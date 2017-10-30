#include <string.h>
#include "../include/zlx/clconv/interface.h"
#include "../include/zlx/clconv/hex.h"
#include "test.h"

int clconv_test (void)
{
    uint8_t o[0x100];
    size_t il, ol;

    memset(o, '-', sizeof(o));
    T(zlx_clconv_bin_to_hex(
            (uint8_t const *) "\x00\xFF\\a1\x7F\x80", 7,
            o, sizeof(o), &il, &ol, NULL) == ZLX_CLCONV_OK);
    T(il == 7); T(ol == 14);
    TE(memcmp(o, "00FF5C61317F80-", 15) == 0, "o: %.*s", 14, o);

    memset(o, '-', sizeof(o));
    T(zlx_clconv_bin_to_hex(NULL, 0, o, sizeof(o), &il, &ol, NULL)
      == ZLX_CLCONV_OK);
    T(il == 0); T(ol == 0);
    T(!memcmp(o, "-", 1));

    memset(o, '-', sizeof(o));
    T(zlx_clconv_bin_to_hex(
            (uint8_t const *) "\x00\xFF\\a1\x7F\x80", 7,
            o, 5, &il, &ol, NULL) == ZLX_CLCONV_FULL);
    T(il == 2); T(ol == 4);
    TE(memcmp(o, "00FF-", 5) == 0, "o: %.*s", 4, o);

    memset(o, '-', sizeof(o));
    T(zlx_clconv_hex_to_bin((uint8_t const *) "4100FF42", 8,
                            o, sizeof(o), &il, &ol, NULL) == ZLX_CLCONV_OK);
    T(il == 8); T(ol == 4);
    T(!memcmp(o, "A\000\377B-", 5));

    memset(o, '-', sizeof(o));
    T(zlx_clconv_hex_to_bin((uint8_t const *) "4100ff4", 7,
                            o, sizeof(o), &il, &ol, NULL)
      == ZLX_CLCONV_INCOMPLETE);
    T(il == 6); T(ol == 3);
    T(!memcmp(o, "A\000\377-", 4));

    memset(o, '-', sizeof(o));
    T(zlx_clconv_hex_to_bin((uint8_t const *) "4100Fg4", 7,
                            o, sizeof(o), &il, &ol, NULL)
      == ZLX_CLCONV_MALFORMED);
    T(il == 5); T(ol == 2); T(!memcmp(o, "A\000-", 3));

    memset(o, '-', sizeof(o));
    T(zlx_clconv_hex_to_bin((uint8_t const *) "4100 FF42", 9,
                            o, sizeof(o), &il, &ol, NULL)
      == ZLX_CLCONV_MALFORMED);
    T(il == 4); T(ol == 2); T(!memcmp(o, "A\000-", 3));

    memset(o, '-', sizeof(o));
    T(zlx_clconv_hex_to_bin((uint8_t const *) "4100FF42", 8,
                            o, 3, &il, &ol, NULL) == ZLX_CLCONV_FULL);
    T(il == 6); T(ol == 3);
    T(!memcmp(o, "A\000\377-", 4));

    memset(o, '-', sizeof(o));
    T(zlx_clconv_hex_to_bin((uint8_t const *) "4100 FF:42", 10,
                            o, sizeof(o), &il, &ol, ": ")
      == ZLX_CLCONV_OK);
    T(il == 10); T(ol == 4); T(!memcmp(o, "A\000\377B-", 5));

    memset(o, '-', sizeof(o));
    T(zlx_clconv_hex_to_bin((uint8_t const *) "410:0 FF:42", 11,
                            o, sizeof(o), &il, &ol, ": ")
      == ZLX_CLCONV_MALFORMED);
    T(il == 3); T(ol == 1); T(!memcmp(o, "A-", 2));

    return 0;
}

