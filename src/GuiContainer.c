#include "Renderer.h"

#include "GuiContainer.h"
#include "Minecraft.h"
#include "RenderHelper.h"
#include "RenderItem.h"
#include "StringTranslate.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "InventoryPlayer.h"
#include "PlayerController.h"
#include "ItemStack.h"
#include "Slot.h"
#include "Container.h"
#include <GLFW/glfw3.h>
#include <stddef.h>

static RenderItem *itemRenderer = NULL;

static RenderItem *get_itemRenderer(void) {
    if (!itemRenderer)
        itemRenderer = RenderItem_create();
    return itemRenderer;
}

GuiScreenVtable GuiContainer_vtable;
static int GuiContainer_vtable_init = 0;

void GuiContainer_initVtable(void) {
    if (GuiContainer_vtable_init)
        return;
    GuiScreen_initVtable(&GuiContainer_vtable);
    GuiContainer_vtable.initGui = GuiContainer_initGui;
    GuiContainer_vtable.drawScreen = GuiContainer_drawScreen;
    GuiContainer_vtable.mouseClicked = GuiContainer_mouseClicked;
    GuiContainer_vtable.mouseMovedOrUp = GuiContainer_mouseMovedOrUp;
    GuiContainer_vtable.keyTyped = GuiContainer_keyTyped;
    GuiContainer_vtable.onGuiClosed = GuiContainer_onGuiClosed;
    GuiContainer_vtable.destroy = GuiContainer_destroy;
    GuiContainer_vtable.doesGuiPauseGame = GuiContainer_doesGuiPauseGame;
    GuiContainer_vtable.updateScreen = GuiContainer_updateScreen;
    GuiContainer_vtable.drawGuiContainerForegroundLayer =
        GuiContainer_drawGuiContainerForegroundLayer;
    GuiContainer_vtable.drawGuiContainerBackgroundLayer = NULL;
    GuiContainer_vtable_init = 1;
}

void GuiContainer_construct(GuiContainer *self, Container *var1) {
    GuiContainer_initVtable();
    self->base.vtable = &GuiContainer_vtable;
    self->xSize = 176;
    self->ySize = 166;
    self->inventorySlots = var1;
}

void GuiContainer_initGui(GuiScreen *self_) {
    GuiContainer *self = (GuiContainer *)self_;
    GuiScreen_defaultVtable.initGui((GuiScreen *)self);
    self->base.mc->thePlayer->base.craftingInventory = self->inventorySlots;
}

int GuiContainer_getIsMouseOverSlot(GuiContainer *self, Slot *var1, int var2, int var3) {
    int var4 = (self->base.width - self->xSize) / 2;
    int var5 = (self->base.height - self->ySize) / 2;
    var2 -= var4;
    var3 -= var5;
    return var2 >= var1->xDisplayPosition - 1 && var2 < var1->xDisplayPosition + 16 + 1 &&
           var3 >= var1->yDisplayPosition - 1 && var3 < var1->yDisplayPosition + 16 + 1;
}

Slot *GuiContainer_getSlotAtPosition(GuiContainer *self, int var1, int var2) {
    for (int var3 = 0; var3 < self->inventorySlots->slots.count; ++var3) {
        Slot *var4 = (Slot *)self->inventorySlots->slots.entries[var3];
        if (GuiContainer_getIsMouseOverSlot(self, var4, var1, var2)) {
            return var4;
        }
    }
    return NULL;
}

void GuiContainer_drawSlotInventory(GuiContainer *self, Slot *var1) {
    int var2 = var1->xDisplayPosition;
    int var3 = var1->yDisplayPosition;
    ItemStack *var4 = Slot_getStack(var1);
    if (var4 == NULL) {
        int var5 = Slot_getBackgroundIconIndex(var1);
        if (var5 >= 0) {
            R_disable(R_LIGHTING);
            RenderEngine_bindTexture(
                self->base.mc->renderEngine,
                RenderEngine_getTexture(self->base.mc->renderEngine, "/gui/items.png"));
            Gui_drawTexturedModalRect((Gui *)self, var2, var3, var5 % 16 * 16, var5 / 16 * 16, 16,
                                      16);
            R_enable(R_LIGHTING);
            return;
        }
    }
    RenderItem_renderItemIntoGUI(get_itemRenderer(), self->base.fontRenderer,
                                 self->base.mc->renderEngine, var4, var2, var3);
    RenderItem_renderItemOverlayIntoGUI(get_itemRenderer(), self->base.fontRenderer,
                                        self->base.mc->renderEngine, var4, var2, var3);
}

