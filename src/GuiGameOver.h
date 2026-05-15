#ifndef GUI_GAME_OVER_H
#define GUI_GAME_OVER_H

#include "GuiScreen.h"

typedef struct GuiGameOver {
    GuiScreen base;
} GuiGameOver;

GuiGameOver *GuiGameOver_create(void);

void GuiGameOver_initGui(GuiScreen *self_);

void GuiGameOver_keyTyped(GuiScreen *self_, char var1, int var2);

void GuiGameOver_actionPerformed(GuiScreen *self_, GuiButton *var1);

void GuiGameOver_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

int GuiGameOver_doesGuiPauseGame(GuiScreen *self_);

#endif
