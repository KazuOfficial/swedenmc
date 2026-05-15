#ifndef TILE_ENTITY_CHEST_H
#define TILE_ENTITY_CHEST_H

#include "TileEntity.h"
#include "IInventory.h"
#include "ItemStack.h"

#define CHEST_CONTENTS_SIZE 36
#define CHEST_SIZE 27

typedef struct TileEntityChest {
    TileEntity base;
    IInventory inventory;
    ItemStack chestContents[CHEST_CONTENTS_SIZE];
    int chestContentsHas[CHEST_CONTENTS_SIZE];
} TileEntityChest;

TileEntityChest *TileEntityChest_create(void);
ItemStack *TileEntityChest_getStackInSlot(TileEntityChest *self, int slot);
ItemStack *TileEntityChest_decrStackSize(TileEntityChest *self, int slot, int count);
void TileEntityChest_setSlotContents(TileEntityChest *self, int slot, ItemStack *stack);
int TileEntityChest_getSizeInventory(TileEntityChest *self);

#endif
