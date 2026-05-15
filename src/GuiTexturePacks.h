#ifndef GUI_TEXTURE_PACKS_H
#define GUI_TEXTURE_PACKS_H

#include "GuiScreen.h"
#include "GuiTexturePackSlot.h"

typedef struct GuiTexturePacks {
    GuiScreen base;

    GuiScreen *guiScreen;

    int field_6454_o;

    char fileLocation[256];

    GuiTexturePackSlot *guiTexturePackSlot;
} GuiTexturePacks;

void GuiTexturePacks_construct(GuiTexturePacks *self, GuiScreen *var1);

void GuiTexturePacks_initGui(GuiScreen *self);
void GuiTexturePacks_actionPerformed(GuiScreen *self, GuiButton *var1);
void GuiTexturePacks_mouseClicked(GuiScreen *self, int var1, int var2, int var3);
void GuiTexturePacks_mouseMovedOrUp(GuiScreen *self, int var1, int var2, int var3);
void GuiTexturePacks_drawScreen(GuiScreen *self, int var1, int var2, float var3);
void GuiTexturePacks_updateScreen(GuiScreen *self);

struct Minecraft *GuiTexturePacks_func_22124_a(GuiTexturePacks *var0);

struct Minecraft *GuiTexturePacks_func_22126_b(GuiTexturePacks *var0);

struct Minecraft *GuiTexturePacks_func_22119_c(GuiTexturePacks *var0);

struct Minecraft *GuiTexturePacks_func_22122_d(GuiTexturePacks *var0);

struct Minecraft *GuiTexturePacks_func_22117_e(GuiTexturePacks *var0);

struct Minecraft *GuiTexturePacks_func_22118_f(GuiTexturePacks *var0);

struct Minecraft *GuiTexturePacks_func_22116_g(GuiTexturePacks *var0);

struct Minecraft *GuiTexturePacks_func_22121_h(GuiTexturePacks *var0);

struct Minecraft *GuiTexturePacks_func_22123_i(GuiTexturePacks *var0);

FontRenderer *GuiTexturePacks_func_22127_j(GuiTexturePacks *var0);

FontRenderer *GuiTexturePacks_func_22120_k(GuiTexturePacks *var0);

FontRenderer *GuiTexturePacks_func_22125_l(GuiTexturePacks *var0);

#endif
