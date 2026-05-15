#include "KeyBinding.h"

void KeyBinding_init(KeyBinding *self, const char *var1, int var2) {
    self->keyDescription = var1;
    self->keyCode = var2;
}
