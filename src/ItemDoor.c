#include "ItemDoor.h"
#include "ItemStack.h"
#include "Block.h"
#include "Material.h"
#include "World.h"
#include "EntityPlayer.h"
#include "MathHelper.h"
#include <stdlib.h>

typedef struct {
    Item base;
    Material *doorMaterial;
} ItemDoor;

static int ItemDoor_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                              int var4, int var5, int var6, int var7) {
    if (var7 != 1) {
        return 0;
    } else {
        ++var5;
        Block *var8;
        if (((ItemDoor *)self)->doorMaterial == Material_wood) {
            var8 = Block_doorWood;
        } else {
            var8 = Block_doorSteel;
        }

        if (!var8->vtable->canPlaceBlockAt(var8, var3, var4, var5, var6)) {
            return 0;
        } else {
            int var9 = MathHelper_floor_double(
                           (double)((var2->base.base.rotationYaw + 180.0F) * 4.0F / 360.0F) - 0.5) &
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

            int var12 =
                (World_isBlockNormalCube(var3, var4 - var10, var5, var6 - var11) ? 1 : 0) +
                (World_isBlockNormalCube(var3, var4 - var10, var5 + 1, var6 - var11) ? 1 : 0);
            int var13 =
                (World_isBlockNormalCube(var3, var4 + var10, var5, var6 + var11) ? 1 : 0) +
                (World_isBlockNormalCube(var3, var4 + var10, var5 + 1, var6 + var11) ? 1 : 0);
            int var14 =
                (World_getBlockId(var3, var4 - var10, var5, var6 - var11) == var8->blockID ||
                 World_getBlockId(var3, var4 - var10, var5 + 1, var6 - var11) == var8->blockID)
                    ? 1
                    : 0;
            int var15 =
                (World_getBlockId(var3, var4 + var10, var5, var6 + var11) == var8->blockID ||
                 World_getBlockId(var3, var4 + var10, var5 + 1, var6 + var11) == var8->blockID)
                    ? 1
                    : 0;
            int var16 = 0;
            if (var14 && !var15) {
                var16 = 1;
            } else if (var13 > var12) {
                var16 = 1;
            }

            if (var16) {
                var9 = var9 - 1 & 3;
                var9 += 4;
            }

            var3->editingBlocks = 1;
            World_setBlockAndMetadataWithNotify(var3, var4, var5, var6, var8->blockID, var9);
            World_setBlockAndMetadataWithNotify(var3, var4, var5 + 1, var6, var8->blockID,
                                                var9 + 8);
            var3->editingBlocks = 0;
            World_notifyBlocksOfNeighborChange(var3, var4, var5, var6, var8->blockID);
            World_notifyBlocksOfNeighborChange(var3, var4, var5 + 1, var6, var8->blockID);
            --var1->stackSize;
            return 1;
        }
    }
}

Item *ItemDoor_create(int var1, Material *var2) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemDoor_onItemUse;
        vi = 1;
    }
    ItemDoor *self = (ItemDoor *)calloc(1, sizeof(ItemDoor));
    Item_construct(&self->base, var1);
    self->base.vtable = &vtable;
    self->doorMaterial = var2;
    self->base.maxStackSize = 1;
    return &self->base;
}
