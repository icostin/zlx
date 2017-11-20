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
        uint8_t const a[] = { 0xCD, __VA_ARGS__ }; \
        uint32_t ucp = UF; \
        ptrdiff_t d; \
        d = zlx_##enc##_to_ucp(a + 1, a + sizeof(a), flags, &ucp); \
        TE(d == rve, "d=%d (expected %d). ucp=%X", (int) d, (int) rve, ucp); \
        TE(ucp == ucpe, "ucp=%X (expected %X)", ucp, ucpe); \
    }

    /* test UTF32LE decoding of a non-surrogate codepoint but
     * with a 16-bit mask identical to the one for surrogates */
    UT(utf32le, 0, 4, 0x10DBFF, 0xFF, 0xDB, 0x10, 0x00);
    UT(utf32le, 0, 4, 0x10DBFF, 0xFF, 0xDB, 0x10, 0x00, 0xFF);

    /* test passing less than 4 bytes of input */
    UT(utf32le, 0, -ZLX_UTF_ERR_TRUNC, UF, 0xFF, 0xDB, 0x10);

    /* test decoding a surrogate codepoint without the flag for
     * accepting surrogates */
    UT(utf32le, 0, -ZLX_UTF_ERR_SURROGATE, UF, 0xFF, 0xDB, 0x00, 0x00);

    /* test decoding a surrogate codepoint with the flag for
     * accepting surrogates */
    UT(utf32le, ZLX_UTF32_DEC_SURROGATES, 4, 0xDBFF, 0xFF, 0xDB, 0x00, 0x00);

    UT(utf32le, 0, -ZLX_UTF_ERR_CP_TOO_BIG, UF, 0x00, 0x00, 0x11, 0x00);

    UT(utf16le, 0, 2, 0xD7FF, 0xFF, 0xD7);
    UT(utf16le, 0, -ZLX_UTF_ERR_TRUNC, UF, 0xFF);
    UT(utf16le, 0, -ZLX_UTF_ERR_TRUNC, UF, );
    UT(utf16le, 0, 4, 0x10000, 0x00, 0xD8, 0x00, 0xDC);
    UT(utf16le, 0, -ZLX_UTF_ERR_TRUNC, UF, 0x00, 0xD8, 0x00);
    UT(utf16le, ZLX_UTF16_DEC_NO_PAIRING, 2, 0xD800, 0x00, 0xD8, 0x00);
    UT(utf16le, 0, 4, 0x10FFFF, 0xFF, 0xDB, 0xFF, 0xDF);
    UT(utf16le, ZLX_UTF16_DEC_UNPAIRED_SURROGATES, 2, 0xD800, 0x00, 0xD8, 0xFF, 0xD7);
    UT(utf16le, 0, -ZLX_UTF_ERR_SURROGATE, UF, 0x00, 0xD8, 0xFF, 0xD7);
    UT(utf16le, ZLX_UTF16_DEC_UNPAIRED_SURROGATES, 2, 0xDFFF, 0xFF, 0xDF);
    UT(utf16le, 0, -ZLX_UTF_ERR_SURROGATE, UF, 0xFF, 0xDF);

    /* UTF8 ***************************/
    UT(utf8, 0, 1, 0, 0x00);
    UT(utf8, ZLX_UTF8_DEC_NO_NUL_BYTE, -ZLX_UTF_ERR_NUL_BYTE, UF, 0x00);
    UT(utf8, 0, -ZLX_UTF_ERR_TRUNC, UF, );
    UT(utf8, 0, -ZLX_UTF_ERR_LEAD, UF, 0x80);
    UT(utf8, 0, -ZLX_UTF_ERR_LEAD, UF, 0xBF);
    UT(utf8, 0, -ZLX_UTF_ERR_TRUNC, UF, 0xC0);
    UT(utf8, 0, -ZLX_UTF_ERR_CONT1, UF, 0xC0, 0x7F);
    UT(utf8, 0, -ZLX_UTF_ERR_CONT1, UF, 0xC0, 0xC0);
    UT(utf8, 0, -ZLX_UTF_ERR_CONT1, UF, 0xC0, 0xFF);
    UT(utf8, 0, -ZLX_UTF_ERR_OVERLY_LONG, UF, 0xC0, 0x80);
    UT(utf8, 0, 2, 0x80, 0xC2, 0x80);
    UT(utf8, 0, 2, 0x7FF, 0xDF, 0xBF);
    UT(utf8, ZLX_UTF8_DEC_OVERLY_LONG, 2, 0, 0xC0, 0x80);
    UT(utf8, ZLX_UTF8_DEC_TWO_BYTE_NUL, 2, 0, 0xC0, 0x80);
    UT(utf8, ZLX_UTF8_DEC_OVERLY_LONG, 2, 0x7F, 0xC1, 0xBF);
    UT(utf8, 0, -ZLX_UTF_ERR_TRUNC, UF, 0xE0, 0x80);
    UT(utf8, 0, -ZLX_UTF_ERR_CONT1, UF, 0xE0, 0xC0, 0x80);
    UT(utf8, 0, -ZLX_UTF_ERR_CONT2, UF, 0xE0, 0x80, 0xC0);
    UT(utf8, 0, -ZLX_UTF_ERR_OVERLY_LONG, UF, 0xE0, 0x80, 0x80);
    UT(utf8, 0, -ZLX_UTF_ERR_OVERLY_LONG, UF, 0xE0, 0x9F, 0xBF);
    UT(utf8, 0, 3, 0x800, 0xE0, 0xA0, 0x80);
    UT(utf8, ZLX_UTF8_DEC_SURROGATES, 3, 0xD800, 0xED, 0xA0, 0x80);
    UT(utf8, ZLX_UTF8_DEC_SURROGATES, 3, 0xDC00, 0xED, 0xB0, 0x80);
    UT(utf8, ZLX_UTF8_DEC_SURROGATE_PAIRS, 6, 0x10000, 0xED, 0xA0, 0x80, 0xED, 0xB0, 0x80);
    UT(utf8, ZLX_UTF8_DEC_SURROGATE_PAIRS, 6, 0x10FFFF, 0xED, 0xAF, 0xBF, 0xED, 0xBF, 0xBF);
    UT(utf8, 0, -ZLX_UTF_ERR_SURROGATE, UF, 0xED, 0xA0, 0x80);
    UT(utf8, 0, -ZLX_UTF_ERR_TRUNC, UF, 0xF0, 0x80, 0x80);
    UT(utf8, 0, -ZLX_UTF_ERR_CONT1, UF, 0xF0, 0xC0, 0x80, 0x80);
    UT(utf8, 0, -ZLX_UTF_ERR_CONT2, UF, 0xF0, 0x80, 0xC0, 0x80);
    UT(utf8, 0, -ZLX_UTF_ERR_CONT3, UF, 0xF0, 0x80, 0x80, 0xC0);
    UT(utf8, 0, 4, 0x10000, 0xF0, 0x90, 0x80, 0x80);
    UT(utf8, ZLX_UTF8_DEC_OVERLY_LONG, 4, 0xFFFF, 0xF0, 0x8F, 0xBF, 0xBF);
    UT(utf8, 0, -ZLX_UTF_ERR_OVERLY_LONG, UF, 0xF0, 0x8F, 0xBF, 0xBF);
    UT(utf8, 0, 4, 0x10FFFF, 0xF4, 0x8F, 0xBF, 0xBF);
    UT(utf8, 0, -ZLX_UTF_ERR_CP_TOO_BIG, UF, 0xF4, 0x90, 0x80, 0x80);

