#ifndef GUI_DISPENSER_H
#define GUI_DISPENSER_H

#include "GuiContainer.h"
#include "InventoryPlayer.h"
#include "TileEntityDispenser.h"

typedef struct GuiDispenser {
    GuiContainer base;
} GuiDispenser;

void GuiDispenser_construct(GuiDispenser *self, InventoryPlayer *var1, TileEntityDispenser *var2);

void GuiDispenser_drawGuiContainerForegroundLayer(GuiDispenser *self);

void GuiDispenser_drawGuiContainerBackgroundLayer(GuiDispenser *self, float var1);

#endif
