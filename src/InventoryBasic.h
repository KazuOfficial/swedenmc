#ifndef INVENTORY_BASIC_H
#define INVENTORY_BASIC_H

#include "IInventory.h"
#include "IInvBasic.h"

struct InventoryBasic {
    IInventory base;
    const char *inventoryTitle;
    int slotsCount;
    ItemStack **inventoryContents;

    IInvBasic **field_20073_d;
    int field_20073_d_size;
};

InventoryBasic *InventoryBasic_create(const char *var1, int var2);
void InventoryBasic_init(InventoryBasic *self, const char *var1, int var2);

ItemStack *InventoryBasic_getStackInSlot(InventoryBasic *self, int var1);

ItemStack *InventoryBasic_decrStackSize(InventoryBasic *self, int var1, int var2);

void InventoryBasic_setInventorySlotContents(InventoryBasic *self, int var1, ItemStack *var2);

int InventoryBasic_getSizeInventory(InventoryBasic *self);

const char *InventoryBasic_getInvName(InventoryBasic *self);

int InventoryBasic_getInventoryStackLimit(InventoryBasic *self);

void InventoryBasic_onInventoryChanged(InventoryBasic *self);

int InventoryBasic_canInteractWith(InventoryBasic *self, EntityPlayer *var1);

#endif
