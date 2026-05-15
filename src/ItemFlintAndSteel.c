#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "World.h"
#include "EntityPlayer.h"
#include <stdlib.h>

static int ItemFlintAndSteel_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                                       int var4, int var5, int var6, int var7) {
    (void)self;
    if (var7 == 0) {
        --var5;
    }
    if (var7 == 1) {
        ++var5;
    }
    if (var7 == 2) {
        --var6;
    }
    if (var7 == 3) {
        ++var6;
    }
    if (var7 == 4) {
        --var4;
    }
    if (var7 == 5) {
        ++var4;
    }
    int var8 = World_getBlockId(var3, var4, var5, var6);
    if (var8 == 0) {
        World_playSoundEffect(var3, (double)var4 + 0.5, (double)var5 + 0.5, (double)var6 + 0.5,
                              "fire.ignite", 1.0f,
                              JavaRandom_nextFloat(&Item_itemRand) * 0.4f + 0.8f);
        World_setBlockWithNotify(var3, var4, var5, var6, Block_fire->blockID);
    }
    ItemStack_damageItem(var1, 1, (struct Entity *)var2);
    return 1;
}

Item *ItemFlintAndSteel_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemFlintAndSteel_onItemUse;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 1;
    Item_setMaxDamage(self, 64);
    return self;
}
