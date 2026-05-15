#ifndef IINVENTORY_H
#define IINVENTORY_H

#include "GameForward.h"

struct IInventory;

typedef struct IInventoryVtable {

    int (*getSizeInventory)(struct IInventory *self);

    ItemStack *(*getStackInSlot)(struct IInventory *self, int var1);

    ItemStack *(*decrStackSize)(struct IInventory *self, int var1, int var2);

    void (*setInventorySlotContents)(struct IInventory *self, int var1, ItemStack *var2);

    const char *(*getInvName)(struct IInventory *self);

    int (*getInventoryStackLimit)(struct IInventory *self);

    void (*onInventoryChanged)(struct IInventory *self);

    int (*canInteractWith)(struct IInventory *self, EntityPlayer *var1);
} IInventoryVtable;

typedef struct IInventory {
    const IInventoryVtable *vtable;
} IInventory;

#endif
