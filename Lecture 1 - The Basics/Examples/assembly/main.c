#include <stdio.h>

extern unsigned long _asm_function();

int main(void)
{
    printf("asm_function returned %lu\n", _asm_function());
    return 0;
}
