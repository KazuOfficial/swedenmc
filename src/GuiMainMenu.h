#ifndef GUI_MAIN_MENU_H
#define GUI_MAIN_MENU_H

#include "GuiScreen.h"

typedef struct GuiMainMenu {
    GuiScreen base;

    float updateCounter;

    char splashText[256];

    GuiButton *multiplayerButton;
} GuiMainMenu;

void GuiMainMenu_updateScreen(GuiScreen *self);
void GuiMainMenu_keyTyped(GuiScreen *self, char var1, int var2);
void GuiMainMenu_initGui(GuiScreen *self);
void GuiMainMenu_actionPerformed(GuiScreen *self, GuiButton *var1);
void GuiMainMenu_drawScreen(GuiScreen *self, int var1, int var2, float var3);

GuiMainMenu *GuiMainMenu_create(void);
void GuiMainMenu_free(GuiMainMenu *self);

#endif
