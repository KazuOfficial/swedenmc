#ifndef GUI_CONNECT_FAILED_H
#define GUI_CONNECT_FAILED_H

#include "GuiScreen.h"

typedef struct GuiConnectFailed {
    GuiScreen base;

    char errorMessage[256];

    char errorDetail[256];
} GuiConnectFailed;

void GuiConnectFailed_construct(GuiConnectFailed *self, const char *var1, const char *var2,
                                const char *var3);

void GuiConnectFailed_updateScreen(GuiScreen *self_);

void GuiConnectFailed_keyTyped(GuiScreen *self_, char var1, int var2);

void GuiConnectFailed_initGui(GuiScreen *self_);

void GuiConnectFailed_actionPerformed(GuiScreen *self_, GuiButton *var1);

void GuiConnectFailed_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

#endif
