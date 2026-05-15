#ifndef MINECRAFT_EXCEPTION_H
#define MINECRAFT_EXCEPTION_H

typedef struct MinecraftException {
    char message[512];
} MinecraftException;

void MinecraftException_init(MinecraftException *self, const char *var1);

#endif
