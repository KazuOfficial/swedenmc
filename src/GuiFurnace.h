#ifndef GUI_FURNACE_H
#define GUI_FURNACE_H

#include "GuiContainer.h"
#include "InventoryPlayer.h"
#include "TileEntityFurnace.h"

typedef struct GuiFurnace {
    GuiContainer base;

    TileEntityFurnace *furnaceInventory;
} GuiFurnace;

void GuiFurnace_construct(GuiFurnace *self, InventoryPlayer *var1, TileEntityFurnace *var2);

void GuiFurnace_drawGuiContainerForegroundLayer(GuiFurnace *self);

void GuiFurnace_drawGuiContainerBackgroundLayer(GuiFurnace *self, float var1);

#endif
