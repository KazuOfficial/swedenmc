#ifndef KEY_BINDING_H
#define KEY_BINDING_H

typedef struct KeyBinding {

    const char *keyDescription;

    int keyCode;
} KeyBinding;

void KeyBinding_init(KeyBinding *self, const char *var1, int var2);

#endif
