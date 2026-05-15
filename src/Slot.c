#include "Slot.h"

#include <stdlib.h>

static void def_onPickupFromSlot(Slot *self, ItemStack *var1) {
    (void)var1;
    Slot_onSlotChanged(self);
}
static int def_isItemValid(Slot *self, ItemStack *var1) {
    (void)self;
    (void)var1;
    return 1;
}

static const SlotVtable Slot_defaultVtable = {
    def_onPickupFromSlot,
    def_isItemValid,
};

void Slot_init(Slot *self, IInventory *var1, int var2, int var3, int var4) {
    self->vtable = &Slot_defaultVtable;
    self->inventory = var1;
    self->slotIndex = var2;
    self->xDisplayPosition = var3;
    self->yDisplayPosition = var4;
    self->slotNumber = 0;
}

Slot *Slot_create(IInventory *var1, int var2, int var3, int var4) {
    Slot *self = (Slot *)calloc(1, sizeof(Slot));
    Slot_init(self, var1, var2, var3, var4);
    return self;
}

void Slot_onPickupFromSlot(Slot *self, ItemStack *var1) {
    self->vtable->onPickupFromSlot(self, var1);
}

int Slot_isItemValid(Slot *self, ItemStack *var1) { return self->vtable->isItemValid(self, var1); }

ItemStack *Slot_getStack(Slot *self) {
    return self->inventory->vtable->getStackInSlot(self->inventory, self->slotIndex);
}

int Slot_getHasStack(Slot *self) { return Slot_getStack(self) != NULL; }

void Slot_putStack(Slot *self, ItemStack *var1) {
    self->inventory->vtable->setInventorySlotContents(self->inventory, self->slotIndex, var1);
    Slot_onSlotChanged(self);
}

void Slot_onSlotChanged(Slot *self) {
    self->inventory->vtable->onInventoryChanged(self->inventory);
}

int Slot_getSlotStackLimit(Slot *self) {
    return self->inventory->vtable->getInventoryStackLimit(self->inventory);
}

int Slot_getBackgroundIconIndex(Slot *self) {
    (void)self;
    return -1;
}

ItemStack *Slot_decrStackSize(Slot *self, int var1) {
    return self->inventory->vtable->decrStackSize(self->inventory, self->slotIndex, var1);
}
