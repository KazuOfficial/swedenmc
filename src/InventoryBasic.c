#include "InventoryBasic.h"
#include "ItemStack.h"
#include <stdlib.h>
#include <string.h>

static int ii_getSizeInventory(IInventory *self) {
    return InventoryBasic_getSizeInventory((InventoryBasic *)self);
}
static ItemStack *ii_getStackInSlot(IInventory *self, int var1) {
    return InventoryBasic_getStackInSlot((InventoryBasic *)self, var1);
}
static ItemStack *ii_decrStackSize(IInventory *self, int var1, int var2) {
    return InventoryBasic_decrStackSize((InventoryBasic *)self, var1, var2);
}
static void ii_setInventorySlotContents(IInventory *self, int var1, ItemStack *var2) {
    InventoryBasic_setInventorySlotContents((InventoryBasic *)self, var1, var2);
}
static const char *ii_getInvName(IInventory *self) {
    return InventoryBasic_getInvName((InventoryBasic *)self);
}
static int ii_getInventoryStackLimit(IInventory *self) {
    return InventoryBasic_getInventoryStackLimit((InventoryBasic *)self);
}
static void ii_onInventoryChanged(IInventory *self) {
    InventoryBasic_onInventoryChanged((InventoryBasic *)self);
}
static int ii_canInteractWith(IInventory *self, EntityPlayer *var1) {
    return InventoryBasic_canInteractWith((InventoryBasic *)self, var1);
}

static const IInventoryVtable InventoryBasic_vtable = {
    .getSizeInventory = ii_getSizeInventory,
    .getStackInSlot = ii_getStackInSlot,
    .decrStackSize = ii_decrStackSize,
    .setInventorySlotContents = ii_setInventorySlotContents,
    .getInvName = ii_getInvName,
    .getInventoryStackLimit = ii_getInventoryStackLimit,
    .onInventoryChanged = ii_onInventoryChanged,
    .canInteractWith = ii_canInteractWith,
};

void InventoryBasic_init(InventoryBasic *self, const char *var1, int var2) {
    memset(self, 0, sizeof(InventoryBasic));
    self->base.vtable = &InventoryBasic_vtable;
    self->inventoryTitle = var1;
    self->slotsCount = var2;
    self->inventoryContents = (ItemStack **)calloc(var2, sizeof(ItemStack *));
}

InventoryBasic *InventoryBasic_create(const char *var1, int var2) {
    InventoryBasic *self = (InventoryBasic *)calloc(1, sizeof(InventoryBasic));
    InventoryBasic_init(self, var1, var2);
    return self;
}

ItemStack *InventoryBasic_getStackInSlot(InventoryBasic *self, int var1) {
    return self->inventoryContents[var1];
}

ItemStack *InventoryBasic_decrStackSize(InventoryBasic *self, int var1, int var2) {
    if (self->inventoryContents[var1] != NULL) {
        ItemStack *var3;
        if (self->inventoryContents[var1]->stackSize <= var2) {
            var3 = self->inventoryContents[var1];
            self->inventoryContents[var1] = NULL;
            InventoryBasic_onInventoryChanged(self);
            return var3;
        } else {
            ItemStack split = ItemStack_splitStack(self->inventoryContents[var1], var2);
            var3 = (ItemStack *)calloc(1, sizeof(ItemStack));
            *var3 = split;
            if (self->inventoryContents[var1]->stackSize == 0) {
                self->inventoryContents[var1] = NULL;
            }
            InventoryBasic_onInventoryChanged(self);
            return var3;
        }
    } else {
        return NULL;
    }
}

void InventoryBasic_setInventorySlotContents(InventoryBasic *self, int var1, ItemStack *var2) {
    self->inventoryContents[var1] = var2;
    if (var2 != NULL && var2->stackSize > InventoryBasic_getInventoryStackLimit(self)) {
        var2->stackSize = InventoryBasic_getInventoryStackLimit(self);
    }
    InventoryBasic_onInventoryChanged(self);
}

int InventoryBasic_getSizeInventory(InventoryBasic *self) { return self->slotsCount; }

const char *InventoryBasic_getInvName(InventoryBasic *self) { return self->inventoryTitle; }

int InventoryBasic_getInventoryStackLimit(InventoryBasic *self) {
    (void)self;
    return 64;
}

void InventoryBasic_onInventoryChanged(InventoryBasic *self) {
    if (self->field_20073_d != NULL) {
        for (int var1 = 0; var1 < self->field_20073_d_size; ++var1) {
            self->field_20073_d[var1]->vtable->func_20134_a(self->field_20073_d[var1], self);
        }
    }
}

int InventoryBasic_canInteractWith(InventoryBasic *self, EntityPlayer *var1) {
    (void)self;
    (void)var1;
    return 1;
}
