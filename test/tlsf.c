#include <inttypes.h>
#include "../include/zlx/memalloc/tlsf.h"
#include "../include/zlx/memalloc/interface.h"
#include "../include/zlx/log.h"
#include "test.h"

int tlsf_test (void)
{
    static uint8_t buffer[0x10000];
    zlx_tlsf_status_t ts;
    zlx_ma_t * ma;
    unsigned int c;
    size_t i;
    uint8_t * p;
    zlx_log_level_t ll;

    for (c = 0; c < (64 - 9) * 32; ++c)
    {
        size_t size = zlx_tlsf_cell_to_size(c);
        unsigned int cc;

        //printf("cell 0x%02X: size=0x%" PRIX64 "\n", c, (uint64_t) size);
        cc = zlx_tlsf_size_to_cell(size);
        TE(c == cc, "c=0x%02X cc=0x%02X size=0x%" PRIX64 "", c, cc, (uint64_t) size);
    }

    ts = zlx_tlsf_create(&ma, buffer, sizeof(buffer), ZLX_TLSF_BLOCK_LIMIT + 1);
    TE(ts == ZLX_TLSF_BAD_MAX, "status %s", zlx_tlsf_status_as_str(ts));

    ts = zlx_tlsf_create(&ma, buffer, 0, 4096);
    TE(ts == ZLX_TLSF_BUFFER_TOO_SMALL, "status %s", zlx_tlsf_status_as_str(ts));

    ts = zlx_tlsf_create(&ma, (void *) ((intptr_t) 0 - ZLX_TLSF_BLOCK_LIMIT), ZLX_TLSF_BLOCK_LIMIT, ZLX_TLSF_BLOCK_LIMIT);
    TE(ts == ZLX_TLSF_BAD_BUFFER, "status %s", zlx_tlsf_status_as_str(ts));
    // ac=40 mssb(40 >> 5)=mssb(2)=2

    ts = zlx_tlsf_create(&ma, buffer, 100, sizeof(buffer));
    TE(ts == ZLX_TLSF_BUFFER_TOO_SMALL, "status %s size_needed=%lu", 
       zlx_tlsf_status_as_str(ts), (long) (intptr_t) ma);

    ts = zlx_tlsf_create(&ma, buffer, sizeof(buffer) / 2, sizeof(buffer));
    TE(ts == ZLX_TLSF_OK, "status %s size_needed=%lu", 
       zlx_tlsf_status_as_str(ts), (long) (intptr_t) ma);

    ts = zlx_tlsf_add_block(ma, buffer + sizeof(buffer) / 2,
                            sizeof(buffer) / 2);

    ts = zlx_tlsf_create(&ma, buffer, 0, 0);
    TE(ts == ZLX_TLSF_BAD_MAX, "status %s", zlx_tlsf_status_as_str(ts));

    ll = zlx_log_set_level(zlx_default_log, ZLX_LL_WARNING);
    for (i = 1; i < sizeof(buffer) - 1; ++i)
    {
        ts = zlx_tlsf_create(&ma, buffer + 1, i, i);
        if (ts == ZLX_TLSF_OK) break;
        TE(ts == ZLX_TLSF_BUFFER_TOO_SMALL, "status %s", 
           zlx_tlsf_status_as_str(ts));
    }
    zlx_log_set_level(zlx_default_log, ll);

    ts = zlx_tlsf_create(&ma, buffer + 1, i, i);
    T(ts == ZLX_TLSF_OK);

    T(zlx_alloc(ma, 0, "none") == NULL);
    p = zlx_alloc(ma, 1, "one byte");
    TE(p >= &buffer[0] && (size_t) (p - buffer) < sizeof(buffer), "p=%p", 
       (void *) p);

    return 0;
}

