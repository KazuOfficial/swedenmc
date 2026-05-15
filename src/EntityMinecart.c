#include "EntityMinecart.h"
#include "BlockRail.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "IInventory.h"
#include "MathHelper.h"
#include "World.h"
#include "NBTTagList.h"
#include "Vec3D.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int field_855_j[10][2][3] = {{{0, 0, -1}, {0, 0, 1}},  {{-1, 0, 0}, {1, 0, 0}},
                                          {{-1, -1, 0}, {1, 0, 0}}, {{-1, 0, 0}, {1, -1, 0}},
                                          {{0, 0, -1}, {0, -1, 1}}, {{0, -1, -1}, {0, 0, 1}},
                                          {{0, 0, 1}, {1, 0, 0}},   {{0, 0, 1}, {-1, 0, 0}},
                                          {{0, 0, -1}, {-1, 0, 0}}, {{0, 0, -1}, {1, 0, 0}}};

static EntityVtable EntityMinecart_vtable;
static int EntityMinecart_vtable_init = 0;

static const char *minecart_getEntityString(Entity *self) {
    (void)self;
    return "Minecart";
}

static void ensure_vtable(void) {
    if (EntityMinecart_vtable_init)
        return;
    Entity_initVtable(&EntityMinecart_vtable);
    EntityMinecart_vtable.getEntityString = minecart_getEntityString;
    EntityMinecart_vtable.setEntityDead = EntityMinecart_setEntityDead;
    EntityMinecart_vtable.canTriggerWalking = EntityMinecart_canTriggerWalking;
    EntityMinecart_vtable.entityInit = EntityMinecart_entityInit;
    EntityMinecart_vtable.getCollisionBox = EntityMinecart_getCollisionBox;
    EntityMinecart_vtable.getBoundingBox = EntityMinecart_getBoundingBox;
    EntityMinecart_vtable.canBePushed = EntityMinecart_canBePushed;
    EntityMinecart_vtable.getMountedYOffset = EntityMinecart_getMountedYOffset;
    EntityMinecart_vtable.attackEntityFrom = EntityMinecart_attackEntityFrom;
    EntityMinecart_vtable.performHurtAnimation = EntityMinecart_performHurtAnimation;
    EntityMinecart_vtable.canBeCollidedWith = EntityMinecart_canBeCollidedWith;
    EntityMinecart_vtable.onUpdate = EntityMinecart_onUpdate;
    EntityMinecart_vtable.getShadowSize = EntityMinecart_getShadowSize;
    EntityMinecart_vtable.applyEntityCollision = EntityMinecart_applyEntityCollision;
    EntityMinecart_vtable.interact = EntityMinecart_interact;
    EntityMinecart_vtable.writeEntityToNBT = EntityMinecart_writeEntityToNBT;
    EntityMinecart_vtable.readEntityFromNBT = EntityMinecart_readEntityFromNBT;
    EntityMinecart_vtable.setPositionAndRotation2 = EntityMinecart_setPositionAndRotation2;
    EntityMinecart_vtable.setVelocity = EntityMinecart_setVelocity;
    EntityMinecart_vtable_init = 1;
}

void EntityMinecart_construct1(EntityMinecart *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityMinecart_vtable;
    memset(self->cargoItems, 0, sizeof(self->cargoItems));
    self->minecartCurrentDamage = 0;
    self->minecartTimeSinceHit = 0;
    self->minecartRockDirection = 1;
    self->field_856_i = 0;
    self->base.preventEntitySpawning = 1;
    self->base.isMinecart = 1;
    Entity_setSize(&self->base, 0.98F, 0.7F);
    self->base.yOffset = self->base.height / 2.0F;
}

void EntityMinecart_construct2(EntityMinecart *self, World *var1, double var2, double var4,
                               double var6, int var8) {
    EntityMinecart_construct1(self, var1);
    self->base.vtable->setPosition(&self->base, var2, var4 + (double)self->base.yOffset, var6);
    self->base.motionX = 0.0;
    self->base.motionY = 0.0;
    self->base.motionZ = 0.0;
    self->base.prevPosX = var2;
    self->base.prevPosY = var4;
    self->base.prevPosZ = var6;
    self->minecartType = var8;
}

EntityMinecart *EntityMinecart_create(World *var1, double var2, double var4, double var6,
                                      int var8) {
    EntityMinecart *self = (EntityMinecart *)calloc(1, sizeof(EntityMinecart));
    EntityMinecart_construct2(self, var1, var2, var4, var6, var8);
    return self;
}

int EntityMinecart_canTriggerWalking(Entity *self) {
    (void)self;
    return 0;
}

void EntityMinecart_entityInit(Entity *self) { (void)self; }

