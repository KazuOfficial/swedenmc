#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "World.h"
#include "Material.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "Vec3D.h"
#include "MovingObjectPosition.h"
#include "MathHelper.h"
#include "JavaMath.h"
#include "JavaRandom.h"
#include <stdlib.h>

typedef struct {
    Item base;
    int isFull;
} ItemBucket;

static ItemStack *ItemBucket_onItemRightClick(Item *self, ItemStack *var1, World *var2,
                                              EntityPlayer *var3) {
    ItemBucket *bucket = (ItemBucket *)self;
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
    double var21 = 5.0;
    float var20 = var14 * var16;
    Vec3D *var23 =
        Vec3D_addVector(var13, (double)var18 * var21, (double)var17 * var21, (double)var20 * var21);
    MovingObjectPosition *var24 = World_rayTraceBlocks_do(var2, var13, var23, bucket->isFull == 0);
    if (var24 == NULL) {
        return var1;
    }
    ItemStack *result = var1;
    if (var24->typeOfHit == EnumMovingObjectType_TILE) {
        int var25 = var24->blockX;
        int var26 = var24->blockY;
        int var27 = var24->blockZ;
        if (World_func_6466_a(var2, var3, var25, var26, var27)) {
            if (bucket->isFull == 0) {
                if (World_getBlockMaterial(var2, var25, var26, var27) == Material_water &&
                    World_getBlockMetadata(var2, var25, var26, var27) == 0) {
                    World_setBlockWithNotify(var2, var25, var26, var27, 0);
                    result = ItemStack_new(Item_bucketWater->shiftedIndex, 1, 0);
                } else if (World_getBlockMaterial(var2, var25, var26, var27) == Material_lava &&
                           World_getBlockMetadata(var2, var25, var26, var27) == 0) {
                    World_setBlockWithNotify(var2, var25, var26, var27, 0);
                    result = ItemStack_new(Item_bucketLava->shiftedIndex, 1, 0);
                }
            } else {
                if (bucket->isFull < 0) {
                    result = ItemStack_new(Item_bucketEmpty->shiftedIndex, 1, 0);
                } else {
                    if (var24->sideHit == 0) {
                        --var26;
                    }
                    if (var24->sideHit == 1) {
                        ++var26;
                    }
                    if (var24->sideHit == 2) {
                        --var27;
                    }
                    if (var24->sideHit == 3) {
                        ++var27;
                    }
                    if (var24->sideHit == 4) {
                        --var25;
                    }
                    if (var24->sideHit == 5) {
                        ++var25;
                    }
                    if (World_isAirBlock(var2, var25, var26, var27) ||
                        !Material_isSolid(World_getBlockMaterial(var2, var25, var26, var27))) {
                        if (var2->worldProvider->isHellWorld &&
                            bucket->isFull == Block_waterMoving->blockID) {
                            World_playSoundEffect(var2, var7 + 0.5, var9 + 0.5, var11 + 0.5,
                                                  "random.fizz", 0.5f,
                                                  2.6f + (JavaRandom_nextFloat(&var2->rand) -
                                                          JavaRandom_nextFloat(&var2->rand)) *
                                                             0.8f);
                            for (int var28 = 0; var28 < 8; ++var28) {
                                World_spawnParticle(var2, "largesmoke",
                                                    (double)var25 + Math_random(),
                                                    (double)var26 + Math_random(),
                                                    (double)var27 + Math_random(), 0.0, 0.0, 0.0);
                            }
                        } else {
                            World_setBlockAndMetadataWithNotify(var2, var25, var26, var27,
                                                                bucket->isFull, 0);
                        }
                        result = ItemStack_new(Item_bucketEmpty->shiftedIndex, 1, 0);
                    }
                }
            }
        }
    } else if (bucket->isFull == 0 && var24->entityHit != NULL && var24->entityHit->isCow) {
        result = ItemStack_new(Item_bucketMilk->shiftedIndex, 1, 0);
    }
    MovingObjectPosition_free(var24);
    return result;
}

Item *ItemBucket_create(int id, int isFull) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemRightClick = ItemBucket_onItemRightClick;
        vi = 1;
    }

    ItemBucket *self = (ItemBucket *)calloc(1, sizeof(ItemBucket));
    Item_construct(&self->base, id);
    self->base.vtable = &vtable;
    self->base.maxStackSize = 1;
    self->isFull = isFull;
    return &self->base;
}
