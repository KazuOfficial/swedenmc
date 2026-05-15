#ifndef MINECRAFT_ERROR_H
#define MINECRAFT_ERROR_H

#include <setjmp.h>

extern jmp_buf MinecraftError_jmpbuf;
extern int MinecraftError_active;

void MinecraftError_throw(void);

#endif
