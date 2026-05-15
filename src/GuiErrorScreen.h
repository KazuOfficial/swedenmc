#ifndef GUI_ERROR_SCREEN_H
#define GUI_ERROR_SCREEN_H

#include "GuiScreen.h"

typedef struct GuiErrorScreen {
    GuiScreen base;

    int field_28098_a;
} GuiErrorScreen;

void GuiErrorScreen_updateScreen(GuiErrorScreen *self);

void GuiErrorScreen_initGui(GuiErrorScreen *self);

void GuiErrorScreen_actionPerformed(GuiErrorScreen *self, GuiButton *var1);

void GuiErrorScreen_keyTyped(GuiErrorScreen *self, char var1, int var2);

void GuiErrorScreen_drawScreen(GuiErrorScreen *self, int var1, int var2, float var3);

#endif
