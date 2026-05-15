#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "World.h"
#include "StepSound.h"
#include "EntityLiving.h"
#include <stdlib.h>

typedef struct {
    Item base;
    int field_320_a;
} ItemReed;

static int ItemReed_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                              int var4, int var5, int var6, int var7) {
    ItemReed *r = (ItemReed *)self;
    if (World_getBlockId(var3, var4, var5, var6) == Block_snow->blockID) {
        var7 = 0;
    } else {
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
    }
    if (var1->stackSize == 0) {
        return 0;
    } else {
        if (World_canBlockBePlacedAt(var3, r->field_320_a, var4, var5, var6, 0, var7)) {
            Block *var8 = Block_blocksList[r->field_320_a];
            if (World_setBlockWithNotify(var3, var4, var5, var6, r->field_320_a)) {
                Block_blocksList[r->field_320_a]->vtable->onBlockPlaced(
                    Block_blocksList[r->field_320_a], var3, var4, var5, var6, var7);
                Block_blocksList[r->field_320_a]->vtable->onBlockPlacedBy(
                    Block_blocksList[r->field_320_a], var3, var4, var5, var6, (EntityLiving *)var2);
                World_playSoundEffect(var3, (double)((float)var4 + 0.5f),
                                      (double)((float)var5 + 0.5f), (double)((float)var6 + 0.5f),
                                      StepSound_func_1145_d(var8->stepSound),
                                      (StepSound_getVolume(var8->stepSound) + 1.0f) / 2.0f,
                                      StepSound_getPitch(var8->stepSound) * 0.8f);
                --var1->stackSize;
            }
        }
        return 1;
    }
}

Item *ItemReed_create(int id, int blockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemReed_onItemUse;
        vi = 1;
    }

    ItemReed *self = (ItemReed *)calloc(1, sizeof(ItemReed));
    Item_construct(&self->base, id);
    self->base.vtable = &vtable;
    self->field_320_a = blockID;
    return &self->base;
}
