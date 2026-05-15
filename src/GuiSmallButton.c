#include "GuiSmallButton.h"
#include <string.h>

void GuiSmallButton_construct(GuiSmallButton *self, int var1, int var2, int var3,
                              const char *var4) {
    GuiSmallButton_constructEnum(self, var1, var2, var3, (EnumOptions)0, var4);
}

void GuiSmallButton_constructSized(GuiSmallButton *self, int var1, int var2, int var3, int var4,
                                   int var5, const char *var6) {
    GuiButton *base = &self->base;
    base->vtable = &GuiButton_defaultVtable;
    base->width = var4;
    base->height = var5;
    base->enabled = 1;
    base->enabled2 = 1;
    base->id = var1;
    base->xPosition = var2;
    base->yPosition = var3;
    if (var6)
        strncpy(base->displayString, var6, sizeof(base->displayString) - 1);
    self->enumOptions = (EnumOptions)0;
}

void GuiSmallButton_constructEnum(GuiSmallButton *self, int var1, int var2, int var3,
                                  EnumOptions var4, const char *var5) {
    GuiButton *base = &self->base;
    base->vtable = &GuiButton_defaultVtable;
    base->width = 150;
    base->height = 20;
    base->enabled = 1;
    base->enabled2 = 1;
    base->id = var1;
    base->xPosition = var2;
    base->yPosition = var3;
    if (var5)
        strncpy(base->displayString, var5, sizeof(base->displayString) - 1);
    self->enumOptions = var4;
}

EnumOptions GuiSmallButton_returnEnumOptions(GuiSmallButton *self) { return self->enumOptions; }
