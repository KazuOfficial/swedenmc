#include "EntityBoat.h"
#include "World.h"
#include "Block.h"
#include "Item.h"
#include "Material.h"
#include "MathHelper.h"
#include "AxisAlignedBB.h"
#include <math.h>

static EntityVtable EntityBoat_vtable;
static int EntityBoat_vtable_init = 0;

static const char *boat_getEntityString(Entity *self) {
    (void)self;
    return "Boat";
}

static void ensure_vtable(void) {
    if (!EntityBoat_vtable_init) {
        Entity_initVtable(&EntityBoat_vtable);
        EntityBoat_vtable.getEntityString = boat_getEntityString;
        EntityBoat_vtable.entityInit = EntityBoat_entityInit;
        EntityBoat_vtable.onUpdate = EntityBoat_onUpdate;
        EntityBoat_vtable.getBoundingBox = EntityBoat_getBoundingBox;
        EntityBoat_vtable.getCollisionBox = EntityBoat_getCollisionBox;
        EntityBoat_vtable.attackEntityFrom = EntityBoat_attackEntityFrom;
        EntityBoat_vtable.interact = EntityBoat_interact;
        EntityBoat_vtable.writeEntityToNBT = EntityBoat_writeEntityToNBT;
        EntityBoat_vtable.readEntityFromNBT = EntityBoat_readEntityFromNBT;
        EntityBoat_vtable.getShadowSize = EntityBoat_getShadowSize;
        EntityBoat_vtable.getMountedYOffset = EntityBoat_getMountedYOffset;
        EntityBoat_vtable.canBeCollidedWith = EntityBoat_canBeCollidedWith;
        EntityBoat_vtable.canBePushed = EntityBoat_canBePushed;
        EntityBoat_vtable.canTriggerWalking = EntityBoat_canTriggerWalking;
        EntityBoat_vtable.performHurtAnimation = EntityBoat_performHurtAnimation;
        EntityBoat_vtable.updateRiderPosition = EntityBoat_updateRiderPosition;
        EntityBoat_vtable_init = 1;
    }
}

void EntityBoat_construct1(EntityBoat *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityBoat_vtable;
    self->boatCurrentDamage = 0;
    self->boatTimeSinceHit = 0;
    self->boatRockDirection = 1;
    self->field_9394_d = 0;
    self->field_9393_e = 0.0;
    self->field_9392_f = 0.0;
    self->field_9391_g = 0.0;
    self->field_9390_h = 0.0;
    self->field_9389_i = 0.0;
    self->field_9388_j = 0.0;
    self->field_9387_k = 0.0;
    self->field_9386_l = 0.0;
    self->base.preventEntitySpawning = 1;
    Entity_setSize(&self->base, 1.5F, 0.6F);
    self->base.yOffset = self->base.height / 2.0F;
    self->base.isBoat = 1;
}

void EntityBoat_construct2(EntityBoat *self, World *var1, double var2, double var4, double var6) {
    EntityBoat_construct1(self, var1);
    self->base.vtable->setPosition(&self->base, var2, var4 + (double)self->base.yOffset, var6);
    self->base.motionX = 0.0;
    self->base.motionY = 0.0;
    self->base.motionZ = 0.0;
    self->base.prevPosX = var2;
    self->base.prevPosY = var4;
    self->base.prevPosZ = var6;
}

int EntityBoat_canTriggerWalking(Entity *selfE) {
    (void)selfE;
    return 0;
}

void EntityBoat_entityInit(Entity *selfE) { (void)selfE; }

AxisAlignedBB *EntityBoat_getCollisionBox(Entity *selfE, Entity *var1) {
    (void)selfE;
    return &var1->boundingBox;
}

AxisAlignedBB *EntityBoat_getBoundingBox(Entity *selfE) { return &selfE->boundingBox; }

int EntityBoat_canBePushed(Entity *selfE) {
    (void)selfE;
    return 1;
}

double EntityBoat_getMountedYOffset(Entity *selfE) {
    return (double)selfE->height * 0.0 - (double)0.3F;
}

