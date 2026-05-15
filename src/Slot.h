#ifndef SLOT_H
#define SLOT_H

#include "IInventory.h"
#include "ItemStack.h"

typedef struct Slot Slot;

typedef struct SlotVtable {
    void (*onPickupFromSlot)(Slot *self, ItemStack *var1);
    int (*isItemValid)(Slot *self, ItemStack *var1);
} SlotVtable;

struct Slot {
    const SlotVtable *vtable;

    int slotIndex;

    IInventory *inventory;

    int slotNumber;

    int xDisplayPosition;

    int yDisplayPosition;
};

void Slot_init(Slot *self, IInventory *var1, int var2, int var3, int var4);

Slot *Slot_create(IInventory *var1, int var2, int var3, int var4);

void Slot_onPickupFromSlot(Slot *self, ItemStack *var1);

int Slot_isItemValid(Slot *self, ItemStack *var1);

ItemStack *Slot_getStack(Slot *self);

int Slot_getHasStack(Slot *self);

void Slot_putStack(Slot *self, ItemStack *var1);

void Slot_onSlotChanged(Slot *self);

int Slot_getSlotStackLimit(Slot *self);

int Slot_getBackgroundIconIndex(Slot *self);

ItemStack *Slot_decrStackSize(Slot *self, int var1);

#endif
