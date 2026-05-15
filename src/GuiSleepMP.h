#ifndef GUI_SLEEP_MP_H
#define GUI_SLEEP_MP_H

#include "GuiChat.h"

typedef struct GuiSleepMP {
    GuiChat base;
} GuiSleepMP;

void GuiSleepMP_construct(GuiSleepMP *self);

void GuiSleepMP_initGui(GuiScreen *self);

void GuiSleepMP_onGuiClosed(GuiScreen *self);

void GuiSleepMP_keyTyped(GuiScreen *self, char var1, int var2);

void GuiSleepMP_drawScreen(GuiScreen *self, int var1, int var2, float var3);

void GuiSleepMP_actionPerformed(GuiScreen *self, GuiButton *var1);

void GuiSleepMP_func_22115_j(GuiSleepMP *self);

#endif