AxisAlignedBB *EntityMinecart_getCollisionBox(Entity *self, Entity *var1) {
    (void)self;
    return &var1->boundingBox;
}

AxisAlignedBB *EntityMinecart_getBoundingBox(Entity *self) {
    (void)self;
    return NULL;
}

int EntityMinecart_canBePushed(Entity *self) {
    (void)self;
    return 1;
}

double EntityMinecart_getMountedYOffset(Entity *self) {
    return (double)self->height * 0.0 - (double)0.3F;
}

void EntityMinecart_setEntityDead(Entity *selfE) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    for (int var1 = 0; var1 < EntityMinecart_getSizeInventory(self); ++var1) {
        ItemStack *var2 = EntityMinecart_getStackInSlot(self, var1);
        if (var2 != NULL) {
            float var3 = JavaRandom_nextFloat(&selfE->rand) * 0.8F + 0.1F;
            float var4 = JavaRandom_nextFloat(&selfE->rand) * 0.8F + 0.1F;
            float var5 = JavaRandom_nextFloat(&selfE->rand) * 0.8F + 0.1F;
            while (var2->stackSize > 0) {
                int var6 = JavaRandom_nextInt(&selfE->rand, 21) + 10;
                if (var6 > var2->stackSize) {
                    var6 = var2->stackSize;
                }
                var2->stackSize -= var6;
                EntityItem *var7 = EntityItem_create(
                    selfE->worldObj, selfE->posX + (double)var3, selfE->posY + (double)var4,
                    selfE->posZ + (double)var5,
                    ItemStack_fromItemID(var2->itemID, var6, ItemStack_getItemDamage(var2)));
                float var8 = 0.05F;
                var7->base.motionX = (double)((float)JavaRandom_nextGaussian(&selfE->rand) * var8);
                var7->base.motionY =
                    (double)((float)JavaRandom_nextGaussian(&selfE->rand) * var8 + 0.2F);
                var7->base.motionZ = (double)((float)JavaRandom_nextGaussian(&selfE->rand) * var8);
                World_entityJoinedWorld(selfE->worldObj, &var7->base);
            }
        }
    }

    Entity_defaultVtable.setEntityDead(selfE);
}

void EntityMinecart_performHurtAnimation(Entity *selfE) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    printf("Animating hurt\n");
    self->minecartRockDirection = -self->minecartRockDirection;
    self->minecartTimeSinceHit = 10;
    self->minecartCurrentDamage += self->minecartCurrentDamage * 10;
}

int EntityMinecart_canBeCollidedWith(Entity *selfE) { return !selfE->isDead; }

int EntityMinecart_attackEntityFrom(Entity *selfE, Entity *var1, int var2) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    (void)var1;
    if (!selfE->worldObj->isRemote && !selfE->isDead) {
        self->minecartRockDirection = -self->minecartRockDirection;
        self->minecartTimeSinceHit = 10;
        Entity_setBeenAttacked(selfE);
        self->minecartCurrentDamage += var2 * 10;
        if (self->minecartCurrentDamage > 40) {
            if (selfE->riddenByEntity != NULL) {
                Entity_mountEntity(selfE->riddenByEntity, selfE);
            }
            selfE->vtable->setEntityDead(selfE);
            Entity_dropItemWithOffset(selfE, Item_minecartEmpty->shiftedIndex, 1, 0.0F);
            if (self->minecartType == 1) {
                EntityMinecart *var3 = self;
                for (int var4 = 0; var4 < EntityMinecart_getSizeInventory(var3); ++var4) {
                    ItemStack *var5 = EntityMinecart_getStackInSlot(var3, var4);
                    if (var5 != NULL) {
                        float var6 = JavaRandom_nextFloat(&selfE->rand) * 0.8F + 0.1F;
                        float var7 = JavaRandom_nextFloat(&selfE->rand) * 0.8F + 0.1F;
                        float var8 = JavaRandom_nextFloat(&selfE->rand) * 0.8F + 0.1F;
                        while (var5->stackSize > 0) {
                            int var9 = JavaRandom_nextInt(&selfE->rand, 21) + 10;
                            if (var9 > var5->stackSize) {
                                var9 = var5->stackSize;
                            }
                            var5->stackSize -= var9;
                            EntityItem *var10 = EntityItem_create(
                                selfE->worldObj, selfE->posX + (double)var6,
                                selfE->posY + (double)var7, selfE->posZ + (double)var8,
                                ItemStack_fromItemID(var5->itemID, var9,
                                                     ItemStack_getItemDamage(var5)));
                            float var11 = 0.05F;
                            var10->base.motionX =
                                (double)((float)JavaRandom_nextGaussian(&selfE->rand) * var11);
                            var10->base.motionY =
                                (double)((float)JavaRandom_nextGaussian(&selfE->rand) * var11 +
                                         0.2F);
                            var10->base.motionZ =
                                (double)((float)JavaRandom_nextGaussian(&selfE->rand) * var11);
                            World_entityJoinedWorld(selfE->worldObj, &var10->base);
                        }
                    }
                }
                Entity_dropItemWithOffset(selfE, Block_chest->blockID, 1, 0.0F);
            } else if (self->minecartType == 2) {
                Entity_dropItemWithOffset(selfE, Block_stoneOvenIdle->blockID, 1, 0.0F);
            }
        }
        return 1;
    } else {
        return 1;
    }
}

