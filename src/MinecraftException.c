#include "MinecraftException.h"
#include <string.h>

void MinecraftException_init(MinecraftException *self, const char *var1) {
    strncpy(self->message, var1, sizeof(self->message) - 1);
    self->message[sizeof(self->message) - 1] = '\0';
}
