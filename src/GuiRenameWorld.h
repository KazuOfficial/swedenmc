#ifndef GUI_RENAME_WORLD_H
#define GUI_RENAME_WORLD_H

#include "GuiScreen.h"
#include "GuiTextField.h"

typedef struct GuiRenameWorld {
    GuiScreen base;

    GuiScreen *field_22112_a;

    GuiTextField *field_22114_h;

    char field_22113_i[256];
} GuiRenameWorld;

void GuiRenameWorld_construct(GuiRenameWorld *self, GuiScreen *var1, const char *var2);

void GuiRenameWorld_updateScreen(GuiScreen *self);

void GuiRenameWorld_initGui(GuiScreen *self);

void GuiRenameWorld_onGuiClosed(GuiScreen *self);

void GuiRenameWorld_actionPerformed(GuiScreen *self, GuiButton *var1);

void GuiRenameWorld_keyTyped(GuiScreen *self, char var1, int var2);

void GuiRenameWorld_mouseClicked(GuiScreen *self, int var1, int var2, int var3);

void GuiRenameWorld_drawScreen(GuiScreen *self, int var1, int var2, float var3);

#endif
