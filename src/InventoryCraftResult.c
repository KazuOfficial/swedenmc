#include "InventoryCraftResult.h"
#include <stdlib.h>
#include <string.h>

static int ii_getSizeInventory(IInventory *self) {
    return InventoryCraftResult_getSizeInventory((InventoryCraftResult *)self);
}
static ItemStack *ii_getStackInSlot(IInventory *self, int var1) {
    return InventoryCraftResult_getStackInSlot((InventoryCraftResult *)self, var1);
}
static ItemStack *ii_decrStackSize(IInventory *self, int var1, int var2) {
    return InventoryCraftResult_decrStackSize((InventoryCraftResult *)self, var1, var2);
}
static void ii_setInventorySlotContents(IInventory *self, int var1, ItemStack *var2) {
    InventoryCraftResult_setInventorySlotContents((InventoryCraftResult *)self, var1, var2);
}
static const char *ii_getInvName(IInventory *self) {
    return InventoryCraftResult_getInvName((InventoryCraftResult *)self);
}
static int ii_getInventoryStackLimit(IInventory *self) {
    return InventoryCraftResult_getInventoryStackLimit((InventoryCraftResult *)self);
}
static void ii_onInventoryChanged(IInventory *self) {
    InventoryCraftResult_onInventoryChanged((InventoryCraftResult *)self);
}
static int ii_canInteractWith(IInventory *self, EntityPlayer *var1) {
    return InventoryCraftResult_canInteractWith((InventoryCraftResult *)self, var1);
}

static const IInventoryVtable InventoryCraftResult_vtable = {
    .getSizeInventory = ii_getSizeInventory,
    .getStackInSlot = ii_getStackInSlot,
    .decrStackSize = ii_decrStackSize,
    .setInventorySlotContents = ii_setInventorySlotContents,
    .getInvName = ii_getInvName,
    .getInventoryStackLimit = ii_getInventoryStackLimit,
    .onInventoryChanged = ii_onInventoryChanged,
    .canInteractWith = ii_canInteractWith,
};

void InventoryCraftResult_init(InventoryCraftResult *self) {
    memset(self, 0, sizeof(InventoryCraftResult));
    self->base.vtable = &InventoryCraftResult_vtable;
}

int InventoryCraftResult_getSizeInventory(InventoryCraftResult *self) {
    (void)self;
    return 1;
}

ItemStack *InventoryCraftResult_getStackInSlot(InventoryCraftResult *self, int var1) {
    return self->stackResult[var1];
}

const char *InventoryCraftResult_getInvName(InventoryCraftResult *self) {
    (void)self;
    return "Result";
}

ItemStack *InventoryCraftResult_decrStackSize(InventoryCraftResult *self, int var1, int var2) {
    (void)var2;
    if (self->stackResult[var1] != NULL) {
        ItemStack *var3 = self->stackResult[var1];
        self->stackResult[var1] = NULL;
        return var3;
    } else {
        return NULL;
    }
}

void InventoryCraftResult_setInventorySlotContents(InventoryCraftResult *self, int var1,
                                                   ItemStack *var2) {
    self->stackResult[var1] = var2;
}

int InventoryCraftResult_getInventoryStackLimit(InventoryCraftResult *self) {
    (void)self;
    return 64;
}

void InventoryCraftResult_onInventoryChanged(InventoryCraftResult *self) { (void)self; }

int InventoryCraftResult_canInteractWith(InventoryCraftResult *self, EntityPlayer *var1) {
    (void)self;
    (void)var1;
    return 1;
}
