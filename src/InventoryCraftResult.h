#ifndef INVENTORY_CRAFT_RESULT_H
#define INVENTORY_CRAFT_RESULT_H

#include "IInventory.h"

typedef struct InventoryCraftResult {
    IInventory base;
    ItemStack *stackResult[1];
} InventoryCraftResult;

void InventoryCraftResult_init(InventoryCraftResult *self);

int InventoryCraftResult_getSizeInventory(InventoryCraftResult *self);

ItemStack *InventoryCraftResult_getStackInSlot(InventoryCraftResult *self, int var1);

const char *InventoryCraftResult_getInvName(InventoryCraftResult *self);

ItemStack *InventoryCraftResult_decrStackSize(InventoryCraftResult *self, int var1, int var2);

void InventoryCraftResult_setInventorySlotContents(InventoryCraftResult *self, int var1,
                                                   ItemStack *var2);

int InventoryCraftResult_getInventoryStackLimit(InventoryCraftResult *self);

void InventoryCraftResult_onInventoryChanged(InventoryCraftResult *self);

int InventoryCraftResult_canInteractWith(InventoryCraftResult *self, EntityPlayer *var1);

#endif
