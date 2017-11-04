#include <string.h>
#include <inttypes.h>
#include "../include/zlx/clconv/interface.h"
#include "../include/zlx/clconv/hex.h"
#include "../include/zlx/clconv/c_escape.h"
#include "test.h"

int clconv_test (void)
{
    uint8_t o[0x100];
    zlx_clconv_c_escape_t ce;
    zlx_clconv_status_t cs;
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

    T(zlx_clconv_c_escape_force_hex(
            (uint8_t const *) "\\\x01\x41\aa\b\xFE""b\tt\nn\ff\rr\x7F", 17,
            o, sizeof(o), &il, &ol, NULL) == ZLX_CLCONV_OK);
//printf("[%.*s]\n", (int) ol, o);
    TE(il == 17, "il: %u", (int) il); TE(ol == 33, "ol: %u", (int) ol);
    T(!memcmp(o, "\\\\\\x01A\\aa\\b\\xFEb\\tt\\nn\\ff\\rr\\x7F", ol));

    T(zlx_clconv_c_escape_force_hex(
            (uint8_t const *) "\\\x01\x41\aa\b\xFE""b\tt\nn\ff\rr\x7F", 17,
            o, 32, &il, &ol, NULL) == ZLX_CLCONV_FULL);
    TE(il == 16, "il: %u", (int) il); TE(ol == 29, "ol: %u", (int) ol);
    T(!memcmp(o, "\\\\\\x01A\\aa\\b\\xFEb\\tt\\nn\\ff\\rr", ol));

    zlx_clconv_c_escape_init(&ce);

    il = ol = 999;
    memset(o, '-', sizeof(o));
    cs = zlx_clconv_c_escape(o + 8, 0, o, 8, &il, &ol, &ce);
    T(cs == ZLX_CLCONV_OK); T(il == 0); T(ol == 0); T(!memcmp(o, "----", 4));

    memset(o, '-', sizeof(o));
    T(zlx_clconv_c_escape(
            (uint8_t const *) "\x01\x41\aa\b\xFE""b\tt\nn\ff\rr\x7F", 16,
            o, sizeof(o), &il, &ol, &ce) == ZLX_CLCONV_OK);
//printf("[%.*s]\n", (int) ol, o);
    TE(il == 16, "il: %u", (int) il); TE(ol == 27, "ol: %u", (int) ol);
    T(!memcmp(o, "\\001A\\aa\\b\\376b\\tt\\nn\\ff\\rr-", ol + 1));
    T(ce.crt_byte == 0x7F);

    T(zlx_clconv_c_escape(NULL, 0, o, sizeof(o), &il, &ol, &ce)
      == ZLX_CLCONV_OK);
//printf("[%.*s]\n", (int) ol, o);
    TE(ol == 4, "ol: %u", (int) ol);
    T(!memcmp(o, "\\x7F", ol));

    memset(o, '-', sizeof(o));
    zlx_clconv_c_escape_init(&ce);
    T(zlx_clconv_c_escape( (uint8_t const *) "[\x01]", 3, o, 2, &il, &ol, &ce)
      == ZLX_CLCONV_FULL);
//printf("[%.*s]\n", (int) ol, o);
    TE(il == 2, "il: %u", (int) il); TE(ol == 1, "ol: %u", (int) ol);
    T(!memcmp(o, "[", ol)); T(ce.crt_byte == 1);

    cs = zlx_clconv_c_escape((uint8_t const *) "]", 1, o, 8, &il, &ol, &ce);
    TE(cs == ZLX_CLCONV_OK, "cs: %u, il:%u o:'%.*s'", cs, (int) il, (int) ol, (char *) o);
//printf("[%.*s]\n", (int) ol, o);
    TE(il == 1, "il: %u", (int) il); TE(ol == 5, "ol: %u", (int) ol);
    T(!memcmp(o, "\\x01]", ol));

    il = ol = 999;
    memset(o, '-', sizeof(o));
    cs = zlx_clconv_c_escape(NULL, 0, o, 8, &il, &ol, &ce);
    T(cs == ZLX_CLCONV_OK); T(ol == 0); T(!memcmp(o, "----", 4));

    zlx_clconv_c_escape_init(&ce); il = ol = 999; memset(o, '-', sizeof(o));
    cs = zlx_clconv_c_escape((uint8_t const *) "a\\b", 3, o, 2, &il, &ol, &ce);
    T(cs == ZLX_CLCONV_FULL); T(il == 2); T(ol == 1); T(!memcmp(o, "a---", 4));

    zlx_clconv_c_escape_init(&ce); il = ol = 999; memset(o, '-', sizeof(o));
    cs = zlx_clconv_c_escape((uint8_t const *) "a\\", 2, o, 3, &il, &ol, &ce);
    T(cs == ZLX_CLCONV_OK); T(il == 2); T(ol == 3); T(!memcmp(o, "a\\\\-", 4));

    zlx_clconv_c_escape_init(&ce); il = ol = 999; memset(o, '-', sizeof(o));
    cs = zlx_clconv_c_escape((uint8_t const *) "ab", 2, o, 1, &il, &ol, &ce);
    T(cs == ZLX_CLCONV_FULL); T(il == 2); T(ol == 1); T(!memcmp(o, "a---", 4));

    il = ol = 999; memset(o, '-', sizeof(o));
    cs = zlx_clconv_c_escape(NULL, 0, o, 4, &il, &ol, &ce);
    printf("[%.*s]\n", (int) ol, (char *) o);
    T(cs == ZLX_CLCONV_OK); T(ol == 1); T(!memcmp(o, "b---", 4));

    return 0;
}

