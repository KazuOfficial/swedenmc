#ifndef GUI_CRAFTING_H
#define GUI_CRAFTING_H

#include "GuiContainer.h"
#include "InventoryPlayer.h"
#include "World.h"

typedef struct GuiCrafting {
    GuiContainer base;
} GuiCrafting;

void GuiCrafting_construct(GuiCrafting *self, InventoryPlayer *var1, World *var2, int var3,
                           int var4, int var5);

void GuiCrafting_onGuiClosed(GuiCrafting *self);

void GuiCrafting_drawGuiContainerForegroundLayer(GuiCrafting *self);

void GuiCrafting_drawGuiContainerBackgroundLayer(GuiCrafting *self, float var1);

#endif
