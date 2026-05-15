#ifndef GUI_SMALL_BUTTON_H
#define GUI_SMALL_BUTTON_H

#include "GuiButton.h"
#include "EnumOptions.h"

typedef struct GuiSmallButton {
    GuiButton base;

    EnumOptions enumOptions;
} GuiSmallButton;

void GuiSmallButton_construct(GuiSmallButton *self, int var1, int var2, int var3, const char *var4);

void GuiSmallButton_constructSized(GuiSmallButton *self, int var1, int var2, int var3, int var4,
                                   int var5, const char *var6);

void GuiSmallButton_constructEnum(GuiSmallButton *self, int var1, int var2, int var3,
                                  EnumOptions var4, const char *var5);

EnumOptions GuiSmallButton_returnEnumOptions(GuiSmallButton *self);

#endif
