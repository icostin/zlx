#include <string.h>
#include "../include/zlx/unicode.h"
#include "test.h"

unsigned int ucp_is_valid
(
    uint32_t ucp
);

unsigned int ucp_to_utf8_len
(
    uint32_t ucp
);

unsigned int ucp_to_utf8
(
    uint32_t ucp,
    uint8_t * out
);

size_t utf8_lead_to_len
(
    uint8_t lead_byte
);

unsigned int utf8_lead_valid
(
    uint8_t lead_byte
);

int unicode_test (void)
{
    uint8_t b[8];

    T(ucp_is_valid(0));
    T(ucp_is_valid(1));
    T(ucp_is_valid(0x80));
    T(ucp_is_valid(0xD7FF));
    T(!ucp_is_valid(0xD800));
    T(!ucp_is_valid(0xDFFF));
    T(ucp_is_valid(0xE000));
    T(ucp_is_valid(0x10FFFF));
    T(!ucp_is_valid(0x110000));
    T(ucp_to_utf8_len(0) == 1);
    T(ucp_to_utf8_len(1) == 1);
    T(ucp_to_utf8_len(0x7F) == 1);
    T(ucp_to_utf8_len(0x80) == 2);
    T(ucp_to_utf8_len(0x7FF) == 2);
    T(ucp_to_utf8_len(0x800) == 3);
    T(ucp_to_utf8_len(0xD800) == 3);
    T(ucp_to_utf8_len(0xDFFF) == 3);
    T(ucp_to_utf8_len(0xFFFF) == 3);
    T(ucp_to_utf8_len(0x10000) == 4);
    T(ucp_to_utf8_len(0x10FFFF) == 4);

    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0, b) == 1); 
    T(!memcmp(b, "\x00-", 2));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(1, b) == 1);
    T(!memcmp(b, "\x01-", 2));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0x7F, b) == 1);
    T(!memcmp(b, "\x7F-", 2));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0x80, b) == 2);
    T(!memcmp(b, "\xC2\x80-", 3));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0x7FF, b) == 2);
    T(!memcmp(b, "\xDF\xBF-", 3));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0x800, b) == 3);
    T(!memcmp(b, "\xE0\xA0\x80-", 4));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0xFFFF, b) == 3);
    T(!memcmp(b, "\xEF\xBF\xBF-", 4));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0x10000, b) == 4);
    T(!memcmp(b, "\xF0\x90\x80\x80-", 5));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0xFFFFF, b) == 4);
    T(!memcmp(b, "\xF3\xBF\xBF\xBF-", 5));
    memset(b, '-', sizeof(b)); T(ucp_to_utf8(0x10FFFF, b) == 4);
    T(!memcmp(b, "\xF4\x8F\xBF\xBF-", 5));

    T(utf8_lead_to_len(0) == 1);
    T(utf8_lead_to_len(0x7F) == 1);
    T(utf8_lead_to_len(0xC2) == 2);
    T(utf8_lead_to_len(0xDF) == 2);
    T(utf8_lead_to_len(0xE0) == 3);
    T(utf8_lead_to_len(0xEF) == 3);
    T(utf8_lead_to_len(0xF0) == 4);
    T(utf8_lead_to_len(0xF4) == 4);

    T(utf8_lead_valid(0x00));
    T(utf8_lead_valid(0x7F));
    T(!utf8_lead_valid(0x80));
    T(!utf8_lead_valid(0xA0));
    T(!utf8_lead_valid(0xB0));
    T(utf8_lead_valid(0xC0));
    T(utf8_lead_valid(0xC1));
    T(utf8_lead_valid(0xC2));
    T(utf8_lead_valid(0xDF));
    T(utf8_lead_valid(0xE0));
    T(utf8_lead_valid(0xEF));
    T(utf8_lead_valid(0xF0));
    T(utf8_lead_valid(0xF4));
    T(utf8_lead_valid(0xF4));

#if 0
    d = zlx_utf16le_to_utf8_len((uint8_t const *) "\xFF\xFE", 2, 0);
    TE(d == 3, "d=%d", (int) d);
#endif

#define UF 0xFFFFFFFF
#define UT(enc, flags, rve, ucpe, ...) \
    { \
        uint8_t const a[] = { __VA_ARGS__ }; \
        uint32_t ucp = UF; \
        ptrdiff_t d; \
        d = zlx_##enc##_to_ucp(a, a + sizeof(a), flags, &ucp); \
        TE(d == rve, "d=%d (expected %d). ucp=%X", (int) d, (int) rve, ucp); \
        TE(ucp == ucpe, "ucp=%X (expected %X)", ucp, ucpe); \
    }

    /* test UTF32LE decoding of a non-surrogate codepoint but
     * with a 16-bit mask identical to the one for surrogates */
    UT(utf32le, 0, 4, 0x10DBFF, 0xFF, 0xDB, 0x10, 0x00);
    UT(utf32le, 0, 4, 0x10DBFF, 0xFF, 0xDB, 0x10, 0x00, 0xFF);

    /* test passing less than 4 bytes of input */
    UT(utf32le, 0, ZLX_UTF_ERR_TRUNC, UF, 0xFF, 0xDB, 0x10);

    /* test decoding a surrogate codepoint without the flag for
     * accepting surrogates */
    UT(utf32le, 0, ZLX_UTF_ERR_SURROGATE, UF, 0xFF, 0xDB, 0x00, 0x00);

    /* test decoding a surrogate codepoint with the flag for
     * accepting surrogates */
    UT(utf32le, ZLX_UTF32_DEC_SURROGATES, 4, 0xDBFF, 0xFF, 0xDB, 0x00, 0x00);

    UT(utf32le, 0, ZLX_UTF_ERR_CP_TOO_BIG, UF, 0x00, 0x00, 0x11, 0x00);

    return 0;
}