void EntityMinecart_onUpdate(Entity *selfE) {
    EntityMinecart *self = (EntityMinecart *)selfE;

    if (self->minecartTimeSinceHit > 0) {
        --self->minecartTimeSinceHit;
    }
    if (self->minecartCurrentDamage > 0) {
        --self->minecartCurrentDamage;
    }

    double var7;
    if (selfE->worldObj->isRemote && self->field_9415_k > 0) {
        if (self->field_9415_k > 0) {
            double var46 =
                selfE->posX + (self->field_9414_l - selfE->posX) / (double)self->field_9415_k;
            double var47 =
                selfE->posY + (self->field_9413_m - selfE->posY) / (double)self->field_9415_k;
            double var5 =
                selfE->posZ + (self->field_9412_n - selfE->posZ) / (double)self->field_9415_k;
            for (var7 = self->field_9411_o - (double)selfE->rotationYaw; var7 < -180.0;
                 var7 += 360.0) {
            }
            while (var7 >= 180.0) {
                var7 -= 360.0;
            }
            selfE->rotationYaw =
                (float)((double)selfE->rotationYaw + var7 / (double)self->field_9415_k);
            selfE->rotationPitch = (float)((double)selfE->rotationPitch +
                                           (self->field_9410_p - (double)selfE->rotationPitch) /
                                               (double)self->field_9415_k);
            --self->field_9415_k;
            selfE->vtable->setPosition(selfE, var46, var47, var5);
            Entity_setRotation(selfE, selfE->rotationYaw, selfE->rotationPitch);
        } else {
            selfE->vtable->setPosition(selfE, selfE->posX, selfE->posY, selfE->posZ);
            Entity_setRotation(selfE, selfE->rotationYaw, selfE->rotationPitch);
        }
    } else {
        selfE->prevPosX = selfE->posX;
        selfE->prevPosY = selfE->posY;
        selfE->prevPosZ = selfE->posZ;
        selfE->motionY -= (double)0.04F;
        int var1 = MathHelper_floor_double(selfE->posX);
        int var2 = MathHelper_floor_double(selfE->posY);
        int var3 = MathHelper_floor_double(selfE->posZ);
        if (BlockRail_isRailBlockAt(selfE->worldObj, var1, var2 - 1, var3)) {
            --var2;
        }
        double var4 = 0.4;
        int var6 = 0;
        var7 = 1.0 / 128.0;
        int var9 = World_getBlockId(selfE->worldObj, var1, var2, var3);
        if (BlockRail_isRailBlock(var9)) {
            Vec3D *var10 = EntityMinecart_func_514_g(self, selfE->posX, selfE->posY, selfE->posZ);
            int var11 = World_getBlockMetadata(selfE->worldObj, var1, var2, var3);
            selfE->posY = (double)var2;
            int var12 = 0;
            int var13 = 0;
            if (var9 == Block_railPowered->blockID) {
                var12 = (var11 & 8) != 0;
                var13 = !var12;
            }
            if (Block_blocksList[var9] && BlockRail_getIsPowered(Block_blocksList[var9])) {
                var11 &= 7;
            }
            if (var11 >= 2 && var11 <= 5) {
                selfE->posY = (double)(var2 + 1);
            }
            if (var11 == 2) {
                selfE->motionX -= var7;
            }
            if (var11 == 3) {
                selfE->motionX += var7;
            }
            if (var11 == 4) {
                selfE->motionZ += var7;
            }
            if (var11 == 5) {
                selfE->motionZ -= var7;
            }
            const int (*var14)[3] = field_855_j[var11];
            double var15 = (double)(var14[1][0] - var14[0][0]);
            double var17 = (double)(var14[1][2] - var14[0][2]);
            double var19 = sqrt(var15 * var15 + var17 * var17);
            double var21 = selfE->motionX * var15 + selfE->motionZ * var17;
            if (var21 < 0.0) {
                var15 = -var15;
                var17 = -var17;
            }
            double var23 = sqrt(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
            selfE->motionX = var23 * var15 / var19;
            selfE->motionZ = var23 * var17 / var19;
            double var25;
            if (var13) {
                var25 = sqrt(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
                if (var25 < 0.03) {
                    selfE->motionX *= 0.0;
                    selfE->motionY *= 0.0;
                    selfE->motionZ *= 0.0;
                } else {
                    selfE->motionX *= 0.5;
                    selfE->motionY *= 0.0;
                    selfE->motionZ *= 0.5;
                }
            }
            var25 = 0.0;
            double var27 = (double)var1 + 0.5 + (double)var14[0][0] * 0.5;
            double var29 = (double)var3 + 0.5 + (double)var14[0][2] * 0.5;
            double var31 = (double)var1 + 0.5 + (double)var14[1][0] * 0.5;
            double var33 = (double)var3 + 0.5 + (double)var14[1][2] * 0.5;
            var15 = var31 - var27;
            var17 = var33 - var29;
            double var35, var37, var39;
            if (var15 == 0.0) {
                selfE->posX = (double)var1 + 0.5;
                var25 = selfE->posZ - (double)var3;
            } else if (var17 == 0.0) {
                selfE->posZ = (double)var3 + 0.5;
                var25 = selfE->posX - (double)var1;
            } else {
                var35 = selfE->posX - var27;
                var37 = selfE->posZ - var29;
                var39 = (var35 * var15 + var37 * var17) * 2.0;
                var25 = var39;
            }
            selfE->posX = var27 + var15 * var25;
            selfE->posZ = var29 + var17 * var25;
            selfE->vtable->setPosition(selfE, selfE->posX, selfE->posY + (double)selfE->yOffset,
                                       selfE->posZ);
            var35 = selfE->motionX;
            var37 = selfE->motionZ;
            if (selfE->riddenByEntity != NULL) {
                var35 *= 0.75;
                var37 *= 0.75;
            }
            if (var35 < -var4) {
                var35 = -var4;
            }
            if (var35 > var4) {
                var35 = var4;
            }
            if (var37 < -var4) {
                var37 = -var4;
            }
            if (var37 > var4) {
                var37 = var4;
            }
            selfE->vtable->moveEntity(selfE, var35, 0.0, var37);
            if (var14[0][1] != 0 && MathHelper_floor_double(selfE->posX) - var1 == var14[0][0] &&
                MathHelper_floor_double(selfE->posZ) - var3 == var14[0][2]) {
                selfE->vtable->setPosition(selfE, selfE->posX, selfE->posY + (double)var14[0][1],
                                           selfE->posZ);
            } else if (var14[1][1] != 0 &&
                       MathHelper_floor_double(selfE->posX) - var1 == var14[1][0] &&
                       MathHelper_floor_double(selfE->posZ) - var3 == var14[1][2]) {
                selfE->vtable->setPosition(selfE, selfE->posX, selfE->posY + (double)var14[1][1],
                                           selfE->posZ);
            }
            if (selfE->riddenByEntity != NULL) {
                selfE->motionX *= (double)0.997F;
                selfE->motionY *= 0.0;
                selfE->motionZ *= (double)0.997F;
            } else {
                if (self->minecartType == 2) {
                    var39 = (double)MathHelper_sqrt_double(self->pushX * self->pushX +
                                                           self->pushZ * self->pushZ);
                    if (var39 > 0.01) {
                        var6 = 1;
                        self->pushX /= var39;
                        self->pushZ /= var39;
                        double var41 = 0.04;
                        selfE->motionX *= (double)0.8F;
                        selfE->motionY *= 0.0;
                        selfE->motionZ *= (double)0.8F;
                        selfE->motionX += self->pushX * var41;
                        selfE->motionZ += self->pushZ * var41;
                    } else {
                        selfE->motionX *= (double)0.9F;
                        selfE->motionY *= 0.0;
                        selfE->motionZ *= (double)0.9F;
                    }
                }
                selfE->motionX *= (double)0.96F;
                selfE->motionY *= 0.0;
                selfE->motionZ *= (double)0.96F;
            }
            Vec3D *var52 = EntityMinecart_func_514_g(self, selfE->posX, selfE->posY, selfE->posZ);
            if (var52 != NULL && var10 != NULL) {
                double var40 = (var10->yCoord - var52->yCoord) * 0.05;
                var23 = sqrt(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
                if (var23 > 0.0) {
                    selfE->motionX = selfE->motionX / var23 * (var23 + var40);
                    selfE->motionZ = selfE->motionZ / var23 * (var23 + var40);
                }
                selfE->vtable->setPosition(selfE, selfE->posX, var52->yCoord, selfE->posZ);
            }
            int var53 = MathHelper_floor_double(selfE->posX);
            int var54 = MathHelper_floor_double(selfE->posZ);
            if (var53 != var1 || var54 != var3) {
                var23 = sqrt(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
                selfE->motionX = var23 * (double)(var53 - var1);
                selfE->motionZ = var23 * (double)(var54 - var3);
            }
            double var42;
            if (self->minecartType == 2) {
                var42 = (double)MathHelper_sqrt_double(self->pushX * self->pushX +
                                                       self->pushZ * self->pushZ);
                if (var42 > 0.01 &&
                    selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ > 0.001) {
                    self->pushX /= var42;
                    self->pushZ /= var42;
                    if (self->pushX * selfE->motionX + self->pushZ * selfE->motionZ < 0.0) {
                        self->pushX = 0.0;
                        self->pushZ = 0.0;
                    } else {
                        self->pushX = selfE->motionX;
                        self->pushZ = selfE->motionZ;
                    }
                }
            }
            if (var12) {
                var42 = sqrt(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
                if (var42 > 0.01) {
                    double var44 = 0.06;
                    selfE->motionX += selfE->motionX / var42 * var44;
                    selfE->motionZ += selfE->motionZ / var42 * var44;
                } else if (var11 == 1) {
                    if (World_isBlockNormalCube(selfE->worldObj, var1 - 1, var2, var3)) {
                        selfE->motionX = 0.02;
                    } else if (World_isBlockNormalCube(selfE->worldObj, var1 + 1, var2, var3)) {
                        selfE->motionX = -0.02;
                    }
                } else if (var11 == 0) {
                    if (World_isBlockNormalCube(selfE->worldObj, var1, var2, var3 - 1)) {
                        selfE->motionZ = 0.02;
                    } else if (World_isBlockNormalCube(selfE->worldObj, var1, var2, var3 + 1)) {
                        selfE->motionZ = -0.02;
                    }
                }
            }
        } else {
            if (selfE->motionX < -var4) {
                selfE->motionX = -var4;
            }
            if (selfE->motionX > var4) {
                selfE->motionX = var4;
            }
            if (selfE->motionZ < -var4) {
                selfE->motionZ = -var4;
            }
            if (selfE->motionZ > var4) {
                selfE->motionZ = var4;
            }
            if (selfE->onGround) {
                selfE->motionX *= 0.5;
                selfE->motionY *= 0.5;
                selfE->motionZ *= 0.5;
            }
            selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);
            if (!selfE->onGround) {
                selfE->motionX *= (double)0.95F;
                selfE->motionY *= (double)0.95F;
                selfE->motionZ *= (double)0.95F;
            }
        }

        selfE->rotationPitch = 0.0F;
        double var48 = selfE->prevPosX - selfE->posX;
        double var49 = selfE->prevPosZ - selfE->posZ;
        if (var48 * var48 + var49 * var49 > 0.001) {
            selfE->rotationYaw = (float)(atan2(var49, var48) * 180.0 / M_PI);
            if (self->field_856_i) {
                selfE->rotationYaw += 180.0F;
            }
        }
        double var50;
        for (var50 = (double)(selfE->rotationYaw - selfE->prevRotationYaw); var50 >= 180.0;
             var50 -= 360.0) {
        }
        while (var50 < -180.0) {
            var50 += 360.0;
        }
        if (var50 < -170.0 || var50 >= 170.0) {
            selfE->rotationYaw += 180.0F;
            self->field_856_i = !self->field_856_i;
        }
        Entity_setRotation(selfE, selfE->rotationYaw, selfE->rotationPitch);

        Entity *nearby[256];
        int count = World_getEntitiesWithinAABBExcludingEntity(
            selfE->worldObj, selfE,
            AxisAlignedBB_expand(&selfE->boundingBox, (double)0.2F, 0.0, (double)0.2F), nearby,
            256);
        if (count > 0) {
            for (int var51 = 0; var51 < count; ++var51) {
                Entity *var18 = nearby[var51];
                if (var18 != selfE->riddenByEntity && var18->vtable->canBePushed(var18) &&
                    var18->vtable == &EntityMinecart_vtable) {
                    var18->vtable->applyEntityCollision(var18, selfE);
                }
            }
        }

        if (selfE->riddenByEntity != NULL && selfE->riddenByEntity->isDead) {
            selfE->riddenByEntity = NULL;
        }

        if (var6 && JavaRandom_nextInt(&selfE->rand, 4) == 0) {
            --self->fuel;
            if (self->fuel < 0) {
                self->pushX = self->pushZ = 0.0;
            }
            World_spawnParticle(selfE->worldObj, "largesmoke", selfE->posX, selfE->posY + 0.8,
                                selfE->posZ, 0.0, 0.0, 0.0);
        }
    }
}

Vec3D *EntityMinecart_func_515_a(EntityMinecart *self, double var1, double var3, double var5,
                                 double var7) {
    Entity *selfE = &self->base;
    int var9 = MathHelper_floor_double(var1);
    int var10 = MathHelper_floor_double(var3);
    int var11 = MathHelper_floor_double(var5);
    if (BlockRail_isRailBlockAt(selfE->worldObj, var9, var10 - 1, var11)) {
        --var10;
    }
    int var12 = World_getBlockId(selfE->worldObj, var9, var10, var11);
    if (!BlockRail_isRailBlock(var12)) {
        return NULL;
    } else {
        int var13 = World_getBlockMetadata(selfE->worldObj, var9, var10, var11);
        if (Block_blocksList[var12] && BlockRail_getIsPowered(Block_blocksList[var12])) {
            var13 &= 7;
        }
        var3 = (double)var10;
        if (var13 >= 2 && var13 <= 5) {
            var3 = (double)(var10 + 1);
        }
        const int (*var14)[3] = field_855_j[var13];
        double var15 = (double)(var14[1][0] - var14[0][0]);
        double var17 = (double)(var14[1][2] - var14[0][2]);
        double var19 = sqrt(var15 * var15 + var17 * var17);
        var15 /= var19;
        var17 /= var19;
        var1 += var15 * var7;
        var5 += var17 * var7;
        if (var14[0][1] != 0 && MathHelper_floor_double(var1) - var9 == var14[0][0] &&
            MathHelper_floor_double(var5) - var11 == var14[0][2]) {
            var3 += (double)var14[0][1];
        } else if (var14[1][1] != 0 && MathHelper_floor_double(var1) - var9 == var14[1][0] &&
                   MathHelper_floor_double(var5) - var11 == var14[1][2]) {
            var3 += (double)var14[1][1];
        }
        return EntityMinecart_func_514_g(self, var1, var3, var5);
    }
}

Vec3D *EntityMinecart_func_514_g(EntityMinecart *self, double var1, double var3, double var5) {
    Entity *selfE = &self->base;
    int var7 = MathHelper_floor_double(var1);
    int var8 = MathHelper_floor_double(var3);
    int var9 = MathHelper_floor_double(var5);
    if (BlockRail_isRailBlockAt(selfE->worldObj, var7, var8 - 1, var9)) {
        --var8;
    }
    int var10 = World_getBlockId(selfE->worldObj, var7, var8, var9);
    if (BlockRail_isRailBlock(var10)) {
        int var11 = World_getBlockMetadata(selfE->worldObj, var7, var8, var9);
        var3 = (double)var8;
        if (Block_blocksList[var10] && BlockRail_getIsPowered(Block_blocksList[var10])) {
            var11 &= 7;
        }
        if (var11 >= 2 && var11 <= 5) {
            var3 = (double)(var8 + 1);
        }
        const int (*var12)[3] = field_855_j[var11];
        double var13 = 0.0;
        double var15 = (double)var7 + 0.5 + (double)var12[0][0] * 0.5;
        double var17 = (double)var8 + 0.5 + (double)var12[0][1] * 0.5;
        double var19 = (double)var9 + 0.5 + (double)var12[0][2] * 0.5;
        double var21 = (double)var7 + 0.5 + (double)var12[1][0] * 0.5;
        double var23 = (double)var8 + 0.5 + (double)var12[1][1] * 0.5;
        double var25 = (double)var9 + 0.5 + (double)var12[1][2] * 0.5;
        double var27 = var21 - var15;
        double var29 = (var23 - var17) * 2.0;
        double var31 = var25 - var19;
        if (var27 == 0.0) {
            var1 = (double)var7 + 0.5;
            var13 = var5 - (double)var9;
        } else if (var31 == 0.0) {
            var5 = (double)var9 + 0.5;
            var13 = var1 - (double)var7;
        } else {
            double var33 = var1 - var15;
            double var35 = var5 - var19;
            double var37 = (var33 * var27 + var35 * var31) * 2.0;
            var13 = var37;
        }
        var1 = var15 + var27 * var13;
        var3 = var17 + var29 * var13;
        var5 = var19 + var31 * var13;
        if (var29 < 0.0) {
            ++var3;
        }
        if (var29 > 0.0) {
            var3 += 0.5;
        }
        return Vec3D_createVector(var1, var3, var5);
    } else {
        return NULL;
    }
}

void EntityMinecart_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    NBTTagCompound_setInteger(var1, "Type", self->minecartType);
    if (self->minecartType == 2) {
        NBTTagCompound_setDouble(var1, "PushX", self->pushX);
        NBTTagCompound_setDouble(var1, "PushZ", self->pushZ);
        NBTTagCompound_setShort(var1, "Fuel", (int16_t)self->fuel);
    } else if (self->minecartType == 1) {
        NBTTagList *var2 = NBTTagList_create();
        for (int var3 = 0; var3 < (int)(sizeof(self->cargoItems) / sizeof(self->cargoItems[0]));
             ++var3) {
            if (self->cargoItems[var3] != NULL) {
                NBTTagCompound *var4 = NBTTagCompound_create();
                NBTTagCompound_setByte(var4, "Slot", (int8_t)var3);
                ItemStack_writeToNBT(self->cargoItems[var3], var4);
                NBTTagList_setTag(var2, (NBTBase *)var4);
            }
        }
        NBTTagCompound_setTag(var1, "Items", (NBTBase *)var2);
    }
}

void EntityMinecart_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    self->minecartType = NBTTagCompound_getInteger(var1, "Type");
    if (self->minecartType == 2) {
        self->pushX = NBTTagCompound_getDouble(var1, "PushX");
        self->pushZ = NBTTagCompound_getDouble(var1, "PushZ");
        self->fuel = NBTTagCompound_getShort(var1, "Fuel");
    } else if (self->minecartType == 1) {
        NBTTagList *var2 = NBTTagCompound_getTagList(var1, "Items");
        memset(self->cargoItems, 0, sizeof(self->cargoItems));
        for (int var3 = 0; var3 < NBTTagList_tagCount(var2); ++var3) {
            NBTTagCompound *var4 = (NBTTagCompound *)NBTTagList_tagAt(var2, var3);
            int var5 = NBTTagCompound_getByte(var4, "Slot") & 255;
            if (var5 >= 0 && var5 < (int)(sizeof(self->cargoItems) / sizeof(self->cargoItems[0]))) {
                ItemStack tmp = ItemStack_fromNBT(var4);
                self->cargoItems[var5] =
                    ItemStack_new(tmp.itemID, tmp.stackSize, ItemStack_getItemDamage(&tmp));
            }
        }
    }
}

float EntityMinecart_getShadowSize(Entity *self) {
    (void)self;
    return 0.0F;
}

void EntityMinecart_applyEntityCollision(Entity *selfE, Entity *var1) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    if (!selfE->worldObj->isRemote) {
        if (var1 != selfE->riddenByEntity) {
            if (var1->isLiving && !var1->isPlayer && self->minecartType == 0 &&
                selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ > 0.01 &&
                selfE->riddenByEntity == NULL && var1->ridingEntity == NULL) {
                Entity_mountEntity(var1, selfE);
            }
            double var2 = var1->posX - selfE->posX;
            double var4 = var1->posZ - selfE->posZ;
            double var6 = var2 * var2 + var4 * var4;
            if (var6 >= (double)1.0E-4F) {
                var6 = (double)MathHelper_sqrt_double(var6);
                var2 /= var6;
                var4 /= var6;
                double var8 = 1.0 / var6;
                if (var8 > 1.0) {
                    var8 = 1.0;
                }
                var2 *= var8;
                var4 *= var8;
                var2 *= (double)0.1F;
                var4 *= (double)0.1F;
                var2 *= (double)(1.0F - selfE->entityCollisionReduction);
                var4 *= (double)(1.0F - selfE->entityCollisionReduction);
                var2 *= 0.5;
                var4 *= 0.5;
                if (var1->vtable == &EntityMinecart_vtable) {
                    EntityMinecart *var1mc = (EntityMinecart *)var1;
                    double var10 = var1->posX - selfE->posX;
                    double var12 = var1->posZ - selfE->posZ;
                    double var14 = var10 * var1->motionZ + var12 * var1->prevPosX;
                    var14 *= var14;
                    if (var14 > 5.0) {
                        return;
                    }
                    double var16 = var1->motionX + selfE->motionX;
                    double var18 = var1->motionZ + selfE->motionZ;
                    if (var1mc->minecartType == 2 && self->minecartType != 2) {
                        selfE->motionX *= (double)0.2F;
                        selfE->motionZ *= (double)0.2F;
                        Entity_addVelocity(selfE, var1->motionX - var2, 0.0, var1->motionZ - var4);
                        var1->motionX *= (double)0.7F;
                        var1->motionZ *= (double)0.7F;
                    } else if (var1mc->minecartType != 2 && self->minecartType == 2) {
                        var1->motionX *= (double)0.2F;
                        var1->motionZ *= (double)0.2F;
                        Entity_addVelocity(var1, selfE->motionX + var2, 0.0, selfE->motionZ + var4);
                        selfE->motionX *= (double)0.7F;
                        selfE->motionZ *= (double)0.7F;
                    } else {
                        var16 /= 2.0;
                        var18 /= 2.0;
                        selfE->motionX *= (double)0.2F;
                        selfE->motionZ *= (double)0.2F;
                        Entity_addVelocity(selfE, var16 - var2, 0.0, var18 - var4);
                        var1->motionX *= (double)0.2F;
                        var1->motionZ *= (double)0.2F;
                        Entity_addVelocity(var1, var16 + var2, 0.0, var18 + var4);
                    }
                } else {
                    Entity_addVelocity(selfE, -var2, 0.0, -var4);
                    Entity_addVelocity(var1, var2 / 4.0, 0.0, var4 / 4.0);
                }
            }
        }
    }
}

int EntityMinecart_interact(Entity *selfE, EntityPlayer *var1) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    if (self->minecartType == 0) {
        if (selfE->riddenByEntity != NULL && selfE->riddenByEntity->isPlayer &&
            selfE->riddenByEntity != (Entity *)var1) {
            return 1;
        }
        if (!selfE->worldObj->isRemote) {
            Entity_mountEntity((Entity *)var1, selfE);
        }
    } else if (self->minecartType == 1) {
        if (!selfE->worldObj->isRemote) {
            EntityPlayer_displayGUIChest(var1, (struct IInventory *)self);
        }
    } else if (self->minecartType == 2) {
        ItemStack *var2 = InventoryPlayer_getCurrentItem(&var1->inventory);
        if (var2 != NULL && var2->itemID == Item_coal->shiftedIndex) {
            if (--var2->stackSize == 0) {
                InventoryPlayer_setInventorySlotContents(&var1->inventory,
                                                         var1->inventory.currentItem, NULL);
            }
            self->fuel += 1200;
        }
        self->pushX = selfE->posX - ((Entity *)var1)->posX;
        self->pushZ = selfE->posZ - ((Entity *)var1)->posZ;
    }
    return 1;
}

