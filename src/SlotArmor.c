#include "SlotArmor.h"
#include "ItemArmor.h"
#include "Block.h"
#include "Item.h"
#include <stdlib.h>

static void sa_onPickupFromSlot(Slot *base, ItemStack *var1) {
    (void)base;
    (void)var1;
    Slot_onSlotChanged(base);
}
static int sa_isItemValid(Slot *base, ItemStack *var1) {
    return SlotArmor_isItemValid((SlotArmor *)base, var1);
}
static const SlotVtable SlotArmor_vtable = {sa_onPickupFromSlot, sa_isItemValid};

SlotArmor *SlotArmor_create(ContainerPlayer *var1, IInventory *var2, int var3, int var4, int var5,
                            int var6) {
    SlotArmor *self = (SlotArmor *)calloc(1, sizeof(SlotArmor));
    Slot_init(&self->base, var2, var3, var4, var5);
    self->base.vtable = &SlotArmor_vtable;
    self->inventory = var1;
    self->armorType = var6;
    return self;
}

int SlotArmor_getSlotStackLimit(SlotArmor *self) {
    (void)self;
    return 1;
}

int SlotArmor_isItemValid(SlotArmor *self, ItemStack *var1) {
    if (var1 == NULL)
        return 0;
    Item *item = Item_itemsList[var1->itemID];
    if (item == NULL)
        return 0;

    if (ItemArmor_isInstance(item)) {

        ItemArmor *armor = (ItemArmor *)item;
        return armor->armorType == self->armorType;
    } else {

        return (item->shiftedIndex == Block_pumpkin->blockID) ? (self->armorType == 0) : 0;
    }
}
