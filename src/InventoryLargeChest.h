#ifndef INVENTORY_LARGE_CHEST_H
#define INVENTORY_LARGE_CHEST_H

#include "IInventory.h"

typedef struct InventoryLargeChest {
    IInventory base;
    const char *name;
    IInventory *upperChest;
    IInventory *lowerChest;
} InventoryLargeChest;

int InventoryLargeChest_isInstance(IInventory *inv);

void InventoryLargeChest_init(InventoryLargeChest *self, const char *var1, IInventory *var2,
                              IInventory *var3);

int InventoryLargeChest_getSizeInventory(InventoryLargeChest *self);

const char *InventoryLargeChest_getInvName(InventoryLargeChest *self);

ItemStack *InventoryLargeChest_getStackInSlot(InventoryLargeChest *self, int var1);

ItemStack *InventoryLargeChest_decrStackSize(InventoryLargeChest *self, int var1, int var2);

void InventoryLargeChest_setInventorySlotContents(InventoryLargeChest *self, int var1,
                                                  ItemStack *var2);

int InventoryLargeChest_getInventoryStackLimit(InventoryLargeChest *self);

void InventoryLargeChest_onInventoryChanged(InventoryLargeChest *self);

int InventoryLargeChest_canInteractWith(InventoryLargeChest *self, EntityPlayer *var1);

#endif
