#include "ItemArmor.h"
#include <stdlib.h>

static const int damageReduceAmountArray[4] = {3, 8, 6, 3};
static const int maxDamageArray[4] = {11, 16, 15, 13};

static ItemVtable s_itemArmorVtable;
static int s_itemArmorVtableInited = 0;

Item *ItemArmor_create(int id, int armorLevel, int renderIndex, int armorType) {
    if (!s_itemArmorVtableInited) {
        s_itemArmorVtable = Item_defaultVtable;
        s_itemArmorVtableInited = 1;
    }

    ItemArmor *self = (ItemArmor *)calloc(1, sizeof(ItemArmor));
    Item_construct(&self->base, id);
    self->base.vtable = &s_itemArmorVtable;
    self->base.maxStackSize = 1;
    self->armorLevel = armorLevel;
    self->armorType = armorType;
    self->renderIndex = renderIndex;
    self->damageReduceAmount = damageReduceAmountArray[armorType];
    Item_setMaxDamage(&self->base, maxDamageArray[armorType] * 3 << armorLevel);
    return &self->base;
}

int ItemArmor_isInstance(Item *item) {
    return item != NULL && s_itemArmorVtableInited && item->vtable == &s_itemArmorVtable;
}
