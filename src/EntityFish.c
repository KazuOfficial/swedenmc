#include "EntityFish.h"
#include "World.h"
#include "Item.h"
#include "ItemStack.h"
#include "MathHelper.h"
#include "Vec3D.h"
#include "MovingObjectPosition.h"
#include "AxisAlignedBB.h"
#include "Material.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "StatList.h"
#include "NBTTagCompound.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

static EntityVtable EntityFish_vtable;
static int EntityFish_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityFish_vtable_init)
        return;
    Entity_initVtable(&EntityFish_vtable);
    EntityFish_vtable.entityInit = EntityFish_entityInit;
    EntityFish_vtable.isInRangeToRenderDist = EntityFish_isInRangeToRenderDist;
    EntityFish_vtable.onUpdate = EntityFish_onUpdate;
    EntityFish_vtable.writeEntityToNBT = EntityFish_writeEntityToNBT;
    EntityFish_vtable.readEntityFromNBT = EntityFish_readEntityFromNBT;
    EntityFish_vtable.getShadowSize = EntityFish_getShadowSize;
    EntityFish_vtable.setPositionAndRotation2 = EntityFish_setPositionAndRotation2;
    EntityFish_vtable.setVelocity = EntityFish_setVelocity;
    EntityFish_vtable_init = 1;
}

static void EntityFish_initCommon(EntityFish *self) {
    self->xTile = -1;
    self->yTile = -1;
    self->zTile = -1;
    self->inTile = 0;
    self->inGround = 0;
    self->shake = 0;
    self->ticksInAir = 0;
    self->ticksCatchable = 0;
    self->bobber = NULL;
    Entity_setSize(&self->base, 0.25F, 0.25F);
    self->base.ignoreFrustumCheck = 1;
}

void EntityFish_construct(EntityFish *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityFish_vtable;
    EntityFish_initCommon(self);
    self->base.isFish = 1;
}

EntityFish *EntityFish_create(World *var1) {
    EntityFish *self = (EntityFish *)calloc(1, sizeof(EntityFish));
    EntityFish_construct(self, var1);
    return self;
}

void EntityFish_construct2(EntityFish *self, World *var1, double var2, double var4, double var6) {
    EntityFish_construct(self, var1);
    self->base.vtable->setPosition(&self->base, var2, var4, var6);
    self->base.ignoreFrustumCheck = 1;
}

EntityFish *EntityFish_create2(World *var1, double var2, double var4, double var6) {
    EntityFish *self = (EntityFish *)calloc(1, sizeof(EntityFish));
    EntityFish_construct2(self, var1, var2, var4, var6);
    return self;
}

void EntityFish_construct3(EntityFish *self, World *var1, EntityPlayer *var2) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityFish_vtable;
    self->xTile = -1;
    self->yTile = -1;
    self->zTile = -1;
    self->inTile = 0;
    self->inGround = 0;
    self->shake = 0;
    self->ticksInAir = 0;
    self->ticksCatchable = 0;
    self->bobber = NULL;
    self->base.ignoreFrustumCheck = 1;
    self->angler = var2;
    self->angler->fishEntity = self;
    Entity_setSize(&self->base, 0.25F, 0.25F);
    self->base.vtable->setLocationAndAngles(
        &self->base, var2->base.base.posX,
        var2->base.base.posY + 1.62 - (double)var2->base.base.yOffset, var2->base.base.posZ,
        var2->base.base.rotationYaw, var2->base.base.rotationPitch);
    self->base.posX -=
        (double)(MathHelper_cos(self->base.rotationYaw / 180.0F * (float)M_PI) * 0.16F);
    self->base.posY -= (double)0.1F;
    self->base.posZ -=
        (double)(MathHelper_sin(self->base.rotationYaw / 180.0F * (float)M_PI) * 0.16F);
    self->base.vtable->setPosition(&self->base, self->base.posX, self->base.posY, self->base.posZ);
    self->base.yOffset = 0.0F;
    float var3 = 0.4F;
    self->base.motionX =
        (double)(-MathHelper_sin(self->base.rotationYaw / 180.0F * (float)M_PI) *
                 MathHelper_cos(self->base.rotationPitch / 180.0F * (float)M_PI) * var3);
    self->base.motionZ =
        (double)(MathHelper_cos(self->base.rotationYaw / 180.0F * (float)M_PI) *
                 MathHelper_cos(self->base.rotationPitch / 180.0F * (float)M_PI) * var3);
    self->base.motionY =
        (double)(-MathHelper_sin(self->base.rotationPitch / 180.0F * (float)M_PI) * var3);
    EntityFish_func_4042_a(self, self->base.motionX, self->base.motionY, self->base.motionZ, 1.5F,
                           1.0F);
}

