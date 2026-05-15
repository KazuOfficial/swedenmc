#include "StatStringFormatKeyInv.h"
#include "Minecraft.h"
#include "GameSettings.h"
#include <GLFW/glfw3.h>
#include "platform.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static const char *StatStringFormatKeyInv_formatString(IStatStringFormat *base, const char *var1) {
    StatStringFormatKeyInv *self = (StatStringFormatKeyInv *)base;
    static char buf[512];
    static char keyBuf[64];
    int keyCode = self->mc->gameSettings->keyBindInventory.keyCode;
    const char *keyName = glfwGetKeyName(keyCode, 0);
    if (keyName == NULL)
        keyName = "?";

    strncpy(keyBuf, keyName, sizeof(keyBuf) - 1);
    keyBuf[sizeof(keyBuf) - 1] = '\0';
    for (int i = 0; keyBuf[i]; i++)
        keyBuf[i] = (char)toupper((unsigned char)keyBuf[i]);
    snprintf(buf, sizeof(buf), var1, keyBuf);
    return buf;
}

static const IStatStringFormatVtable s_vtable = {.formatString =
                                                     StatStringFormatKeyInv_formatString};

StatStringFormatKeyInv *StatStringFormatKeyInv_create(struct Minecraft *mc) {
    StatStringFormatKeyInv *self =
        (StatStringFormatKeyInv *)calloc(1, sizeof(StatStringFormatKeyInv));
    self->base.vtable = &s_vtable;
    self->mc = mc;
    return self;
}
