#include "public/zlx/assert.h"
#include "public/zlx/obstream/nop.h"

ZLX_API zlx_obstream_t * zlx_assert_log = &zlx_nop_obstream;
ZLX_API void (* zlx_abort) (void) = NULL;

