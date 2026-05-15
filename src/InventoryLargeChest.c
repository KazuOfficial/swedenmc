#include "InventoryLargeChest.h"
#include <string.h>

#define INV_getSizeInventory(inv) ((inv)->vtable->getSizeInventory(inv))
#define INV_getStackInSlot(inv, v1) ((inv)->vtable->getStackInSlot(inv, v1))
#define INV_decrStackSize(inv, v1, v2) ((inv)->vtable->decrStackSize(inv, v1, v2))
#define INV_setInventorySlotContents(inv, v1, v2)                                                  \
    ((inv)->vtable->setInventorySlotContents(inv, v1, v2))
#define INV_getInventoryStackLimit(inv) ((inv)->vtable->getInventoryStackLimit(inv))
#define INV_onInventoryChanged(inv) ((inv)->vtable->onInventoryChanged(inv))
#define INV_canInteractWith(inv, v1) ((inv)->vtable->canInteractWith(inv, v1))

static int ii_getSizeInventory(IInventory *self) {
    return InventoryLargeChest_getSizeInventory((InventoryLargeChest *)self);
}
static ItemStack *ii_getStackInSlot(IInventory *self, int var1) {
    return InventoryLargeChest_getStackInSlot((InventoryLargeChest *)self, var1);
}
static ItemStack *ii_decrStackSize(IInventory *self, int var1, int var2) {
    return InventoryLargeChest_decrStackSize((InventoryLargeChest *)self, var1, var2);
}
static void ii_setInventorySlotContents(IInventory *self, int var1, ItemStack *var2) {
    InventoryLargeChest_setInventorySlotContents((InventoryLargeChest *)self, var1, var2);
}
static const char *ii_getInvName(IInventory *self) {
    return InventoryLargeChest_getInvName((InventoryLargeChest *)self);
}
static int ii_getInventoryStackLimit(IInventory *self) {
    return InventoryLargeChest_getInventoryStackLimit((InventoryLargeChest *)self);
}
static void ii_onInventoryChanged(IInventory *self) {
    InventoryLargeChest_onInventoryChanged((InventoryLargeChest *)self);
}
static int ii_canInteractWith(IInventory *self, EntityPlayer *var1) {
    return InventoryLargeChest_canInteractWith((InventoryLargeChest *)self, var1);
}

static const IInventoryVtable InventoryLargeChest_vtable = {
    .getSizeInventory = ii_getSizeInventory,
    .getStackInSlot = ii_getStackInSlot,
    .decrStackSize = ii_decrStackSize,
    .setInventorySlotContents = ii_setInventorySlotContents,
    .getInvName = ii_getInvName,
    .getInventoryStackLimit = ii_getInventoryStackLimit,
    .onInventoryChanged = ii_onInventoryChanged,
    .canInteractWith = ii_canInteractWith,
};

int InventoryLargeChest_isInstance(IInventory *inv) {
    return inv && inv->vtable == &InventoryLargeChest_vtable;
}

void InventoryLargeChest_init(InventoryLargeChest *self, const char *var1, IInventory *var2,
                              IInventory *var3) {
    memset(self, 0, sizeof(InventoryLargeChest));
    self->base.vtable = &InventoryLargeChest_vtable;
    self->name = var1;
    self->upperChest = var2;
    self->lowerChest = var3;
}

int InventoryLargeChest_getSizeInventory(InventoryLargeChest *self) {
    return INV_getSizeInventory(self->upperChest) + INV_getSizeInventory(self->lowerChest);
}

const char *InventoryLargeChest_getInvName(InventoryLargeChest *self) { return self->name; }

ItemStack *InventoryLargeChest_getStackInSlot(InventoryLargeChest *self, int var1) {
    return var1 >= INV_getSizeInventory(self->upperChest)
               ? INV_getStackInSlot(self->lowerChest, var1 - INV_getSizeInventory(self->upperChest))
               : INV_getStackInSlot(self->upperChest, var1);
}

ItemStack *InventoryLargeChest_decrStackSize(InventoryLargeChest *self, int var1, int var2) {
    return var1 >= INV_getSizeInventory(self->upperChest)
               ? INV_decrStackSize(self->lowerChest, var1 - INV_getSizeInventory(self->upperChest),
                                   var2)
               : INV_decrStackSize(self->upperChest, var1, var2);
}

void InventoryLargeChest_setInventorySlotContents(InventoryLargeChest *self, int var1,
                                                  ItemStack *var2) {
    if (var1 >= INV_getSizeInventory(self->upperChest)) {
        INV_setInventorySlotContents(self->lowerChest,
                                     var1 - INV_getSizeInventory(self->upperChest), var2);
    } else {
        INV_setInventorySlotContents(self->upperChest, var1, var2);
    }
}

int InventoryLargeChest_getInventoryStackLimit(InventoryLargeChest *self) {
    return INV_getInventoryStackLimit(self->upperChest);
}

void InventoryLargeChest_onInventoryChanged(InventoryLargeChest *self) {
    INV_onInventoryChanged(self->upperChest);
    INV_onInventoryChanged(self->lowerChest);
}

int InventoryLargeChest_canInteractWith(InventoryLargeChest *self, EntityPlayer *var1) {
    return INV_canInteractWith(self->upperChest, var1) &&
           INV_canInteractWith(self->lowerChest, var1);
}
