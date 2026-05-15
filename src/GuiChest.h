#ifndef GUI_CHEST_H
#define GUI_CHEST_H

#include "GuiContainer.h"
#include "IInventory.h"

typedef struct GuiChest {
    GuiContainer base;

    struct IInventory *upperChestInventory;

    struct IInventory *lowerChestInventory;

    int inventoryRows;
} GuiChest;

void GuiChest_construct(GuiChest *self, struct IInventory *var1, struct IInventory *var2);

void GuiChest_drawGuiContainerForegroundLayer(GuiScreen *self);

void GuiChest_drawGuiContainerBackgroundLayer(GuiScreen *self, float var1);

#endif
