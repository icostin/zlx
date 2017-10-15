#ifndef _SOFT_ABORT_H
#define _SOFT_ABORT_H

#include "../include/zlx/lib.h"

extern ZLX_LOCAL unsigned int soft_abort_count;
extern ZLX_LOCAL char assert_msg[0x100];

ZLX_LOCAL void init_soft_abort (void);
ZLX_LOCAL int run_catching_aborts
(
    int (* func) (void *),
    void * context,
    int ret_on_abort
);

#endif
