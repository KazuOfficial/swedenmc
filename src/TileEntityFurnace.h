#ifndef TILE_ENTITY_FURNACE_H
#define TILE_ENTITY_FURNACE_H

#include "TileEntity.h"
#include "IInventory.h"
#include "ItemStack.h"

#define FURNACE_SLOT_INPUT 0
#define FURNACE_SLOT_FUEL 1
#define FURNACE_SLOT_OUTPUT 2
#define FURNACE_SIZE 3

typedef struct TileEntityFurnace {
    TileEntity base;
    IInventory iinv;
    ItemStack furnaceItemStacks[FURNACE_SIZE];
    int furnaceItemStacksHas[FURNACE_SIZE];
    int furnaceBurnTime;
    int currentItemBurnTime;
    int furnaceCookTime;
} TileEntityFurnace;

static inline IInventory *TileEntityFurnace_asIInventory(TileEntityFurnace *self) {
    return &self->iinv;
}

TileEntityFurnace *TileEntityFurnace_create(void);
ItemStack *TileEntityFurnace_getStackInSlot(TileEntityFurnace *self, int slot);
void TileEntityFurnace_setSlotContents(TileEntityFurnace *self, int slot, ItemStack *stack);
int TileEntityFurnace_isBurning(TileEntityFurnace *self);
int TileEntityFurnace_getCookProgressScaled(TileEntityFurnace *self, int var1);
int TileEntityFurnace_getBurnTimeRemainingScaled(TileEntityFurnace *self, int var1);

int TileEntityFurnace_canInteractWith(TileEntityFurnace *self, struct EntityPlayer *var1);

#endif
