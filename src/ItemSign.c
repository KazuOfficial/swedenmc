#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "World.h"
#include "Material.h"
#include "MathHelper.h"
#include "EntityPlayer.h"
#include "Entity.h"
#include "TileEntitySign.h"
#include <stdlib.h>

static int ItemSign_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                              int var4, int var5, int var6, int var7) {
    (void)self;
    if (var7 == 0) {
        return 0;
    } else if (!Material_isSolid(World_getBlockMaterial(var3, var4, var5, var6))) {
        return 0;
    } else {
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
        if (!Block_signPost->vtable->canPlaceBlockAt(Block_signPost, var3, var4, var5, var6)) {
            return 0;
        } else {
            if (var7 == 1) {
                World_setBlockAndMetadataWithNotify(
                    var3, var4, var5, var6, Block_signPost->blockID,
                    MathHelper_floor_double(
                        (double)(((Entity *)var2)->rotationYaw + 180.0f) * 16.0f / 360.0f + 0.5) &
                        15);
            } else {
                World_setBlockAndMetadataWithNotify(var3, var4, var5, var6, Block_signWall->blockID,
                                                    var7);
            }
            --var1->stackSize;
            TileEntitySign *var8 =
                (TileEntitySign *)World_getBlockTileEntity(var3, var4, var5, var6);
            if (var8 != NULL) {
                EntityPlayer_displayGUIEditSign(var2, var8);
            }
            return 1;
        }
    }
}

Item *ItemSign_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemSign_onItemUse;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 1;
    return self;
}
