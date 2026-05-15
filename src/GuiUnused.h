#ifndef GUI_UNUSED_H
#define GUI_UNUSED_H

#include "GuiScreen.h"

typedef struct GuiUnused {
    GuiScreen base;

    char message1[256];

    char message2[256];
} GuiUnused;

void GuiUnused_construct(GuiUnused *self);
void GuiUnused_initGui(GuiUnused *self);
void GuiUnused_drawScreen(GuiUnused *self, int var1, int var2, float var3);
void GuiUnused_keyTyped(GuiUnused *self, char var1, int var2);

#endif
