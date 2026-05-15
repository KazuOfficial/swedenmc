#ifndef GUI_EDIT_SIGN_H
#define GUI_EDIT_SIGN_H

#include "GuiScreen.h"
#include "TileEntitySign.h"
#include "ChatAllowedCharacters.h"

#define GuiEditSign_allowedCharacters ChatAllowedCharacters_allowedCharacters

typedef struct GuiEditSign {
    GuiScreen base;

    char screenTitle[64];

    TileEntitySign *entitySign;

    int updateCounter;

    int editLine;
} GuiEditSign;

void GuiEditSign_construct(GuiEditSign *self, TileEntitySign *var1);

void GuiEditSign_initGui(GuiEditSign *self);

void GuiEditSign_onGuiClosed(GuiEditSign *self);

void GuiEditSign_updateScreen(GuiEditSign *self);

void GuiEditSign_actionPerformed(GuiEditSign *self, GuiButton *var1);

void GuiEditSign_keyTyped(GuiEditSign *self, char var1, int var2);

void GuiEditSign_drawScreen(GuiEditSign *self, int var1, int var2, float var3);

#endif
