#ifndef GUI_CONNECTING_H
#define GUI_CONNECTING_H

#include "GuiScreen.h"

struct NetClientHandler;

typedef struct GuiConnecting {
    GuiScreen base;

    struct NetClientHandler *clientHandler;

    int cancelled;
} GuiConnecting;

void GuiConnecting_construct(GuiConnecting *self, struct Minecraft *var1, const char *var2,
                             int var3);

void GuiConnecting_updateScreen(GuiScreen *self);

void GuiConnecting_keyTyped(GuiScreen *self, char var1, int var2);

void GuiConnecting_initGui(GuiScreen *self);

void GuiConnecting_actionPerformed(GuiScreen *self, GuiButton *var1);

void GuiConnecting_drawScreen(GuiScreen *self, int var1, int var2, float var3);

struct NetClientHandler *GuiConnecting_setNetClientHandler(GuiConnecting *var0,
                                                           struct NetClientHandler *var1);

int GuiConnecting_isCancelled(GuiConnecting *var0);

struct NetClientHandler *GuiConnecting_getNetClientHandler(GuiConnecting *var0);

#endif
