#include "Renderer.h"

#include "GuiChest.h"
#include "ContainerChest.h"
#include "InventoryLargeChest.h"
#include "FontRenderer.h"
#include "RenderEngine.h"
#include "Minecraft.h"
#include <stdlib.h>

static GuiScreenVtable s_GuiChest_vtable;
static int s_GuiChest_vtable_init = 0;

static void GuiChest_destroy(struct GuiScreen *base) {
    GuiChest *self = (GuiChest *)base;

    if (InventoryLargeChest_isInstance(self->lowerChestInventory)) {
        free(self->lowerChestInventory);
        self->lowerChestInventory = NULL;
    }
}

static void s_GuiChest_vtable_ensure_init(void) {
    if (s_GuiChest_vtable_init)
        return;
    GuiContainer_initVtable();
    s_GuiChest_vtable = GuiContainer_vtable;
    s_GuiChest_vtable.drawGuiContainerForegroundLayer = GuiChest_drawGuiContainerForegroundLayer;
    s_GuiChest_vtable.drawGuiContainerBackgroundLayer = GuiChest_drawGuiContainerBackgroundLayer;
    s_GuiChest_vtable.destroy = GuiChest_destroy;
    s_GuiChest_vtable_init = 1;
}

void GuiChest_construct(GuiChest *self, struct IInventory *var1, struct IInventory *var2) {
    ContainerChest *container = (ContainerChest *)calloc(1, sizeof(ContainerChest));
    ContainerChest_construct(container, var1, var2);
    GuiContainer_construct(&self->base, (Container *)container);
    s_GuiChest_vtable_ensure_init();
    self->base.base.vtable = &s_GuiChest_vtable;
    self->upperChestInventory = var1;
    self->lowerChestInventory = var2;
    self->base.base.field_948_f = 0;
    short var3 = 222;
    int var4 = var3 - 108;
    self->inventoryRows = var2->vtable->getSizeInventory(var2) / 9;
    self->base.ySize = var4 + self->inventoryRows * 18;
}

void GuiChest_drawGuiContainerForegroundLayer(GuiScreen *base) {
    GuiChest *self = (GuiChest *)base;
    FontRenderer *fr = self->base.base.fontRenderer;
    if (fr) {
        FontRenderer_drawString(
            fr, self->lowerChestInventory->vtable->getInvName(self->lowerChestInventory), 8, 6,
            4210752);
        FontRenderer_drawString(
            fr, self->upperChestInventory->vtable->getInvName(self->upperChestInventory), 8,
            self->base.ySize - 96 + 2, 4210752);
    }
}

void GuiChest_drawGuiContainerBackgroundLayer(GuiScreen *base, float var1) {
    (void)var1;
    GuiChest *self = (GuiChest *)base;
    int var2 = RenderEngine_getTexture(self->base.base.mc->renderEngine, "/gui/container.png");
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    RenderEngine_bindTexture(self->base.base.mc->renderEngine, var2);
    int var3 = (self->base.base.width - self->base.xSize) / 2;
    int var4 = (self->base.base.height - self->base.ySize) / 2;
    Gui_drawTexturedModalRect((Gui *)self, var3, var4, 0, 0, self->base.xSize,
                              self->inventoryRows * 18 + 17);
    Gui_drawTexturedModalRect((Gui *)self, var3, var4 + self->inventoryRows * 18 + 17, 0, 126,
                              self->base.xSize, 96);
}
