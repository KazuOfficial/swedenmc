#ifndef GUI_OPTIONS_H
#define GUI_OPTIONS_H

#include "GuiScreen.h"
#include "GameSettings.h"
#include "EnumOptions.h"

#define GUI_OPTIONS_FIELD_22135_K_SIZE 5
extern EnumOptions GuiOptions_field_22135_k[GUI_OPTIONS_FIELD_22135_K_SIZE];

typedef struct GuiOptions {
    GuiScreen base;

    GuiScreen *parentScreen;

    char screenTitle[256];

    GameSettings *options;
} GuiOptions;

void GuiOptions_construct(GuiOptions *self, GuiScreen *var1, GameSettings *var2);

void GuiOptions_initGui(GuiScreen *self);

void GuiOptions_actionPerformed(GuiScreen *self, GuiButton *var1);

void GuiOptions_drawScreen(GuiScreen *self, int var1, int var2, float var3);

#endif
