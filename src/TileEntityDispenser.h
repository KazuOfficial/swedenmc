#ifndef TILE_ENTITY_DISPENSER_H
#define TILE_ENTITY_DISPENSER_H

#include "TileEntity.h"
#include "IInventory.h"
#include "ItemStack.h"
#include "JavaRandom.h"

#define DISPENSER_SIZE 9

typedef struct TileEntityDispenser {
    TileEntity base;
    IInventory iinv;
    ItemStack dispenserItemStacks[DISPENSER_SIZE];
    int dispenserItemStacksHas[DISPENSER_SIZE];
    JavaRandom dispenserRandom;
} TileEntityDispenser;

TileEntityDispenser *TileEntityDispenser_create(void);
ItemStack *TileEntityDispenser_getStackInSlot(TileEntityDispenser *self, int slot);

ItemStack *TileEntityDispenser_decrStackSize(TileEntityDispenser *self, int slot, int amount);

ItemStack *TileEntityDispenser_getRandomStackFromInventory(TileEntityDispenser *self);

int TileEntityDispenser_canInteractWith(TileEntityDispenser *self, struct EntityPlayer *var1);

#endif
