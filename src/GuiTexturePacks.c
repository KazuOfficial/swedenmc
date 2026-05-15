#include "GuiTexturePacks.h"
#include "GuiSmallButton.h"
#include "StringTranslate.h"
#include "Minecraft.h"
#include "TexturePackList.h"
#include <string.h>
#include <stdlib.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void ensure_vtable(void) {
    if (s_vtable_init)
        return;
    GuiScreen_initVtable(&s_vtable);
    s_vtable.initGui = GuiTexturePacks_initGui;
    s_vtable.actionPerformed = GuiTexturePacks_actionPerformed;
    s_vtable.mouseClicked = GuiTexturePacks_mouseClicked;
    s_vtable.mouseMovedOrUp = GuiTexturePacks_mouseMovedOrUp;
    s_vtable.drawScreen = GuiTexturePacks_drawScreen;
    s_vtable.updateScreen = GuiTexturePacks_updateScreen;
    s_vtable_init = 1;
}

void GuiTexturePacks_construct(GuiTexturePacks *self, GuiScreen *var1) {
    ensure_vtable();
    self->base.vtable = &s_vtable;
    self->guiScreen = var1;
    self->base.parentScreen = var1;
    self->field_6454_o = -1;
}

void GuiTexturePacks_initGui(GuiScreen *self_) {
    GuiTexturePacks *self = (GuiTexturePacks *)self_;
    StringTranslate *var1 = StringTranslate_getInstance();
    GuiSmallButton *btn5 = (GuiSmallButton *)calloc(1, sizeof(GuiSmallButton));
    GuiSmallButton_construct(btn5, 5, self->base.width / 2 - 154, self->base.height - 48,
                             StringTranslate_translateKey(var1, "texturePack.openFolder"));
    GuiScreen_addButton(&self->base, (GuiButton *)btn5);

    GuiSmallButton *btn6 = (GuiSmallButton *)calloc(1, sizeof(GuiSmallButton));
    GuiSmallButton_construct(btn6, 6, self->base.width / 2 + 4, self->base.height - 48,
                             StringTranslate_translateKey(var1, "gui.done"));
    GuiScreen_addButton(&self->base, (GuiButton *)btn6);

    TexturePackList_updateAvaliableTexturePacks(self->base.mc->texturePackList);

    snprintf(self->fileLocation, sizeof(self->fileLocation), "%s/texturepacks",
             Minecraft_getMinecraftDir());

    self->guiTexturePackSlot = (GuiTexturePackSlot *)calloc(1, sizeof(GuiTexturePackSlot));
    GuiTexturePackSlot_construct(self->guiTexturePackSlot, self);
    GuiSlot_registerScrollButtons(&self->guiTexturePackSlot->base, (void *)self->base.buttonList, 7,
                                  8);
}

void GuiTexturePacks_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiTexturePacks *self = (GuiTexturePacks *)self_;
    if (var1->enabled) {
        if (var1->id == 5) {

            char url[768];
            snprintf(url, sizeof(url), "file://%s", self->fileLocation);
            Sys_openURL(url);
        } else if (var1->id == 6) {
            RenderEngine_refreshTextures(self->base.mc->renderEngine);
            Minecraft_displayGuiScreen(self->base.mc, self->guiScreen);
        } else {
            GuiSlot_actionPerformed(&self->guiTexturePackSlot->base, var1);
        }
    }
}

void GuiTexturePacks_mouseClicked(GuiScreen *self_, int var1, int var2, int var3) {
    GuiTexturePacks *self = (GuiTexturePacks *)self_;
    GuiScreen_defaultVtable.mouseClicked(&self->base, var1, var2, var3);
}

void GuiTexturePacks_mouseMovedOrUp(GuiScreen *self_, int var1, int var2, int var3) {
    GuiTexturePacks *self = (GuiTexturePacks *)self_;
    GuiScreen_defaultVtable.mouseMovedOrUp(&self->base, var1, var2, var3);
}

void GuiTexturePacks_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiTexturePacks *self = (GuiTexturePacks *)self_;
    GuiSlot_drawScreen(&self->guiTexturePackSlot->base, var1, var2, var3);
    if (self->field_6454_o <= 0) {
        TexturePackList_updateAvaliableTexturePacks(self->base.mc->texturePackList);
        self->field_6454_o += 20;
    }
    StringTranslate *var4 = StringTranslate_getInstance();
    Gui_drawCenteredString((Gui *)self, self->base.fontRenderer,
                           StringTranslate_translateKey(var4, "texturePack.title"),
                           self->base.width / 2, 16, 16777215);
    Gui_drawCenteredString((Gui *)self, self->base.fontRenderer,
                           StringTranslate_translateKey(var4, "texturePack.folderInfo"),
                           self->base.width / 2 - 77, self->base.height - 26, 8421504);
    GuiScreen_defaultVtable.drawScreen(&self->base, var1, var2, var3);
}

void GuiTexturePacks_updateScreen(GuiScreen *self_) {
    GuiTexturePacks *self = (GuiTexturePacks *)self_;
    GuiScreen_defaultVtable.updateScreen(&self->base);
    --self->field_6454_o;
}

struct Minecraft *GuiTexturePacks_func_22124_a(GuiTexturePacks *var0) { return var0->base.mc; }
struct Minecraft *GuiTexturePacks_func_22126_b(GuiTexturePacks *var0) { return var0->base.mc; }
struct Minecraft *GuiTexturePacks_func_22119_c(GuiTexturePacks *var0) { return var0->base.mc; }
struct Minecraft *GuiTexturePacks_func_22122_d(GuiTexturePacks *var0) { return var0->base.mc; }
struct Minecraft *GuiTexturePacks_func_22117_e(GuiTexturePacks *var0) { return var0->base.mc; }
struct Minecraft *GuiTexturePacks_func_22118_f(GuiTexturePacks *var0) { return var0->base.mc; }
struct Minecraft *GuiTexturePacks_func_22116_g(GuiTexturePacks *var0) { return var0->base.mc; }
struct Minecraft *GuiTexturePacks_func_22121_h(GuiTexturePacks *var0) { return var0->base.mc; }
struct Minecraft *GuiTexturePacks_func_22123_i(GuiTexturePacks *var0) { return var0->base.mc; }
FontRenderer *GuiTexturePacks_func_22127_j(GuiTexturePacks *var0) {
    return var0->base.fontRenderer;
}
FontRenderer *GuiTexturePacks_func_22120_k(GuiTexturePacks *var0) {
    return var0->base.fontRenderer;
}
FontRenderer *GuiTexturePacks_func_22125_l(GuiTexturePacks *var0) {
    return var0->base.fontRenderer;
}
