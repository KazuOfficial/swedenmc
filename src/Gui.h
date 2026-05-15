#ifndef GUI_H
#define GUI_H

#include "FontRenderer.h"
#include "Tessellator.h"

typedef struct Gui {
    float zLevel;
} Gui;

void Gui_init(Gui *self);

void Gui_func_27100_a(Gui *self, int var1, int var2, int var3, int var4);

void Gui_func_27099_b(Gui *self, int var1, int var2, int var3, int var4);

void Gui_drawRect(Gui *self, int var1, int var2, int var3, int var4, int var5);

void Gui_drawGradientRect(Gui *self, int var1, int var2, int var3, int var4, int var5, int var6);

void Gui_drawCenteredString(Gui *self, FontRenderer *var1, const char *var2, int var3, int var4,
                            int var5);

void Gui_drawString(Gui *self, FontRenderer *var1, const char *var2, int var3, int var4, int var5);

void Gui_drawTexturedModalRect(Gui *self, int var1, int var2, int var3, int var4, int var5,
                               int var6);

#endif
