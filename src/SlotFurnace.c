#include "SlotFurnace.h"
#include "AchievementList.h"
#include "Item.h"
#include "ItemStack.h"
#include <stdlib.h>

static void sf_onPickupFromSlot(Slot *base, ItemStack *var1) {
    SlotFurnace_onPickupFromSlot((SlotFurnace *)base, var1);
}
static int sf_isItemValid(Slot *base, ItemStack *var1) {
    return SlotFurnace_isItemValid((SlotFurnace *)base, var1);
}
static const SlotVtable SlotFurnace_vtable = {sf_onPickupFromSlot, sf_isItemValid};

SlotFurnace *SlotFurnace_create(EntityPlayer *var1, IInventory *var2, int var3, int var4,
                                int var5) {
    SlotFurnace *self = (SlotFurnace *)calloc(1, sizeof(SlotFurnace));
    Slot_init(&self->base, var2, var3, var4, var5);
    self->base.vtable = &SlotFurnace_vtable;
    self->thePlayer = var1;
    return self;
}

int SlotFurnace_isItemValid(SlotFurnace *self, ItemStack *var1) {
    (void)self;
    (void)var1;
    return 0;
}

void SlotFurnace_onPickupFromSlot(SlotFurnace *self, ItemStack *var1) {
    ItemStack_onCrafting(var1, self->thePlayer->base.base.worldObj, self->thePlayer);

    if (var1->itemID == Item_ingotIron->shiftedIndex) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_acquireIron, 1);
    }

    if (var1->itemID == Item_fishCooked->shiftedIndex) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_cookFish, 1);
    }

    Slot_onSlotChanged(&self->base);
}
