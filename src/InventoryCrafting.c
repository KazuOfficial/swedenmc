#include "InventoryCrafting.h"
#include "ItemStack.h"
#include <stdlib.h>
#include <string.h>

static int ii_getSizeInventory(IInventory *self) {
    return InventoryCrafting_getSizeInventory((InventoryCrafting *)self);
}
static ItemStack *ii_getStackInSlot(IInventory *self, int var1) {
    return InventoryCrafting_getStackInSlot((InventoryCrafting *)self, var1);
}
static ItemStack *ii_decrStackSize(IInventory *self, int var1, int var2) {
    return InventoryCrafting_decrStackSize((InventoryCrafting *)self, var1, var2);
}
static void ii_setInventorySlotContents(IInventory *self, int var1, ItemStack *var2) {
    InventoryCrafting_setInventorySlotContents((InventoryCrafting *)self, var1, var2);
}
static const char *ii_getInvName(IInventory *self) {
    return InventoryCrafting_getInvName((InventoryCrafting *)self);
}
static int ii_getInventoryStackLimit(IInventory *self) {
    return InventoryCrafting_getInventoryStackLimit((InventoryCrafting *)self);
}
static void ii_onInventoryChanged(IInventory *self) {
    InventoryCrafting_onInventoryChanged((InventoryCrafting *)self);
}
static int ii_canInteractWith(IInventory *self, EntityPlayer *var1) {
    return InventoryCrafting_canInteractWith((InventoryCrafting *)self, var1);
}

static const IInventoryVtable InventoryCrafting_vtable = {
    .getSizeInventory = ii_getSizeInventory,
    .getStackInSlot = ii_getStackInSlot,
    .decrStackSize = ii_decrStackSize,
    .setInventorySlotContents = ii_setInventorySlotContents,
    .getInvName = ii_getInvName,
    .getInventoryStackLimit = ii_getInventoryStackLimit,
    .onInventoryChanged = ii_onInventoryChanged,
    .canInteractWith = ii_canInteractWith,
};

void InventoryCrafting_init(InventoryCrafting *self, Container *var1, int var2, int var3) {
    memset(self, 0, sizeof(InventoryCrafting));
    self->base.vtable = &InventoryCrafting_vtable;
    int var4 = var2 * var3;
    self->stackList = (ItemStack **)calloc(var4, sizeof(ItemStack *));
    self->stackListLength = var4;
    self->eventHandler = var1;
    self->field_21104_b = var2;
}

int InventoryCrafting_getSizeInventory(InventoryCrafting *self) { return self->stackListLength; }

ItemStack *InventoryCrafting_getStackInSlot(InventoryCrafting *self, int var1) {
    return var1 >= InventoryCrafting_getSizeInventory(self) ? NULL : self->stackList[var1];
}

ItemStack *InventoryCrafting_func_21103_b(InventoryCrafting *self, int var1, int var2) {
    if (var1 >= 0 && var1 < self->field_21104_b) {
        int var3 = var1 + var2 * self->field_21104_b;
        return InventoryCrafting_getStackInSlot(self, var3);
    } else {
        return NULL;
    }
}

const char *InventoryCrafting_getInvName(InventoryCrafting *self) {
    (void)self;
    return "Crafting";
}

ItemStack *InventoryCrafting_decrStackSize(InventoryCrafting *self, int var1, int var2) {
    if (self->stackList[var1] != NULL) {
        ItemStack *var3;
        if (self->stackList[var1]->stackSize <= var2) {
            var3 = self->stackList[var1];
            self->stackList[var1] = NULL;
            Container_onCraftMatrixChanged_v(self->eventHandler, (IInventory *)self);
            return var3;
        } else {
            ItemStack split = ItemStack_splitStack(self->stackList[var1], var2);
            var3 = (ItemStack *)calloc(1, sizeof(ItemStack));
            *var3 = split;
            if (self->stackList[var1]->stackSize == 0) {
                self->stackList[var1] = NULL;
            }
            Container_onCraftMatrixChanged_v(self->eventHandler, (IInventory *)self);
            return var3;
        }
    } else {
        return NULL;
    }
}

void InventoryCrafting_setInventorySlotContents(InventoryCrafting *self, int var1,
                                                ItemStack *var2) {
    self->stackList[var1] = var2;
    Container_onCraftMatrixChanged_v(self->eventHandler, (IInventory *)self);
}

int InventoryCrafting_getInventoryStackLimit(InventoryCrafting *self) {
    (void)self;
    return 64;
}

void InventoryCrafting_onInventoryChanged(InventoryCrafting *self) { (void)self; }

int InventoryCrafting_canInteractWith(InventoryCrafting *self, EntityPlayer *var1) {
    (void)self;
    (void)var1;
    return 1;
}