EntityFish *EntityFish_create3(World *var1, EntityPlayer *var2) {
    EntityFish *self = (EntityFish *)calloc(1, sizeof(EntityFish));
    EntityFish_construct3(self, var1, var2);
    return self;
}

void EntityFish_entityInit(Entity *self) { (void)self; }

int EntityFish_isInRangeToRenderDist(Entity *self, double var1) {
    double var3 = AxisAlignedBB_getAverageEdgeLength(&self->boundingBox) * 4.0;
    var3 *= 64.0;
    return var1 < var3 * var3;
}

void EntityFish_func_4042_a(EntityFish *self, double var1, double var3, double var5, float var7,
                            float var8) {
    float var9 = MathHelper_sqrt_double(var1 * var1 + var3 * var3 + var5 * var5);
    var1 /= (double)var9;
    var3 /= (double)var9;
    var5 /= (double)var9;
    var1 += JavaRandom_nextGaussian(&self->base.rand) * (double)0.0075F * (double)var8;
    var3 += JavaRandom_nextGaussian(&self->base.rand) * (double)0.0075F * (double)var8;
    var5 += JavaRandom_nextGaussian(&self->base.rand) * (double)0.0075F * (double)var8;
    var1 *= (double)var7;
    var3 *= (double)var7;
    var5 *= (double)var7;
    self->base.motionX = var1;
    self->base.motionY = var3;
    self->base.motionZ = var5;
    float var10 = MathHelper_sqrt_double(var1 * var1 + var5 * var5);
    self->base.prevRotationYaw = self->base.rotationYaw =
        (float)(atan2(var1, var5) * 180.0 / (double)((float)M_PI));
    self->base.prevRotationPitch = self->base.rotationPitch =
        (float)(atan2(var3, (double)var10) * 180.0 / (double)((float)M_PI));
    self->ticksInGround = 0;
}

void EntityFish_setPositionAndRotation2(Entity *selfE, double var1, double var3, double var5,
                                        float var7, float var8, int var9) {
    EntityFish *self = (EntityFish *)selfE;
    self->field_6387_m = var1;
    self->field_6386_n = var3;
    self->field_6385_o = var5;
    self->field_6384_p = (double)var7;
    self->field_6383_q = (double)var8;
    self->field_6388_l = var9;
    self->base.motionX = self->velocityX;
    self->base.motionY = self->velocityY;
    self->base.motionZ = self->velocityZ;
}

void EntityFish_setVelocity(Entity *selfE, double var1, double var3, double var5) {
    EntityFish *self = (EntityFish *)selfE;
    self->velocityX = self->base.motionX = var1;
    self->velocityY = self->base.motionY = var3;
    self->velocityZ = self->base.motionZ = var5;
}

