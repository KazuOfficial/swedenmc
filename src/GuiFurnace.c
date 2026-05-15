#include "Renderer.h"

#include "GuiFurnace.h"
#include "Minecraft.h"
#include "ContainerFurnace.h"
#include "FontRenderer.h"
#include <stdlib.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void s_vtable_ensure_init(void) {
    if (s_vtable_init)
        return;
    GuiContainer_initVtable();
    s_vtable = GuiContainer_vtable;
    s_vtable.drawGuiContainerForegroundLayer =
        (void (*)(GuiScreen *))GuiFurnace_drawGuiContainerForegroundLayer;
    s_vtable.drawGuiContainerBackgroundLayer =
        (void (*)(GuiScreen *, float))GuiFurnace_drawGuiContainerBackgroundLayer;
    s_vtable_init = 1;
}

void GuiFurnace_construct(GuiFurnace *self, InventoryPlayer *var1, TileEntityFurnace *var2) {
    ContainerFurnace *container = calloc(1, sizeof(ContainerFurnace));
    ContainerFurnace_construct(container, var1, var2);
    GuiContainer_construct(&self->base, (Container *)container);
    s_vtable_ensure_init();
    self->base.base.vtable = &s_vtable;
    self->furnaceInventory = var2;
}

void GuiFurnace_drawGuiContainerForegroundLayer(GuiFurnace *self) {
    FontRenderer_drawString(self->base.base.fontRenderer, "Furnace", 60, 6, 4210752);
    FontRenderer_drawString(self->base.base.fontRenderer, "Inventory", 8, self->base.ySize - 96 + 2,
                            4210752);
}

void GuiFurnace_drawGuiContainerBackgroundLayer(GuiFurnace *self, float var1) {
    (void)var1;
    unsigned int var2 =
        RenderEngine_getTexture(self->base.base.mc->renderEngine, "/gui/furnace.png");
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    RenderEngine_bindTexture(self->base.base.mc->renderEngine, var2);
    int var3 = (self->base.base.width - self->base.xSize) / 2;
    int var4 = (self->base.base.height - self->base.ySize) / 2;
    Gui_drawTexturedModalRect((Gui *)self, var3, var4, 0, 0, self->base.xSize, self->base.ySize);
    int var5;
    if (TileEntityFurnace_isBurning(self->furnaceInventory)) {
        var5 = TileEntityFurnace_getBurnTimeRemainingScaled(self->furnaceInventory, 12);
        Gui_drawTexturedModalRect((Gui *)self, var3 + 56, var4 + 36 + 12 - var5, 176, 12 - var5, 14,
                                  var5 + 2);
    }
    var5 = TileEntityFurnace_getCookProgressScaled(self->furnaceInventory, 24);
    Gui_drawTexturedModalRect((Gui *)self, var3 + 79, var4 + 34, 176, 14, var5 + 1, 16);
}
