#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include "GuiScreen.h"
#include "Container.h"
#include "ItemStack.h"
#include "Slot.h"

typedef struct GuiContainer {
    GuiScreen base;

    int xSize;

    int ySize;

    Container *inventorySlots;
} GuiContainer;

extern GuiScreenVtable GuiContainer_vtable;
void GuiContainer_initVtable(void);

void GuiContainer_construct(GuiContainer *self, Container *var1);

void GuiContainer_initGui(GuiScreen *self_);

void GuiContainer_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

void GuiContainer_drawGuiContainerForegroundLayer(GuiScreen *self);

void GuiContainer_drawSlotInventory(GuiContainer *self, Slot *var1);

Slot *GuiContainer_getSlotAtPosition(GuiContainer *self, int var1, int var2);

int GuiContainer_getIsMouseOverSlot(GuiContainer *self, Slot *var1, int var2, int var3);

void GuiContainer_mouseClicked(GuiScreen *self_, int var1, int var2, int var3);

void GuiContainer_mouseMovedOrUp(GuiScreen *self_, int var1, int var2, int var3);

void GuiContainer_keyTyped(GuiScreen *self_, char var1, int var2);

void GuiContainer_onGuiClosed(GuiScreen *self_);

void GuiContainer_destroy(GuiScreen *self_);

int GuiContainer_doesGuiPauseGame(GuiScreen *self_);

void GuiContainer_updateScreen(GuiScreen *self_);

#endif
