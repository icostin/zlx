#include "../include/zlx/memalloc/tlsf.h"
#include "test.h"

int tlsf_test (void)
{
    uint8_t buffer[0x1000];
    zlx_tlsf_status_t ts;
    zlx_ma_t * ma;
    unsigned int c;

    for (c = 0; c < (32 - 5) * 32; ++c)
    {
        size_t size = zlx_tlsf_cell_to_size(c);
        unsigned int cc;

        printf("cell 0x%02X: size=0x%zX\n", c, size);
        cc = zlx_tlsf_size_to_cell(size);
        TE(c == cc, "c=0x%02X cc=0x%02X size=0x%zX", c, cc, size);
    }

    ts = zlx_tlsf_create(&ma, buffer, sizeof(buffer), 12);
    TE(ts == ZLX_TLSF_OK, "status %s", zlx_tlsf_status_as_str(ts));

    // ac=40 mssb(40 >> 5)=mssb(2)=2

    return 0;
}