int EntityBoat_attackEntityFrom(Entity *selfE, Entity *var1, int var2) {
    EntityBoat *self = (EntityBoat *)selfE;
    (void)var1;
    if (!selfE->worldObj->isRemote && !selfE->isDead) {
        self->boatRockDirection = -self->boatRockDirection;
        self->boatTimeSinceHit = 10;
        self->boatCurrentDamage += var2 * 10;
        Entity_setBeenAttacked(selfE);
        if (self->boatCurrentDamage > 40) {
            if (selfE->riddenByEntity != NULL) {
                Entity_mountEntity(selfE->riddenByEntity, selfE);
            }
            int var3;
            for (var3 = 0; var3 < 3; ++var3) {
                Entity_dropItemWithOffset(selfE, Block_planks->blockID, 1, 0.0F);
            }
            for (var3 = 0; var3 < 2; ++var3) {
                Entity_dropItemWithOffset(selfE, Item_stick->shiftedIndex, 1, 0.0F);
            }
            Entity_setEntityDead(selfE);
        }
        return 1;
    } else {
        return 1;
    }
}

void EntityBoat_performHurtAnimation(Entity *selfE) {
    EntityBoat *self = (EntityBoat *)selfE;
    self->boatRockDirection = -self->boatRockDirection;
    self->boatTimeSinceHit = 10;
    self->boatCurrentDamage += self->boatCurrentDamage * 10;
}

int EntityBoat_canBeCollidedWith(Entity *selfE) { return !selfE->isDead; }

void EntityBoat_setPositionAndRotation2(EntityBoat *self, double var1, double var3, double var5,
                                        float var7, float var8, int var9) {
    self->field_9393_e = var1;
    self->field_9392_f = var3;
    self->field_9391_g = var5;
    self->field_9390_h = (double)var7;
    self->field_9389_i = (double)var8;
    self->field_9394_d = var9 + 4;
    self->base.motionX = self->field_9388_j;
    self->base.motionY = self->field_9387_k;
    self->base.motionZ = self->field_9386_l;
}

void EntityBoat_setVelocity(EntityBoat *self, double var1, double var3, double var5) {
    self->field_9388_j = self->base.motionX = var1;
    self->field_9387_k = self->base.motionY = var3;
    self->field_9386_l = self->base.motionZ = var5;
}

