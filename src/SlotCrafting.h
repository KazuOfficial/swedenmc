#ifndef SLOT_CRAFTING_H
#define SLOT_CRAFTING_H

#include "Slot.h"
#include "IInventory.h"
#include "EntityPlayer.h"

typedef struct SlotCrafting {
    Slot base;
    IInventory *craftMatrix;
    EntityPlayer *thePlayer;
} SlotCrafting;

SlotCrafting *SlotCrafting_create(EntityPlayer *var1, IInventory *var2, IInventory *var3, int var4,
                                  int var5, int var6);

int SlotCrafting_isItemValid(SlotCrafting *self, ItemStack *var1);

void SlotCrafting_onPickupFromSlot(SlotCrafting *self, ItemStack *var1);

#endif
