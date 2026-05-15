#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "BlockBed.h"
#include "World.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "MathHelper.h"
#include <stdlib.h>

static int ItemBed_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3, int var4,
                             int var5, int var6, int var7) {
    (void)self;
    if (var7 != 1) {
        return 0;
    } else {
        ++var5;
        BlockBed *var8 = (BlockBed *)Block_blockBed;
        int var9 =
            MathHelper_floor_double((double)(((Entity *)var2)->rotationYaw * 4.0f / 360.0f) + 0.5) &
            3;
        int var10 = 0;
        int var11 = 0;
        if (var9 == 0) {
            var11 = 1;
        }
        if (var9 == 1) {
            var10 = -1;
        }
        if (var9 == 2) {
            var11 = -1;
        }
        if (var9 == 3) {
            var10 = 1;
        }
        if (World_isAirBlock(var3, var4, var5, var6) &&
            World_isAirBlock(var3, var4 + var10, var5, var6 + var11) &&
            World_isBlockNormalCube(var3, var4, var5 - 1, var6) &&
            World_isBlockNormalCube(var3, var4 + var10, var5 - 1, var6 + var11)) {
            World_setBlockAndMetadataWithNotify(var3, var4, var5, var6, var8->base.blockID, var9);
            World_setBlockAndMetadataWithNotify(var3, var4 + var10, var5, var6 + var11,
                                                var8->base.blockID, var9 + 8);
            --var1->stackSize;
            return 1;
        } else {
            return 0;
        }
    }
}

Item *ItemBed_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemBed_onItemUse;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    return self;
}