void EntityFish_onUpdate(Entity *selfE) {
    EntityFish *self = (EntityFish *)selfE;
    Entity_defaultVtable.onUpdate(&self->base);

    if (self->field_6388_l > 0) {
        double var21 =
            self->base.posX + (self->field_6387_m - self->base.posX) / (double)self->field_6388_l;
        double var22 =
            self->base.posY + (self->field_6386_n - self->base.posY) / (double)self->field_6388_l;
        double var23 =
            self->base.posZ + (self->field_6385_o - self->base.posZ) / (double)self->field_6388_l;

        double var7;
        for (var7 = self->field_6384_p - (double)self->base.rotationYaw; var7 < -180.0;
             var7 += 360.0) {
        }

        while (var7 >= 180.0) {
            var7 -= 360.0;
        }

        self->base.rotationYaw =
            (float)((double)self->base.rotationYaw + var7 / (double)self->field_6388_l);
        self->base.rotationPitch = (float)((double)self->base.rotationPitch +
                                           (self->field_6383_q - (double)self->base.rotationPitch) /
                                               (double)self->field_6388_l);
        --self->field_6388_l;
        self->base.vtable->setPosition(&self->base, var21, var22, var23);
        Entity_setRotation(&self->base, self->base.rotationYaw, self->base.rotationPitch);
    } else {
        if (!self->base.worldObj->isRemote) {
            ItemStack *var1 = EntityPlayer_getCurrentEquippedItem(self->angler);
            if (self->angler->base.base.isDead ||
                !self->base.vtable->isEntityAlive((Entity *)self->angler) || var1 == NULL ||
                var1->itemID != Item_fishingRod->shiftedIndex ||
                Entity_getDistanceSqToEntity(&self->base, (Entity *)self->angler) > 1024.0) {
                Entity_setEntityDead(&self->base);
                self->angler->fishEntity = NULL;
                return;
            }

            if (self->bobber != NULL) {
                if (!self->bobber->isDead) {
                    self->base.posX = self->bobber->posX;
                    self->base.posY =
                        self->bobber->boundingBox.minY + (double)self->bobber->height * 0.8;
                    self->base.posZ = self->bobber->posZ;
                    return;
                }

                self->bobber = NULL;
            }
        }

        if (self->shake > 0) {
            --self->shake;
        }

        if (self->inGround) {
            int var19 = self->base.worldObj->vtable->getBlockId(self->base.worldObj, self->xTile,
                                                                self->yTile, self->zTile);
            if (var19 == self->inTile) {
                ++self->ticksInGround;
                if (self->ticksInGround == 1200) {
                    Entity_setEntityDead(&self->base);
                }
                return;
            }

            self->inGround = 0;
            self->base.motionX *= (double)(JavaRandom_nextFloat(&self->base.rand) * 0.2F);
            self->base.motionY *= (double)(JavaRandom_nextFloat(&self->base.rand) * 0.2F);
            self->base.motionZ *= (double)(JavaRandom_nextFloat(&self->base.rand) * 0.2F);
            self->ticksInGround = 0;
            self->ticksInAir = 0;
        } else {
            ++self->ticksInAir;
        }

        Vec3D *var20 = Vec3D_createVector(self->base.posX, self->base.posY, self->base.posZ);
        Vec3D *var2 = Vec3D_createVector(self->base.posX + self->base.motionX,
                                         self->base.posY + self->base.motionY,
                                         self->base.posZ + self->base.motionZ);
        MovingObjectPosition *var3 = World_rayTraceBlocks(self->base.worldObj, var20, var2);
        var20 = Vec3D_createVector(self->base.posX, self->base.posY, self->base.posZ);
        var2 = Vec3D_createVector(self->base.posX + self->base.motionX,
                                  self->base.posY + self->base.motionY,
                                  self->base.posZ + self->base.motionZ);
        if (var3 != NULL) {
            var2 = Vec3D_createVector(var3->hitVec->xCoord, var3->hitVec->yCoord,
                                      var3->hitVec->zCoord);
        }

        Entity *var4 = NULL;
        AxisAlignedBB *searchBB =
            AxisAlignedBB_expand(AxisAlignedBB_addCoord(&self->base.boundingBox, self->base.motionX,
                                                        self->base.motionY, self->base.motionZ),
                                 1.0, 1.0, 1.0);
        Entity *nearby[256];
        int var5_count = World_getEntitiesWithinAABBExcludingEntity(
            self->base.worldObj, &self->base, searchBB, nearby, 256);
        double var6 = 0.0;

        double var13 = 0.0;
        for (int var8 = 0; var8 < var5_count; ++var8) {
            Entity *var9 = nearby[var8];
            if (var9->vtable->canBeCollidedWith(var9) &&
                (var9 != (Entity *)self->angler || self->ticksInAir >= 5)) {
                float var10 = 0.3F;
                AxisAlignedBB *var11 = AxisAlignedBB_expand(&var9->boundingBox, (double)var10,
                                                            (double)var10, (double)var10);
                MovingObjectPosition *var12 = AxisAlignedBB_func_1169_a(var11, var20, var2);
                if (var12 != NULL) {
                    var13 = Vec3D_distanceTo(var20, var12->hitVec);
                    if (var13 < var6 || var6 == 0.0) {
                        var4 = var9;
                        var6 = var13;
                    }
                }
            }
        }

        if (var4 != NULL) {
            var3 = MovingObjectPosition_createEntity(var4);
        }

        if (var3 != NULL) {
            if (var3->entityHit != NULL) {
                if (var3->entityHit->vtable->attackEntityFrom(var3->entityHit,
                                                              (Entity *)self->angler, 0)) {
                    self->bobber = var3->entityHit;
                }
            } else {
                self->inGround = 1;
            }
        }

        if (!self->inGround) {
            self->base.vtable->moveEntity(&self->base, self->base.motionX, self->base.motionY,
                                          self->base.motionZ);
            float var24 = MathHelper_sqrt_double(self->base.motionX * self->base.motionX +
                                                 self->base.motionZ * self->base.motionZ);
            self->base.rotationYaw = (float)(atan2(self->base.motionX, self->base.motionZ) * 180.0 /
                                             (double)((float)M_PI));

            for (self->base.rotationPitch = (float)(atan2(self->base.motionY, (double)var24) *
                                                    180.0 / (double)((float)M_PI));
                 self->base.rotationPitch - self->base.prevRotationPitch < -180.0F;
                 self->base.prevRotationPitch -= 360.0F) {
            }

            while (self->base.rotationPitch - self->base.prevRotationPitch >= 180.0F) {
                self->base.prevRotationPitch += 360.0F;
            }

            while (self->base.rotationYaw - self->base.prevRotationYaw < -180.0F) {
                self->base.prevRotationYaw -= 360.0F;
            }

            while (self->base.rotationYaw - self->base.prevRotationYaw >= 180.0F) {
                self->base.prevRotationYaw += 360.0F;
            }

            self->base.rotationPitch =
                self->base.prevRotationPitch +
                (self->base.rotationPitch - self->base.prevRotationPitch) * 0.2F;
            self->base.rotationYaw = self->base.prevRotationYaw +
                                     (self->base.rotationYaw - self->base.prevRotationYaw) * 0.2F;
            float var25 = 0.92F;
            if (self->base.onGround || self->base.isCollidedHorizontally) {
                var25 = 0.5F;
            }

            int8_t var26 = 5;
            double var27 = 0.0;

            for (int var28 = 0; var28 < var26; ++var28) {
                double var14 = self->base.boundingBox.minY +
                               (self->base.boundingBox.maxY - self->base.boundingBox.minY) *
                                   (double)(var28 + 0) / (double)var26 -
                               0.125 + 0.125;
                double var16 = self->base.boundingBox.minY +
                               (self->base.boundingBox.maxY - self->base.boundingBox.minY) *
                                   (double)(var28 + 1) / (double)var26 -
                               0.125 + 0.125;
                AxisAlignedBB *var18 = AxisAlignedBB_getBoundingBoxFromPool(
                    self->base.boundingBox.minX, var14, self->base.boundingBox.minZ,
                    self->base.boundingBox.maxX, var16, self->base.boundingBox.maxZ);
                if (World_isAABBInMaterial(self->base.worldObj, var18, Material_water)) {
                    var27 += 1.0 / (double)var26;
                }
            }

            if (var27 > 0.0) {
                if (self->ticksCatchable > 0) {
                    --self->ticksCatchable;
                } else {
                    short var29 = 500;
                    if (World_canBlockBeRainedOn(self->base.worldObj,
                                                 MathHelper_floor_double(self->base.posX),
                                                 MathHelper_floor_double(self->base.posY) + 1,
                                                 MathHelper_floor_double(self->base.posZ))) {
                        var29 = 300;
                    }

                    if (JavaRandom_nextInt(&self->base.rand, var29) == 0) {
                        self->ticksCatchable = JavaRandom_nextInt(&self->base.rand, 30) + 10;
                        self->base.motionY -= (double)0.2F;
                        World_playSoundAtEntity(self->base.worldObj, &self->base, "random.splash",
                                                0.25F,
                                                1.0F + (JavaRandom_nextFloat(&self->base.rand) -
                                                        JavaRandom_nextFloat(&self->base.rand)) *
                                                           0.4F);
                        float var30 = (float)MathHelper_floor_double(self->base.boundingBox.minY);

                        int var15;
                        float var17;
                        float var31;
                        for (var15 = 0; (float)var15 < 1.0F + self->base.width * 20.0F; ++var15) {
                            var31 = (JavaRandom_nextFloat(&self->base.rand) * 2.0F - 1.0F) *
                                    self->base.width;
                            var17 = (JavaRandom_nextFloat(&self->base.rand) * 2.0F - 1.0F) *
                                    self->base.width;
                            World_spawnParticle(
                                self->base.worldObj, "bubble", self->base.posX + (double)var31,
                                (double)(var30 + 1.0F), self->base.posZ + (double)var17,
                                self->base.motionX,
                                self->base.motionY -
                                    (double)(JavaRandom_nextFloat(&self->base.rand) * 0.2F),
                                self->base.motionZ);
                        }

                        for (var15 = 0; (float)var15 < 1.0F + self->base.width * 20.0F; ++var15) {
                            var31 = (JavaRandom_nextFloat(&self->base.rand) * 2.0F - 1.0F) *
                                    self->base.width;
                            var17 = (JavaRandom_nextFloat(&self->base.rand) * 2.0F - 1.0F) *
                                    self->base.width;
                            World_spawnParticle(
                                self->base.worldObj, "splash", self->base.posX + (double)var31,
                                (double)(var30 + 1.0F), self->base.posZ + (double)var17,
                                self->base.motionX, self->base.motionY, self->base.motionZ);
                        }
                    }
                }
            }

            if (self->ticksCatchable > 0) {
                self->base.motionY -= (double)(JavaRandom_nextFloat(&self->base.rand) *
                                               JavaRandom_nextFloat(&self->base.rand) *
                                               JavaRandom_nextFloat(&self->base.rand)) *
                                      0.2;
            }

            var13 = var27 * 2.0 - 1.0;
            self->base.motionY += (double)0.04F * var13;
            if (var27 > 0.0) {
                var25 = (float)((double)var25 * 0.9);
                self->base.motionY *= 0.8;
            }

            self->base.motionX *= (double)var25;
            self->base.motionY *= (double)var25;
            self->base.motionZ *= (double)var25;
            self->base.vtable->setPosition(&self->base, self->base.posX, self->base.posY,
                                           self->base.posZ);
        }
    }
}

