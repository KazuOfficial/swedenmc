#ifndef SLOT_ARMOR_H
#define SLOT_ARMOR_H

#include "Slot.h"
#include "ContainerPlayer.h"

typedef struct SlotArmor {
    Slot base;
    int armorType;
    ContainerPlayer *inventory;
} SlotArmor;

SlotArmor *SlotArmor_create(ContainerPlayer *var1, IInventory *var2, int var3, int var4, int var5,
                            int var6);

int SlotArmor_getSlotStackLimit(SlotArmor *self);

int SlotArmor_isItemValid(SlotArmor *self, ItemStack *var1);

#endif
