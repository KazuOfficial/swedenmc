#ifndef GUI_YES_NO_H
#define GUI_YES_NO_H

#include "GuiScreen.h"

typedef struct GuiYesNo {
    GuiScreen base;

    GuiScreen *parentScreen;

    char message1[256];

    char message2[256];

    char field_22106_k[256];

    char field_22105_l[256];

    int worldNumber;
} GuiYesNo;

void GuiYesNo_construct(GuiYesNo *self, GuiScreen *var1, const char *var2, const char *var3,
                        const char *var4, const char *var5, int var6);

void GuiYesNo_initGui(GuiScreen *self);

void GuiYesNo_actionPerformed(GuiScreen *self, GuiButton *var1);

void GuiYesNo_drawScreen(GuiScreen *self, int var1, int var2, float var3);

#endif
