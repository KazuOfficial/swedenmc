#include "Renderer.h"

#include "GuiCrafting.h"
#include "Minecraft.h"
#include "ContainerWorkbench.h"
#include "EntityPlayer.h"
#include <stdlib.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void s_vtable_ensure_init(void) {
    if (s_vtable_init)
        return;
    GuiContainer_initVtable();
    s_vtable = GuiContainer_vtable;
    s_vtable.onGuiClosed = (void (*)(GuiScreen *))GuiCrafting_onGuiClosed;
    s_vtable.drawGuiContainerForegroundLayer =
        (void (*)(GuiScreen *))GuiCrafting_drawGuiContainerForegroundLayer;
    s_vtable.drawGuiContainerBackgroundLayer =
        (void (*)(GuiScreen *, float))GuiCrafting_drawGuiContainerBackgroundLayer;
    s_vtable_init = 1;
}

void GuiCrafting_construct(GuiCrafting *self, InventoryPlayer *var1, World *var2, int var3,
                           int var4, int var5) {
    ContainerWorkbench *container = calloc(1, sizeof(ContainerWorkbench));
    ContainerWorkbench_construct(container, var1, var2, var3, var4, var5);
    GuiContainer_construct(&self->base, (Container *)container);
    s_vtable_ensure_init();
    self->base.base.vtable = &s_vtable;
}

void GuiCrafting_onGuiClosed(GuiCrafting *self) {
    GuiContainer_onGuiClosed((GuiScreen *)&self->base);
    Container_onCraftGuiClosed_v(self->base.inventorySlots,
                                 (EntityPlayer *)self->base.base.mc->thePlayer);
}

void GuiCrafting_drawGuiContainerForegroundLayer(GuiCrafting *self) {
    FontRenderer_drawString(self->base.base.fontRenderer, "Crafting", 28, 6, 4210752);
    FontRenderer_drawString(self->base.base.fontRenderer, "Inventory", 8, self->base.ySize - 96 + 2,
                            4210752);
}

void GuiCrafting_drawGuiContainerBackgroundLayer(GuiCrafting *self, float var1) {
    (void)var1;
    unsigned int var2 =
        RenderEngine_getTexture(self->base.base.mc->renderEngine, "/gui/crafting.png");
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    RenderEngine_bindTexture(self->base.base.mc->renderEngine, var2);
    int var3 = (self->base.base.width - self->base.xSize) / 2;
    int var4 = (self->base.base.height - self->base.ySize) / 2;
    Gui_drawTexturedModalRect((Gui *)self, var3, var4, 0, 0, self->base.xSize, self->base.ySize);
}
