#ifndef GUI_INGAME_MENU_H
#define GUI_INGAME_MENU_H

#include "GuiScreen.h"

typedef struct GuiIngameMenu {
    GuiScreen base;

    int updateCounter2;

    int updateCounter;
} GuiIngameMenu;

void GuiIngameMenu_initGui(GuiScreen *self_);

void GuiIngameMenu_actionPerformed(GuiScreen *self_, GuiButton *var1);

void GuiIngameMenu_updateScreen(GuiScreen *self_);

void GuiIngameMenu_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

GuiIngameMenu *GuiIngameMenu_create(void);
void GuiIngameMenu_free(GuiIngameMenu *self);

#endif