void EntityFish_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityFish *self = (EntityFish *)selfE;
    NBTTagCompound_setShort(var1, "xTile", (int16_t)self->xTile);
    NBTTagCompound_setShort(var1, "yTile", (int16_t)self->yTile);
    NBTTagCompound_setShort(var1, "zTile", (int16_t)self->zTile);
    NBTTagCompound_setByte(var1, "inTile", (int8_t)self->inTile);
    NBTTagCompound_setByte(var1, "shake", (int8_t)self->shake);
    NBTTagCompound_setByte(var1, "inGround", (int8_t)(self->inGround ? 1 : 0));
}

void EntityFish_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityFish *self = (EntityFish *)selfE;
    self->xTile = NBTTagCompound_getShort(var1, "xTile");
    self->yTile = NBTTagCompound_getShort(var1, "yTile");
    self->zTile = NBTTagCompound_getShort(var1, "zTile");
    self->inTile = NBTTagCompound_getByte(var1, "inTile") & 255;
    self->shake = NBTTagCompound_getByte(var1, "shake") & 255;
    self->inGround = NBTTagCompound_getByte(var1, "inGround") == 1;
}

float EntityFish_getShadowSize(Entity *self) {
    (void)self;
    return 0.0F;
}

int EntityFish_catchFish(EntityFish *self) {
    int8_t var1 = 0;
    if (self->bobber != NULL) {
        double var2 = self->angler->base.base.posX - self->base.posX;
        double var4 = self->angler->base.base.posY - self->base.posY;
        double var6 = self->angler->base.base.posZ - self->base.posZ;
        double var8 = (double)MathHelper_sqrt_double(var2 * var2 + var4 * var4 + var6 * var6);
        double var10 = 0.1;
        self->bobber->motionX += var2 * var10;
        self->bobber->motionY += var4 * var10 + (double)MathHelper_sqrt_double(var8) * 0.08;
        self->bobber->motionZ += var6 * var10;
        var1 = 3;
    } else if (self->ticksCatchable > 0) {
        ItemStack fishStack = ItemStack_fromItemID(Item_fishRaw->shiftedIndex, 1, 0);
        EntityItem *var13 = EntityItem_create(self->base.worldObj, self->base.posX, self->base.posY,
                                              self->base.posZ, fishStack);
        double var3 = self->angler->base.base.posX - self->base.posX;
        double var5 = self->angler->base.base.posY - self->base.posY;
        double var7 = self->angler->base.base.posZ - self->base.posZ;
        double var9 = (double)MathHelper_sqrt_double(var3 * var3 + var5 * var5 + var7 * var7);
        double var11 = 0.1;
        var13->base.motionX = var3 * var11;
        var13->base.motionY = var5 * var11 + (double)MathHelper_sqrt_double(var9) * 0.08;
        var13->base.motionZ = var7 * var11;
        World_entityJoinedWorld(self->base.worldObj, (Entity *)var13);
        EntityPlayer_addStat(self->angler, StatList_fishCaughtStat, 1);
        var1 = 1;
    }

    if (self->inGround) {
        var1 = 2;
    }

    Entity_setEntityDead(&self->base);
    self->angler->fishEntity = NULL;
    return var1;
}
