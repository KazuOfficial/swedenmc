#include "Item.h"
#include "ItemStack.h"
#include "World.h"
#include "EntityPlayer.h"
#include "EntityBoat.h"
#include "Entity.h"
#include "Vec3D.h"
#include "MovingObjectPosition.h"
#include "MathHelper.h"
#include "Block.h"
#include <stdlib.h>

static ItemStack *ItemBoat_onItemRightClick(Item *self, ItemStack *var1, World *var2,
                                            EntityPlayer *var3) {
    (void)self;
    float var4 = 1.0f;
    float var5 = var3->base.base.prevRotationPitch +
                 (var3->base.base.rotationPitch - var3->base.base.prevRotationPitch) * var4;
    float var6 = var3->base.base.prevRotationYaw +
                 (var3->base.base.rotationYaw - var3->base.base.prevRotationYaw) * var4;
    double var7 =
        var3->base.base.prevPosX + (var3->base.base.posX - var3->base.base.prevPosX) * (double)var4;
    double var9 = var3->base.base.prevPosY +
                  (var3->base.base.posY - var3->base.base.prevPosY) * (double)var4 + 1.62 -
                  (double)var3->base.base.yOffset;
    double var11 =
        var3->base.base.prevPosZ + (var3->base.base.posZ - var3->base.base.prevPosZ) * (double)var4;
    Vec3D *var13 = Vec3D_createVector(var7, var9, var11);
    float var14 = MathHelper_cos(-var6 * 0.01745329f - 3.141593f);
    float var15 = MathHelper_sin(-var6 * 0.01745329f - 3.141593f);
    float var16 = -MathHelper_cos(-var5 * 0.01745329f);
    float var17 = MathHelper_sin(-var5 * 0.01745329f);
    float var18 = var15 * var16;
    float var20 = var14 * var16;
    double var21 = 5.0;
    Vec3D *var23 =
        Vec3D_addVector(var13, (double)var18 * var21, (double)var17 * var21, (double)var20 * var21);
    MovingObjectPosition *var24 = World_rayTraceBlocks_do(var2, var13, var23, 1);
    if (var24 == NULL) {
        return var1;
    } else {
        if (var24->typeOfHit == EnumMovingObjectType_TILE) {
            int var25 = var24->blockX;
            int var26 = var24->blockY;
            int var27 = var24->blockZ;
            if (!var2->multiplayerWorld) {
                if (World_getBlockId(var2, var25, var26, var27) == Block_snow->blockID) {
                    --var26;
                }
                EntityBoat *boat = (EntityBoat *)calloc(1, sizeof(EntityBoat));
                EntityBoat_construct2(boat, var2, (double)((float)var25 + 0.5f),
                                      (double)((float)var26 + 1.0f), (double)((float)var27 + 0.5f));
                World_entityJoinedWorld(var2, (Entity *)boat);
            }
            --var1->stackSize;
        }
        return var1;
    }
}

Item *ItemBoat_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemRightClick = ItemBoat_onItemRightClick;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 1;
    return self;
}
