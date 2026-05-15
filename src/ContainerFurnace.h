#ifndef CONTAINER_FURNACE_H
#define CONTAINER_FURNACE_H

#include "Container.h"
#include "TileEntityFurnace.h"
#include "InventoryPlayer.h"

typedef struct ContainerFurnace {
    Container base;

    TileEntityFurnace *furnace;

    int cookTime;

    int burnTime;

    int itemBurnTime;
} ContainerFurnace;

void ContainerFurnace_construct(ContainerFurnace *self, InventoryPlayer *var1,
                                TileEntityFurnace *var2);

void ContainerFurnace_updateCraftingResults(ContainerFurnace *self);

void ContainerFurnace_func_20112_a(ContainerFurnace *self, int var1, int var2);

int ContainerFurnace_isUsableByPlayer(ContainerFurnace *self, EntityPlayer *var1);

ItemStack *ContainerFurnace_getStackInSlot(ContainerFurnace *self, int var1);

#endif