void EntityBoat_onUpdate(Entity *selfE) {
    EntityBoat *self = (EntityBoat *)selfE;

    Entity_defaultVtable.onUpdate(selfE);

    if (self->boatTimeSinceHit > 0) {
        --self->boatTimeSinceHit;
    }
    if (self->boatCurrentDamage > 0) {
        --self->boatCurrentDamage;
    }

    selfE->prevPosX = selfE->posX;
    selfE->prevPosY = selfE->posY;
    selfE->prevPosZ = selfE->posZ;

    int var1 = 5;
    double var2 = 0.0;

    int var4;
    for (var4 = 0; var4 < var1; ++var4) {
        double var5 = selfE->boundingBox.minY +
                      (selfE->boundingBox.maxY - selfE->boundingBox.minY) * (double)(var4 + 0) /
                          (double)var1 -
                      0.125;
        double var7 = selfE->boundingBox.minY +
                      (selfE->boundingBox.maxY - selfE->boundingBox.minY) * (double)(var4 + 1) /
                          (double)var1 -
                      0.125;
        AxisAlignedBB *var9 = AxisAlignedBB_getBoundingBoxFromPool(
            selfE->boundingBox.minX, var5, selfE->boundingBox.minZ, selfE->boundingBox.maxX, var7,
            selfE->boundingBox.maxZ);
        if (World_isAABBInMaterial(selfE->worldObj, var9, Material_water)) {
            var2 += 1.0 / (double)var1;
        }
    }

    double var6, var8, var10, var21;

    if (selfE->worldObj->isRemote) {
        if (self->field_9394_d > 0) {
            var21 = selfE->posX + (self->field_9393_e - selfE->posX) / (double)self->field_9394_d;
            var6 = selfE->posY + (self->field_9392_f - selfE->posY) / (double)self->field_9394_d;
            var8 = selfE->posZ + (self->field_9391_g - selfE->posZ) / (double)self->field_9394_d;

            for (var10 = self->field_9390_h - (double)selfE->rotationYaw; var10 < -180.0;
                 var10 += 360.0) {
            }
            while (var10 >= 180.0) {
                var10 -= 360.0;
            }

            selfE->rotationYaw =
                (float)((double)selfE->rotationYaw + var10 / (double)self->field_9394_d);
            selfE->rotationPitch = (float)((double)selfE->rotationPitch +
                                           (self->field_9389_i - (double)selfE->rotationPitch) /
                                               (double)self->field_9394_d);
            --self->field_9394_d;
            selfE->vtable->setPosition(selfE, var21, var6, var8);
            Entity_setRotation(selfE, selfE->rotationYaw, selfE->rotationPitch);
        } else {
            var21 = selfE->posX + selfE->motionX;
            var6 = selfE->posY + selfE->motionY;
            var8 = selfE->posZ + selfE->motionZ;
            selfE->vtable->setPosition(selfE, var21, var6, var8);
            if (selfE->onGround) {
                selfE->motionX *= 0.5;
                selfE->motionY *= 0.5;
                selfE->motionZ *= 0.5;
            }
            selfE->motionX *= (double)0.99F;
            selfE->motionY *= (double)0.95F;
            selfE->motionZ *= (double)0.99F;
        }
    } else {
        if (var2 < 1.0) {
            var21 = var2 * 2.0 - 1.0;
            selfE->motionY += (double)0.04F * var21;
        } else {
            if (selfE->motionY < 0.0) {
                selfE->motionY /= 2.0;
            }
            selfE->motionY += (double)0.007F;
        }

        if (selfE->riddenByEntity != NULL) {
            selfE->motionX += selfE->riddenByEntity->motionX * 0.2;
            selfE->motionZ += selfE->riddenByEntity->motionZ * 0.2;
        }

        var21 = 0.4;
        if (selfE->motionX < -var21)
            selfE->motionX = -var21;
        if (selfE->motionX > var21)
            selfE->motionX = var21;
        if (selfE->motionZ < -var21)
            selfE->motionZ = -var21;
        if (selfE->motionZ > var21)
            selfE->motionZ = var21;

        if (selfE->onGround) {
            selfE->motionX *= 0.5;
            selfE->motionY *= 0.5;
            selfE->motionZ *= 0.5;
        }

        selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);

        var6 = sqrt(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
        if (var6 > 0.15) {
            var8 = cos((double)selfE->rotationYaw * M_PI / 180.0);
            var10 = sin((double)selfE->rotationYaw * M_PI / 180.0);
            int var12;
            for (var12 = 0; (double)var12 < 1.0 + var6 * 60.0; ++var12) {
                double var13 = (double)(JavaRandom_nextFloat(&selfE->rand) * 2.0F - 1.0F);
                double var15 = (double)(JavaRandom_nextInt(&selfE->rand, 2) * 2 - 1) * 0.7;
                double var17, var19;
                if (JavaRandom_nextBoolean(&selfE->rand)) {
                    var17 = selfE->posX - var8 * var13 * 0.8 + var10 * var15;
                    var19 = selfE->posZ - var10 * var13 * 0.8 - var8 * var15;
                    World_spawnParticle(selfE->worldObj, "splash", var17, selfE->posY - 0.125,
                                        var19, selfE->motionX, selfE->motionY, selfE->motionZ);
                } else {
                    var17 = selfE->posX + var8 + var10 * var13 * 0.7;
                    var19 = selfE->posZ + var10 - var8 * var13 * 0.7;
                    World_spawnParticle(selfE->worldObj, "splash", var17, selfE->posY - 0.125,
                                        var19, selfE->motionX, selfE->motionY, selfE->motionZ);
                }
            }
        }

        if (selfE->isCollidedHorizontally && var6 > 0.15) {
            if (!selfE->worldObj->isRemote) {
                Entity_setEntityDead(selfE);
                int var22;
                for (var22 = 0; var22 < 3; ++var22) {
                    Entity_dropItemWithOffset(selfE, Block_planks->blockID, 1, 0.0F);
                }
                for (var22 = 0; var22 < 2; ++var22) {
                    Entity_dropItemWithOffset(selfE, Item_stick->shiftedIndex, 1, 0.0F);
                }
            }
        } else {
            selfE->motionX *= (double)0.99F;
            selfE->motionY *= (double)0.95F;
            selfE->motionZ *= (double)0.99F;
        }

        selfE->rotationPitch = 0.0F;
        var8 = (double)selfE->rotationYaw;
        var10 = selfE->prevPosX - selfE->posX;
        double var23 = selfE->prevPosZ - selfE->posZ;
        if (var10 * var10 + var23 * var23 > 0.001) {
            var8 = (double)((float)(atan2(var23, var10) * 180.0 / M_PI));
        }

        double var14;
        for (var14 = var8 - (double)selfE->rotationYaw; var14 >= 180.0; var14 -= 360.0) {
        }
        while (var14 < -180.0) {
            var14 += 360.0;
        }
        if (var14 > 20.0)
            var14 = 20.0;
        if (var14 < -20.0)
            var14 = -20.0;

        selfE->rotationYaw = (float)((double)selfE->rotationYaw + var14);
        Entity_setRotation(selfE, selfE->rotationYaw, selfE->rotationPitch);

        Entity *nearby[256];
        int count = World_getEntitiesWithinAABBExcludingEntity(
            selfE->worldObj, selfE,
            AxisAlignedBB_expand(&selfE->boundingBox, (double)0.2F, 0.0, (double)0.2F), nearby,
            256);
        int var24;
        if (count > 0) {
            for (var24 = 0; var24 < count; ++var24) {
                Entity *var18 = nearby[var24];
                if (var18 != selfE->riddenByEntity && var18->vtable->canBePushed(var18) &&
                    var18->vtable == &EntityBoat_vtable) {
                    var18->vtable->applyEntityCollision(var18, selfE);
                }
            }
        }

        for (var24 = 0; var24 < 4; ++var24) {
            int var25 = MathHelper_floor_double(selfE->posX + ((double)(var24 % 2) - 0.5) * 0.8);
            int var26 = MathHelper_floor_double(selfE->posY);
            int var20 = MathHelper_floor_double(selfE->posZ + ((double)(var24 / 2) - 0.5) * 0.8);
            if (World_getBlockId(selfE->worldObj, var25, var26, var20) == Block_snow->blockID) {
                World_setBlockWithNotify(selfE->worldObj, var25, var26, var20, 0);
            }
        }

        if (selfE->riddenByEntity != NULL && selfE->riddenByEntity->isDead) {
            selfE->riddenByEntity = NULL;
        }
    }
}

