#ifndef GUI_CHAT_H
#define GUI_CHAT_H

#include "GuiScreen.h"

typedef struct GuiChat {
    GuiScreen base;

    char message[101];

    int updateCounter;
} GuiChat;

void GuiChat_construct(GuiChat *self);

void GuiChat_initGui(GuiScreen *self_);

void GuiChat_onGuiClosed(GuiScreen *self_);

void GuiChat_updateScreen(GuiScreen *self_);

void GuiChat_keyTyped(GuiScreen *self_, char var1, int var2);

void GuiChat_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

void GuiChat_mouseClicked(GuiScreen *self_, int var1, int var2, int var3);

#endif
