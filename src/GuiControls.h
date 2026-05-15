#ifndef GUI_CONTROLS_H
#define GUI_CONTROLS_H

#include "GuiScreen.h"
#include "GameSettings.h"

struct GuiSmallButton;

typedef struct GuiControls {
    GuiScreen base;

    GuiScreen *parentScreen;

    char screenTitle[256];

    GameSettings *options;

    int buttonId;
} GuiControls;

void GuiControls_construct(GuiControls *self, GuiScreen *var1, GameSettings *var2);

int GuiControls_func_20080_j(GuiControls *self);

void GuiControls_initGui(GuiScreen *self_);

void GuiControls_actionPerformed(GuiScreen *self_, GuiButton *var1);

void GuiControls_keyTyped(GuiScreen *self_, char var1, int var2);

void GuiControls_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

#endif