void EntityBoat_updateRiderPosition(Entity *selfE) {
    if (selfE->riddenByEntity != NULL) {
        double var1 = cos((double)selfE->rotationYaw * M_PI / 180.0) * 0.4;
        double var3 = sin((double)selfE->rotationYaw * M_PI / 180.0) * 0.4;
        selfE->riddenByEntity->vtable->setPosition(
            selfE->riddenByEntity, selfE->posX + var1,
            selfE->posY + selfE->vtable->getMountedYOffset(selfE) +
                selfE->riddenByEntity->vtable->getYOffset(selfE->riddenByEntity),
            selfE->posZ + var3);
    }
}

void EntityBoat_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    (void)selfE;
    (void)var1;
}

void EntityBoat_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    (void)selfE;
    (void)var1;
}

float EntityBoat_getShadowSize(Entity *selfE) {
    (void)selfE;
    return 0.0F;
}

int EntityBoat_interact(Entity *selfE, EntityPlayer *var1) {
    if (selfE->riddenByEntity != NULL && selfE->riddenByEntity->isPlayer &&
        selfE->riddenByEntity != (Entity *)var1) {
        return 1;
    } else {
        if (!selfE->worldObj->isRemote) {
            Entity_mountEntity((Entity *)var1, selfE);
        }
        return 1;
    }
}