void EntityMinecart_setPositionAndRotation2(Entity *selfE, double var1, double var3, double var5,
                                            float var7, float var8, int var9) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    self->field_9414_l = var1;
    self->field_9413_m = var3;
    self->field_9412_n = var5;
    self->field_9411_o = (double)var7;
    self->field_9410_p = (double)var8;
    self->field_9415_k = var9 + 2;
    selfE->motionX = self->field_9409_q;
    selfE->motionY = self->field_9408_r;
    selfE->motionZ = self->field_9407_s;
}

void EntityMinecart_setVelocity(Entity *selfE, double var1, double var3, double var5) {
    EntityMinecart *self = (EntityMinecart *)selfE;
    self->field_9409_q = selfE->motionX = var1;
    self->field_9408_r = selfE->motionY = var3;
    self->field_9407_s = selfE->motionZ = var5;
}

int EntityMinecart_getSizeInventory(EntityMinecart *self) {
    (void)self;
    return 27;
}

ItemStack *EntityMinecart_getStackInSlot(EntityMinecart *self, int var1) {
    return self->cargoItems[var1];
}

ItemStack *EntityMinecart_decrStackSize(EntityMinecart *self, int var1, int var2) {
    if (self->cargoItems[var1] != NULL) {
        ItemStack *var3;
        if (self->cargoItems[var1]->stackSize <= var2) {
            var3 = self->cargoItems[var1];
            self->cargoItems[var1] = NULL;
            return var3;
        } else {
            ItemStack split = ItemStack_splitStack(self->cargoItems[var1], var2);
            var3 = ItemStack_new(split.itemID, split.stackSize, ItemStack_getItemDamage(&split));
            if (self->cargoItems[var1]->stackSize == 0) {
                self->cargoItems[var1] = NULL;
            }
            return var3;
        }
    } else {
        return NULL;
    }
}

void EntityMinecart_setInventorySlotContents(EntityMinecart *self, int var1, ItemStack *var2) {
    self->cargoItems[var1] = var2;
    if (var2 != NULL && var2->stackSize > EntityMinecart_getInventoryStackLimit(self)) {
        var2->stackSize = EntityMinecart_getInventoryStackLimit(self);
    }
}

const char *EntityMinecart_getInvName(EntityMinecart *self) {
    (void)self;
    return "Minecart";
}

int EntityMinecart_getInventoryStackLimit(EntityMinecart *self) {
    (void)self;
    return 64;
}

void EntityMinecart_onInventoryChanged(EntityMinecart *self) { (void)self; }

int EntityMinecart_canInteractWith(EntityMinecart *self, EntityPlayer *var1) {
    return self->base.isDead ? 0
                             : Entity_getDistanceSqToEntity(&self->base, (Entity *)var1) <= 64.0;
}
