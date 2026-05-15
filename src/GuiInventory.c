#include "Renderer.h"

#include "GuiInventory.h"
#include "Minecraft.h"
#include "AchievementList.h"
#include "RenderManager.h"
#include "RenderHelper.h"
#include "FontRenderer.h"
#include "GuiAchievements.h"
#include "GuiStats.h"
#include <math.h>
#include <stdlib.h>

static GuiScreenVtable s_vtable;
static int s_vtable_init = 0;

static void s_vtable_ensure_init(void) {
    if (s_vtable_init)
        return;
    GuiContainer_initVtable();
    s_vtable = GuiContainer_vtable;
    s_vtable.initGui = GuiInventory_initGui;
    s_vtable.drawScreen = GuiInventory_drawScreen;
    s_vtable.actionPerformed = GuiInventory_actionPerformed;
    s_vtable.drawGuiContainerForegroundLayer = GuiInventory_drawGuiContainerForegroundLayer;
    s_vtable.drawGuiContainerBackgroundLayer = GuiInventory_drawGuiContainerBackgroundLayer;
    s_vtable_init = 1;
}

void GuiInventory_construct(GuiInventory *self, EntityPlayer *var1) {
    GuiContainer_construct(&self->base, (Container *)var1->inventorySlots);
    s_vtable_ensure_init();
    self->base.base.vtable = &s_vtable;
    self->base.base.field_948_f = 1;
    self->xSize_lo = 0.0F;
    self->ySize_lo = 0.0F;
    EntityPlayer_addStat(var1, (struct StatBase *)AchievementList_openInventory, 1);
}

void GuiInventory_initGui(GuiScreen *self_) {
    GuiInventory *self = (GuiInventory *)self_;
    self->base.base.buttonCount = 0;
}

void GuiInventory_drawGuiContainerForegroundLayer(GuiScreen *self_) {
    GuiInventory *self = (GuiInventory *)self_;
    FontRenderer_drawString(self->base.base.fontRenderer, "Crafting", 86, 16, 4210752);
}

void GuiInventory_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiInventory *self = (GuiInventory *)self_;
    GuiContainer_drawScreen((GuiScreen *)&self->base, var1, var2, var3);
    self->xSize_lo = (float)var1;
    self->ySize_lo = (float)var2;
}

void GuiInventory_drawGuiContainerBackgroundLayer(GuiScreen *self_, float var1) {
    GuiInventory *self = (GuiInventory *)self_;
    (void)var1;
    unsigned int var2 =
        RenderEngine_getTexture(self->base.base.mc->renderEngine, "/gui/inventory.png");
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    RenderEngine_bindTexture(self->base.base.mc->renderEngine, var2);
    int var3 = (self->base.base.width - self->base.xSize) / 2;
    int var4 = (self->base.base.height - self->base.ySize) / 2;
    Gui_drawTexturedModalRect((Gui *)self, var3, var4, 0, 0, self->base.xSize, self->base.ySize);
    R_enable(R_RESCALE_NORMAL);
    R_enable(R_COLOR_MATERIAL);
    R_pushMatrix();
    R_translatef((float)(var3 + 51), (float)(var4 + 75), 50.0F);
    float var5 = 30.0F;
    R_scalef(-var5, var5, var5);
    R_rotatef(180.0F, 0.0F, 0.0F, 1.0F);
    float var6 = ((EntityLiving *)self->base.base.mc->thePlayer)->renderYawOffset;
    float var7 = ((Entity *)self->base.base.mc->thePlayer)->rotationYaw;
    float var8 = ((Entity *)self->base.base.mc->thePlayer)->rotationPitch;
    float var9 = (float)(var3 + 51) - self->xSize_lo;
    float var10 = (float)(var4 + 75 - 50) - self->ySize_lo;
    R_rotatef(135.0F, 0.0F, 1.0F, 0.0F);
    RenderHelper_enableStandardItemLighting();
    R_rotatef(-135.0F, 0.0F, 1.0F, 0.0F);
    R_rotatef(-((float)atan((double)(var10 / 40.0F))) * 20.0F, 1.0F, 0.0F, 0.0F);
    ((EntityLiving *)self->base.base.mc->thePlayer)->renderYawOffset =
        (float)atan((double)(var9 / 40.0F)) * 20.0F;
    ((Entity *)self->base.base.mc->thePlayer)->rotationYaw =
        (float)atan((double)(var9 / 40.0F)) * 40.0F;
    ((Entity *)self->base.base.mc->thePlayer)->rotationPitch =
        -((float)atan((double)(var10 / 40.0F))) * 20.0F;
    ((Entity *)self->base.base.mc->thePlayer)->entityBrightness = 1.0F;
    R_translatef(0.0F, ((Entity *)self->base.base.mc->thePlayer)->yOffset, 0.0F);
    RenderManager_instance->playerViewY = 180.0F;
    RenderManager_renderEntityWithPosYaw(RenderManager_instance,
                                         (struct Entity *)self->base.base.mc->thePlayer, 0.0, 0.0,
                                         0.0, 0.0F, 1.0F);
    ((Entity *)self->base.base.mc->thePlayer)->entityBrightness = 0.0F;
    ((EntityLiving *)self->base.base.mc->thePlayer)->renderYawOffset = var6;
    ((Entity *)self->base.base.mc->thePlayer)->rotationYaw = var7;
    ((Entity *)self->base.base.mc->thePlayer)->rotationPitch = var8;
    R_popMatrix();
    RenderHelper_disableStandardItemLighting();
    R_disable(R_RESCALE_NORMAL);
}

void GuiInventory_actionPerformed(GuiScreen *self_, GuiButton *var1) {
    GuiInventory *self = (GuiInventory *)self_;
    if (var1->id == 0) {
        GuiAchievements *ach = (GuiAchievements *)calloc(1, sizeof(GuiAchievements));
        GuiAchievements_construct(ach, self->base.base.mc->statFileWriter);
        Minecraft_displayGuiScreen(self->base.base.mc, (GuiScreen *)ach);
    }
    if (var1->id == 1) {
        GuiStats *stats = (GuiStats *)calloc(1, sizeof(GuiStats));
        GuiStats_construct(stats, (GuiScreen *)self, self->base.base.mc->statFileWriter);
        Minecraft_displayGuiScreen(self->base.base.mc, (GuiScreen *)stats);
    }
}