void GuiContainer_drawScreen(GuiScreen *self_, int var1, int var2, float var3) {
    GuiContainer *self = (GuiContainer *)self_;
    GuiScreen_drawDefaultBackground((GuiScreen *)self);
    int var4 = (self->base.width - self->xSize) / 2;
    int var5 = (self->base.height - self->ySize) / 2;
    if (self->base.vtable->drawGuiContainerBackgroundLayer) {
        self->base.vtable->drawGuiContainerBackgroundLayer((GuiScreen *)self, var3);
    }
    R_pushMatrix();
    R_rotatef(120.0F, 1.0F, 0.0F, 0.0F);
    RenderHelper_enableStandardItemLighting();
    R_popMatrix();
    R_pushMatrix();
    R_translatef((float)var4, (float)var5, 0.0F);
    R_color4f(1.0F, 1.0F, 1.0F, 1.0F);
    R_enable(R_RESCALE_NORMAL);
    Slot *var6 = NULL;

    int var9;
    int var10;
    for (int var7 = 0; var7 < self->inventorySlots->slots.count; ++var7) {
        Slot *var8 = (Slot *)self->inventorySlots->slots.entries[var7];
        GuiContainer_drawSlotInventory(self, var8);
        if (GuiContainer_getIsMouseOverSlot(self, var8, var1, var2)) {
            var6 = var8;
            R_disable(R_LIGHTING);
            R_disable(R_DEPTH_TEST);
            var9 = var8->xDisplayPosition;
            var10 = var8->yDisplayPosition;
            Gui_drawGradientRect((Gui *)self, var9, var10, var9 + 16, var10 + 16, -2130706433,
                                 -2130706433);
            R_enable(R_LIGHTING);
            R_enable(R_DEPTH_TEST);
        }
    }

    InventoryPlayer *var12 = &self->base.mc->thePlayer->base.inventory;
    ItemStack *draggedStack = InventoryPlayer_getItemStack(var12);
    if (draggedStack != NULL) {
        R_translatef(0.0F, 0.0F, 32.0F);
        RenderItem_renderItemIntoGUI(get_itemRenderer(), self->base.fontRenderer,
                                     self->base.mc->renderEngine, draggedStack, var1 - var4 - 8,
                                     var2 - var5 - 8);
        RenderItem_renderItemOverlayIntoGUI(get_itemRenderer(), self->base.fontRenderer,
                                            self->base.mc->renderEngine, draggedStack,
                                            var1 - var4 - 8, var2 - var5 - 8);
    }

    R_disable(R_RESCALE_NORMAL);
    RenderHelper_disableStandardItemLighting();
    R_disable(R_LIGHTING);
    R_disable(R_DEPTH_TEST);
    if (self->base.vtable->drawGuiContainerForegroundLayer) {
        self->base.vtable->drawGuiContainerForegroundLayer((GuiScreen *)self);
    }
    if (draggedStack == NULL && var6 != NULL && Slot_getHasStack(var6)) {
        ItemStack *hoverStack = Slot_getStack(var6);
        if (hoverStack != NULL) {
            const char *var13 = StringTranslate_translateNamedKey(
                StringTranslate_getInstance(), ItemStack_getItemName(hoverStack));
            if (var13 && var13[0] != '\0') {
                var9 = var1 - var4 + 12;
                var10 = var2 - var5 - 12;
                int var11 = self->base.fontRenderer
                                ? FontRenderer_getStringWidth(self->base.fontRenderer, var13)
                                : 0;
                Gui_drawGradientRect((Gui *)self, var9 - 3, var10 - 3, var9 + var11 + 3,
                                     var10 + 8 + 3, -1073741824, -1073741824);
                if (self->base.fontRenderer)
                    FontRenderer_drawStringWithShadow(self->base.fontRenderer, var13, var9, var10,
                                                      -1);
            }
        }
    }

    R_popMatrix();
    GuiScreen_defaultVtable.drawScreen((GuiScreen *)self, var1, var2, var3);
    R_enable(R_LIGHTING);
    R_enable(R_DEPTH_TEST);
}

