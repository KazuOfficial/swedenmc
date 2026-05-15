#ifndef GUI_CREATE_WORLD_H
#define GUI_CREATE_WORLD_H

#include "GuiScreen.h"
#include "ISaveFormat.h"
#include "GuiTextField.h"

typedef struct GuiCreateWorld {
    GuiScreen base;

    GuiScreen *field_22131_a;

    GuiTextField *textboxWorldName;

    GuiTextField *textboxSeed;

    char folderName[256];

    int createClicked;
} GuiCreateWorld;

void GuiCreateWorld_construct(GuiCreateWorld *self, GuiScreen *var1);

void GuiCreateWorld_updateScreen(GuiScreen *self_);

void GuiCreateWorld_initGui(GuiScreen *self_);

void GuiCreateWorld_func_22129_j(GuiCreateWorld *self);

const char *GuiCreateWorld_generateUnusedFolderName(struct ISaveFormat *var0, const char *var1,
                                                    char *outBuf, int bufLen);

void GuiCreateWorld_onGuiClosed(GuiScreen *self_);

void GuiCreateWorld_actionPerformed(GuiScreen *self_, GuiButton *var1);

void GuiCreateWorld_keyTyped(GuiScreen *self_, char var1, int var2);

void GuiCreateWorld_mouseClicked(GuiScreen *self_, int var1, int var2, int var3);

void GuiCreateWorld_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

void GuiCreateWorld_selectNextField(GuiScreen *self_);

#endif
