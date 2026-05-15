#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "World.h"
#include "StepSound.h"
#include "EnumToolMaterial.h"
#include <stdlib.h>

static int ItemHoe_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3, int var4,
                             int var5, int var6, int var7) {
    (void)self;
    int var8 = World_getBlockId(var3, var4, var5, var6);
    int var9 = World_getBlockId(var3, var4, var5 + 1, var6);
    if ((var7 == 0 || var9 != 0 || var8 != Block_grass->blockID) && var8 != Block_dirt->blockID) {
        return 0;
    } else {
        Block *var10 = Block_tilledField;
        World_playSoundEffect(var3, (double)((float)var4 + 0.5f), (double)((float)var5 + 0.5f),
                              (double)((float)var6 + 0.5f), StepSound_func_1145_d(var10->stepSound),
                              (StepSound_getVolume(var10->stepSound) + 1.0f) / 2.0f,
                              StepSound_getPitch(var10->stepSound) * 0.8f);
        if (var3->multiplayerWorld) {
            return 1;
        } else {
            World_setBlockWithNotify(var3, var4, var5, var6, var10->blockID);
            ItemStack_damageItem(var1, 1, (struct Entity *)var2);
            return 1;
        }
    }
}

static int ItemHoe_isFull3D(Item *self) {
    (void)self;
    return 1;
}

Item *ItemHoe_create(int id, EnumToolMaterial mat) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemHoe_onItemUse;
        vtable.isFull3D = ItemHoe_isFull3D;
        vi = 1;
    }
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 1;
    Item_setMaxDamage(self, EnumToolMaterial_getMaxUses(mat));
    return self;
}
