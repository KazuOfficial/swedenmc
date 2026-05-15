#include "MinecraftError.h"
#include <stdio.h>
#include <stdlib.h>

jmp_buf MinecraftError_jmpbuf;
int MinecraftError_active = 0;

void MinecraftError_throw(void) {
    if (MinecraftError_active) {
        longjmp(MinecraftError_jmpbuf, 1);
    }
    fprintf(stderr, "MinecraftError thrown with no catch site\n");
    abort();
}
