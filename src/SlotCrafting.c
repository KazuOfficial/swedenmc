#include "SlotCrafting.h"
#include "AchievementList.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include <stdlib.h>

static void sc_onPickupFromSlot(Slot *base, ItemStack *var1) {
    SlotCrafting_onPickupFromSlot((SlotCrafting *)base, var1);
}
static int sc_isItemValid(Slot *base, ItemStack *var1) {
    return SlotCrafting_isItemValid((SlotCrafting *)base, var1);
}
static const SlotVtable SlotCrafting_vtable = {sc_onPickupFromSlot, sc_isItemValid};

SlotCrafting *SlotCrafting_create(EntityPlayer *var1, IInventory *var2, IInventory *var3, int var4,
                                  int var5, int var6) {
    SlotCrafting *self = (SlotCrafting *)calloc(1, sizeof(SlotCrafting));
    Slot_init(&self->base, var3, var4, var5, var6);
    self->base.vtable = &SlotCrafting_vtable;
    self->thePlayer = var1;
    self->craftMatrix = var2;
    return self;
}

int SlotCrafting_isItemValid(SlotCrafting *self, ItemStack *var1) {
    (void)self;
    (void)var1;
    return 0;
}

void SlotCrafting_onPickupFromSlot(SlotCrafting *self, ItemStack *var1) {
    ItemStack_onCrafting(var1, self->thePlayer->base.base.worldObj, self->thePlayer);

    if (var1->itemID == Block_workbench->blockID) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_buildWorkBench, 1);
    } else if (var1->itemID == Item_pickaxeWood->shiftedIndex) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_buildPickaxe, 1);
    } else if (var1->itemID == Block_stoneOvenIdle->blockID) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_buildFurnace, 1);
    } else if (var1->itemID == Item_hoeWood->shiftedIndex) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_buildHoe, 1);
    } else if (var1->itemID == Item_bread->shiftedIndex) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_makeBread, 1);
    } else if (var1->itemID == Item_cake->shiftedIndex) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_bakeCake, 1);
    } else if (var1->itemID == Item_pickaxeStone->shiftedIndex) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_buildBetterPickaxe,
                             1);
    } else if (var1->itemID == Item_swordWood->shiftedIndex) {
        EntityPlayer_addStat(self->thePlayer, (struct StatBase *)AchievementList_buildSword, 1);
    }

    int var2;
    int size = self->craftMatrix->vtable->getSizeInventory(self->craftMatrix);
    for (var2 = 0; var2 < size; ++var2) {
        ItemStack *var3 = self->craftMatrix->vtable->getStackInSlot(self->craftMatrix, var2);
        if (var3 != NULL) {
            self->craftMatrix->vtable->decrStackSize(self->craftMatrix, var2, 1);

            Item *item = Item_itemsList[var3->itemID];
            if (item != NULL && item->vtable->hasContainerItem(item)) {
                Item *container = item->vtable->getContainerItem(item);
                ItemStack *newStack = ItemStack_new(container->shiftedIndex, 1, 0);
                self->craftMatrix->vtable->setInventorySlotContents(self->craftMatrix, var2,
                                                                    newStack);
            }
        }
    }
}
