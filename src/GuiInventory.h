#ifndef GUI_INVENTORY_H
#define GUI_INVENTORY_H

#include "GuiContainer.h"
#include "EntityPlayer.h"

typedef struct GuiInventory {
    GuiContainer base;

    float xSize_lo;

    float ySize_lo;
} GuiInventory;

void GuiInventory_construct(GuiInventory *self, EntityPlayer *var1);

void GuiInventory_initGui(GuiScreen *self_);

void GuiInventory_drawGuiContainerForegroundLayer(GuiScreen *self_);

void GuiInventory_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

void GuiInventory_drawGuiContainerBackgroundLayer(GuiScreen *self_, float var1);

void GuiInventory_actionPerformed(GuiScreen *self_, GuiButton *var1);

#endif
