#ifndef INVENTORY_CRAFTING_H
#define INVENTORY_CRAFTING_H

#include "IInventory.h"
#include "Container.h"

typedef struct InventoryCrafting {
    IInventory base;
    ItemStack **stackList;
    int stackListLength;
    int field_21104_b;
    Container *eventHandler;
} InventoryCrafting;

void InventoryCrafting_init(InventoryCrafting *self, Container *var1, int var2, int var3);

int InventoryCrafting_getSizeInventory(InventoryCrafting *self);

ItemStack *InventoryCrafting_getStackInSlot(InventoryCrafting *self, int var1);

ItemStack *InventoryCrafting_func_21103_b(InventoryCrafting *self, int var1, int var2);

const char *InventoryCrafting_getInvName(InventoryCrafting *self);

ItemStack *InventoryCrafting_decrStackSize(InventoryCrafting *self, int var1, int var2);

void InventoryCrafting_setInventorySlotContents(InventoryCrafting *self, int var1, ItemStack *var2);

int InventoryCrafting_getInventoryStackLimit(InventoryCrafting *self);

void InventoryCrafting_onInventoryChanged(InventoryCrafting *self);

int InventoryCrafting_canInteractWith(InventoryCrafting *self, EntityPlayer *var1);

#endif
