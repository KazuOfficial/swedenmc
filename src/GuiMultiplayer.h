#ifndef GUI_MULTIPLAYER_H
#define GUI_MULTIPLAYER_H

#include "GuiScreen.h"
#include "GuiTextField.h"

typedef struct GuiMultiplayer {
    GuiScreen base;

    GuiScreen *parentScreen;

    GuiTextField *field_22111_h;
} GuiMultiplayer;

void GuiMultiplayer_construct(GuiMultiplayer *self, GuiScreen *var1);

void GuiMultiplayer_updateScreen(GuiScreen *self_);

void GuiMultiplayer_initGui(GuiScreen *self_);

void GuiMultiplayer_onGuiClosed(GuiScreen *self_);

void GuiMultiplayer_actionPerformed(GuiScreen *self_, GuiButton *var1);

int GuiMultiplayer_parseIntWithDefault(GuiMultiplayer *self, const char *var1, int var2);

void GuiMultiplayer_keyTyped(GuiScreen *self_, char var1, int var2);

void GuiMultiplayer_mouseClicked(GuiScreen *self_, int var1, int var2, int var3);

void GuiMultiplayer_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

#endif
