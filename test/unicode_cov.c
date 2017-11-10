#include "../include/zlx/unicode.h"

unsigned int ucp_is_valid
(
    uint32_t ucp
)
{
    return zlx_ucp_is_valid(ucp);
}

unsigned int ucp_to_utf8_len
(
    uint32_t ucp
)
{
    return zlx_ucp_to_utf8_len(ucp);
}

unsigned int ucp_to_utf8
(
    uint32_t ucp,
    uint8_t * out
)
{
    return zlxi_ucp_to_utf8(ucp, out);
}

size_t utf8_lead_to_len
(
    uint8_t lead_byte
)
{
    return zlx_utf8_lead_to_len(lead_byte);
}

unsigned int utf8_lead_valid
(
    uint8_t lead_byte
)
{
    return zlx_utf8_lead_valid(lead_byte);
}

