#include <string.h>
#include <zlx/obstream/buffer.h>

int buffer_obstream_test (void)
{
    uint8_t buffer[8];
    zlx_buffer_obstream_t bobs;

    if (zlx_buffer_obstream_init(&bobs, buffer, sizeof(buffer))
        != &bobs.as_obstream)
        return 1;
    if (zlx_obstream_write(&bobs.as_obstream, (uint8_t const *) "hello", 5)
        != 5) 
        return 2;

    if (zlx_obstream_write(&bobs.as_obstream, (uint8_t const *) " world!", 7)
        != 3) 
        return 3;

    if (zlx_obstream_write(&bobs.as_obstream, (uint8_t const *) "!!!!", 4)
        != 0) 
        return 4;

    return memcmp(buffer, "hello wo", 8) && bobs.offset == 8;
}

