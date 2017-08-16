#include <stdio.h>
#include <zlx.h>

int inline_test (void);
int line_str_test (void);

int main (int argc, char const * const * argv)
{
    puts("zlx_test");
    printf("  zlx_lib: %s\n", zlx_lib_name);
    printf("  inline_test: %d\n", inline_test());
    printf("  line_str_test: %d\n", line_str_test());
    return 0;
}

