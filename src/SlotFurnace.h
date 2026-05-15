#ifndef SLOT_FURNACE_H
#define SLOT_FURNACE_H

#include "Slot.h"
#include "IInventory.h"
#include "EntityPlayer.h"

typedef struct SlotFurnace {
    Slot base;
    EntityPlayer *thePlayer;
} SlotFurnace;

SlotFurnace *SlotFurnace_create(EntityPlayer *var1, IInventory *var2, int var3, int var4, int var5);

int SlotFurnace_isItemValid(SlotFurnace *self, ItemStack *var1);

void SlotFurnace_onPickupFromSlot(SlotFurnace *self, ItemStack *var1);

#endif