void GuiContainer_drawGuiContainerForegroundLayer(GuiScreen *self) { (void)self; }

void GuiContainer_mouseClicked(GuiScreen *self_, int var1, int var2, int var3) {
    GuiContainer *self = (GuiContainer *)self_;
    GuiScreen_defaultVtable.mouseClicked((GuiScreen *)self, var1, var2, var3);
    if (var3 == 0 || var3 == 1) {
        Slot *var4 = GuiContainer_getSlotAtPosition(self, var1, var2);
        int var5 = (self->base.width - self->xSize) / 2;
        int var6 = (self->base.height - self->ySize) / 2;
        int var7 =
            var1 < var5 || var2 < var6 || var1 >= var5 + self->xSize || var2 >= var6 + self->ySize;
        int var8 = -1;
        if (var4 != NULL) {
            var8 = var4->slotNumber;
        }
        if (var7) {
            var8 = -999;
        }
        if (var8 != -1) {
            int shiftDown =
                glfwGetKey((GLFWwindow *)Minecraft_getWindow(), GLFW_KEY_LEFT_SHIFT) ==
                    GLFW_PRESS ||
                glfwGetKey((GLFWwindow *)Minecraft_getWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
            int var9 = var8 != -999 && shiftDown;
            ItemStack *ret = PlayerController_func_27174_a(
                self->base.mc->playerController, self->inventorySlots->windowId, var8, var3, var9,
                (EntityPlayer *)self->base.mc->thePlayer);
            ItemStack_free(ret);
        }
    }
}

void GuiContainer_mouseMovedOrUp(GuiScreen *self_, int var1, int var2, int var3) {
    (void)self_;
    (void)var1;
    (void)var2;
    if (var3 == 0) {
    }
}

void GuiContainer_keyTyped(GuiScreen *self_, char var1, int var2) {
    GuiContainer *self = (GuiContainer *)self_;
    (void)var1;
    if (var2 == GLFW_KEY_ESCAPE || var2 == self->base.mc->gameSettings->keyBindInventory.keyCode) {
        EntityPlayerSP_closeScreen(self->base.mc->thePlayer);
    }
}

void GuiContainer_onGuiClosed(GuiScreen *self_) {
    GuiContainer *self = (GuiContainer *)self_;
    EntityPlayerSP *player = self->base.mc->thePlayer;
    if (player != NULL) {
        PlayerController_func_20086_a(self->base.mc->playerController,
                                      self->inventorySlots->windowId, (EntityPlayer *)player);
        player->base.craftingInventory = player->base.inventorySlots;
    }
}

void GuiContainer_destroy(GuiScreen *self_) {
    GuiContainer *self = (GuiContainer *)self_;
    EntityPlayerSP *player = self->base.mc ? self->base.mc->thePlayer : NULL;
    Container *cp = (player != NULL) ? player->base.inventorySlots : NULL;
    if (self->inventorySlots && self->inventorySlots != cp)
        Container_destroy(self->inventorySlots);
    self->inventorySlots = NULL;
}

int GuiContainer_doesGuiPauseGame(GuiScreen *self_) {
    (void)self_;
    return 0;
}

void GuiContainer_updateScreen(GuiScreen *self_) {
    GuiContainer *self = (GuiContainer *)self_;
    GuiScreen_defaultVtable.updateScreen((GuiScreen *)self);
    Entity *playerAsEntity = (Entity *)self->base.mc->thePlayer;
    if (!playerAsEntity->vtable->isEntityAlive(playerAsEntity) || playerAsEntity->isDead) {
        EntityPlayerSP_closeScreen(self->base.mc->thePlayer);
    }
}