#undef UT
#undef UF


    T(zlx_ucp_to_utf8_size(0, 0) == 1);
    T(zlx_ucp_to_utf8_size(0, ZLX_UTF8_ENC_TWO_BYTE_NUL) == 2);
    T(zlx_ucp_to_utf8_size(0x80, 0) == 2);
    T(zlx_ucp_to_utf8_size(0x7FF, 0) == 2);
    T(zlx_ucp_to_utf8_size(0x800, 0) == 3);
    T(zlx_ucp_to_utf8_size(0xFFFF, 0) == 3);
    T(zlx_ucp_to_utf8_size(0x10000, 0) == 4);
    T(zlx_ucp_to_utf8_size(0x10000, ZLX_UTF8_ENC_BREAK_SUPPLEM) == 6);
    T(zlx_ucp_to_utf16_size(0xFFFF, 0) == 2);
    T(zlx_ucp_to_utf16_size(0x10000, 0) == 4);
    T(zlx_ucp_to_utf32_size(0xFFFF, 0) == 4);
    T(zlx_ucp_to_utf32_size(0x10000, 0) == 4);
    T(zlx_ucp_to_utf32_size(0x10000, ZLX_UTF32_ENC_BREAK_SUPPLEM) == 8);

#define UT(...) UT1(__VA_ARGS__, 0xCA)
#define UT1(enc, ucp, flags, ...) \
    { \
        uint8_t const a[] = { __VA_ARGS__ }; \
        uint8_t b[16]; memset(b, 0xCA, sizeof(b)); \
        T(zlx_ucp_to_##enc(ucp, flags, b) == sizeof(a) - 1); \
        TE(!memcmp(a, b, sizeof(a)), "%02X %02X %02X %02X %02X %02X", \
           b[0], b[1], b[2], b[3], b[4], b[5]); \
    }

    UT(utf8, 0, 0, 0x00);
    UT(utf8, 0, ZLX_UTF8_ENC_TWO_BYTE_NUL, 0xC0, 0x80);
    UT(utf8, 0x80, 0, 0xC2, 0x80);
    UT(utf8, 0x800, 0, 0xE0, 0xA0, 0x80);
    UT(utf8, 0x10000, 0, 0xF0, 0x90, 0x80, 0x80);
    UT(utf8, 0x10000, ZLX_UTF8_ENC_BREAK_SUPPLEM,
       0xED, 0xA0, 0x80, 0xED, 0xB0, 0x80);
    UT(utf16le, 0xABCD, 0, 0xCD, 0xAB);
    UT(utf16le, 0xABCDE, 0, 0x6F, 0xDA, 0xDE, 0xDC);

    UT(utf32le, 0x10FFFE, 0, 0xFE, 0xFF, 0x10, 0x00);
    UT(utf32le, 0x10FFFE, ZLX_UTF32_ENC_BREAK_SUPPLEM,
       0xFF, 0xDB, 0x00, 0x00, 0xFE, 0xDF, 0x00, 0x00);
#undef UT1
#undef UT

{
    uint8_t const a8[] = { 0xF4, 0x8F, 0xBF, 0xBF };
    uint8_t const a8sp[] = { 0xED, 0xAF, 0xBF, 0xED, 0xBF, 0xBF };
    uint8_t const a16[] = { 0xFF, 0xDB, 0xFF, 0xDF };
    uint8_t const a32[] = { 0xFF, 0xFF, 0x10, 0x00 };
    uint8_t o[16];
    size_t pos;

    memset(o, '-', sizeof(o));
    T(zlx_uconv(ZLX_UTF8_DEC | ZLX_UTF8_DEC_SURROGATE_PAIRS | ZLX_UTF16LE_ENC,
                a8sp, sizeof(a8sp), o, 3, &pos) == 4);
    TE(!memcmp(o, a16, 3), "%02X %02X", o[0], o[1]); T(o[3] == '-'); T(pos == 6);

    memset(o, '-', sizeof(o));
    T(zlx_uconv(ZLX_UTF8_DEC | ZLX_UTF16LE_ENC,
                a8, 3, o, 3, &pos) == -ZLX_UTF_ERR_TRUNC);

    memset(o, '-', sizeof(o)); pos = 99;
    T(zlx_uconv(0, a8sp, 1, o, 1, &pos) == -ZLX_UTF_ERR_NO_CONV); T(pos == 0);

    memset(o, '-', sizeof(o)); pos = 99;
    T(zlx_uconv(ZLX_UTF8_DEC, a8sp, 1, o, 1, &pos) == -ZLX_UTF_ERR_NO_CONV);
    T(pos == 0);

    memset(o, '-', sizeof(o));
    T(zlx_uconv(ZLX_UTF16LE_DEC | ZLX_UTF32LE_ENC, a16, sizeof(a16), o, 4, NULL)
      == 4);
    T(!memcmp(o, a32, 4));

    memset(o, '-', sizeof(o));
    T(zlx_uconv(ZLX_UTF32LE_DEC | ZLX_UTF8_ENC, a32, sizeof(a32), o, 4, &pos)
      == 4);
    T(!memcmp(o, a8, 4)); T(o[4] == '-');
}

    T(zlx_ucp_term_width(0x41) == 1);
    T(zlx_ucp_term_width(0x300) == 0);
    T(zlx_ucp_term_width(0x1100) == 2);
    T(zlx_ucp_term_width(0xFFFF) == -1);

#define WT(str, len, err, w) \
{ \
    size_t parsed_size, width; \
    zlx_utf_error_t e; \
    e = zlx_utf8_term_width((uint8_t const *) str, sizeof(str) - 1, &width, \
                             &parsed_size, NULL); \
    TE(e == err, "err=%d", e); \
    TE(parsed_size == len, "parsed_size=%u", (int) parsed_size); \
    TE(w == width, "width=%u", (int) w); \
}

    WT("coo coo", 7, 0, 7);
    WT("a\xCC\x80g", 4, 0, 2);
    WT("a\xCC\xC0g", 1, ZLX_UTF_ERR_CONT1, 1);
    WT("a\xEF\xBF\xBFg", 4, ZLX_UTF_ERR_NON_PRINTABLE, 1);
#undef WT

    return 0;
}

