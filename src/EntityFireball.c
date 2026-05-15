#include "EntityFireball.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "World.h"
#include "MathHelper.h"
#include "Vec3D.h"
#include "AxisAlignedBB.h"
#include "MovingObjectPosition.h"
#include "NBTTagCompound.h"
#include <stdlib.h>
#include <math.h>

static EntityVtable EntityFireball_vtable;
static int EntityFireball_vtable_init = 0;
static float EntityFireball_getCollisionBorderSizeVt(Entity *self);

static void ensure_vtable(void) {
    if (!EntityFireball_vtable_init) {
        Entity_initVtable(&EntityFireball_vtable);
        EntityFireball_vtable.entityInit = EntityFireball_entityInit;
        EntityFireball_vtable.isInRangeToRenderDist = EntityFireball_isInRangeToRenderDist;
        EntityFireball_vtable.onUpdate = EntityFireball_onUpdate;
        EntityFireball_vtable.writeEntityToNBT = EntityFireball_writeEntityToNBT;
        EntityFireball_vtable.readEntityFromNBT = EntityFireball_readEntityFromNBT;
        EntityFireball_vtable.canBeCollidedWith = EntityFireball_canBeCollidedWith;
        EntityFireball_vtable.getCollisionBorderSize = EntityFireball_getCollisionBorderSizeVt;
        EntityFireball_vtable.attackEntityFrom = EntityFireball_attackEntityFrom;
        EntityFireball_vtable.getShadowSize = EntityFireball_getShadowSize;
        EntityFireball_vtable_init = 1;
    }
}

void EntityFireball_construct1(EntityFireball *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityFireball_vtable;

    self->field_9402_e = -1;
    self->field_9401_f = -1;
    self->field_9400_g = -1;
    self->field_9399_h = 0;
    self->field_9398_i = 0;
    self->field_9406_a = 0;
    self->field_9397_j = NULL;
    self->field_9396_k = 0;
    self->field_9395_l = 0;
    self->field_9405_b = 0.0;
    self->field_9404_c = 0.0;
    self->field_9403_d = 0.0;
    Entity_setSize(&self->base, 1.0F, 1.0F);
    self->base.isFireball = 1;
}

void EntityFireball_construct2(EntityFireball *self, World *var1, double var2, double var4,
                               double var6, double var8, double var10, double var12) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityFireball_vtable;

    self->field_9402_e = -1;
    self->field_9401_f = -1;
    self->field_9400_g = -1;
    self->field_9399_h = 0;
    self->field_9398_i = 0;
    self->field_9406_a = 0;
    self->field_9397_j = NULL;
    self->field_9396_k = 0;
    self->field_9395_l = 0;

    Entity_setSize(&self->base, 1.0F, 1.0F);
    self->base.vtable->setLocationAndAngles(&self->base, var2, var4, var6, self->base.rotationYaw,
                                            self->base.rotationPitch);
    self->base.vtable->setPosition(&self->base, var2, var4, var6);
    double var14 = (double)MathHelper_sqrt_double(var8 * var8 + var10 * var10 + var12 * var12);
    self->field_9405_b = var8 / var14 * 0.1;
    self->field_9404_c = var10 / var14 * 0.1;
    self->field_9403_d = var12 / var14 * 0.1;
    self->base.isFireball = 1;
}

void EntityFireball_construct3(EntityFireball *self, World *var1, EntityLiving *var2, double var3,
                               double var5, double var7) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityFireball_vtable;

    self->field_9402_e = -1;
    self->field_9401_f = -1;
    self->field_9400_g = -1;
    self->field_9399_h = 0;
    self->field_9398_i = 0;
    self->field_9406_a = 0;
    self->field_9396_k = 0;
    self->field_9395_l = 0;

    self->field_9397_j = var2;
    Entity_setSize(&self->base, 1.0F, 1.0F);
    self->base.vtable->setLocationAndAngles(&self->base, var2->base.posX, var2->base.posY,
                                            var2->base.posZ, var2->base.rotationYaw,
                                            var2->base.rotationPitch);
    self->base.vtable->setPosition(&self->base, self->base.posX, self->base.posY, self->base.posZ);
    self->base.yOffset = 0.0F;
    self->base.motionX = self->base.motionY = self->base.motionZ = 0.0;
    var3 += JavaRandom_nextGaussian(&self->base.rand) * 0.4;
    var5 += JavaRandom_nextGaussian(&self->base.rand) * 0.4;
    var7 += JavaRandom_nextGaussian(&self->base.rand) * 0.4;
    double var9 = (double)MathHelper_sqrt_double(var3 * var3 + var5 * var5 + var7 * var7);
    self->field_9405_b = var3 / var9 * 0.1;
    self->field_9404_c = var5 / var9 * 0.1;
    self->field_9403_d = var7 / var9 * 0.1;
    self->base.isFireball = 1;
}

void EntityFireball_entityInit(Entity *self) { (void)self; }

int EntityFireball_isInRangeToRenderDist(Entity *selfE, double var1) {
    double var3 = AxisAlignedBB_getAverageEdgeLength(&selfE->boundingBox) * 4.0;
    var3 *= 64.0;
    return var1 < var3 * var3;
}

void EntityFireball_onUpdate(Entity *selfE) {
    EntityFireball *self = (EntityFireball *)selfE;

    Entity_defaultVtable.onUpdate(selfE);
    selfE->fire = 10;
    if (self->field_9406_a > 0) {
        --self->field_9406_a;
    }

    if (self->field_9398_i) {
        int var1 = World_getBlockId(selfE->worldObj, self->field_9402_e, self->field_9401_f,
                                    self->field_9400_g);
        if (var1 == self->field_9399_h) {
            ++self->field_9396_k;
            if (self->field_9396_k == 1200) {
                Entity_setEntityDead(selfE);
            }
            return;
        }

        self->field_9398_i = 0;
        selfE->motionX *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        selfE->motionY *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        selfE->motionZ *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        self->field_9396_k = 0;
        self->field_9395_l = 0;
    } else {
        ++self->field_9395_l;
    }

    Vec3D *var15 = Vec3D_createVector(selfE->posX, selfE->posY, selfE->posZ);
    Vec3D *var2 = Vec3D_createVector(selfE->posX + selfE->motionX, selfE->posY + selfE->motionY,
                                     selfE->posZ + selfE->motionZ);
    MovingObjectPosition *var3 = World_rayTraceBlocks(selfE->worldObj, var15, var2);
    var15 = Vec3D_createVector(selfE->posX, selfE->posY, selfE->posZ);
    var2 = Vec3D_createVector(selfE->posX + selfE->motionX, selfE->posY + selfE->motionY,
                              selfE->posZ + selfE->motionZ);
    if (var3 != NULL) {
        var2 = Vec3D_createVector(var3->hitVec->xCoord, var3->hitVec->yCoord, var3->hitVec->zCoord);
    }

    Entity *var4 = NULL;
    AxisAlignedBB *searchBB = AxisAlignedBB_expand(
        AxisAlignedBB_addCoord(&selfE->boundingBox, selfE->motionX, selfE->motionY, selfE->motionZ),
        1.0, 1.0, 1.0);
    Entity *nearby[256];
    int count =
        World_getEntitiesWithinAABBExcludingEntity(selfE->worldObj, selfE, searchBB, nearby, 256);
    double var6 = 0.0;

    for (int var8 = 0; var8 < count; ++var8) {
        Entity *var9 = nearby[var8];
        if (var9->vtable->canBeCollidedWith(var9) &&
            (var9 != (Entity *)self->field_9397_j || self->field_9395_l >= 25)) {
            float var10 = 0.3F;
            AxisAlignedBB *var11 = AxisAlignedBB_expand(&var9->boundingBox, (double)var10,
                                                        (double)var10, (double)var10);
            MovingObjectPosition *var12 = AxisAlignedBB_func_1169_a(var11, var15, var2);
            if (var12 != NULL) {
                double var13 = Vec3D_distanceTo(var15, var12->hitVec);
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
        if (!selfE->worldObj->isRemote) {
            if (var3->entityHit != NULL) {
                var3->entityHit->vtable->attackEntityFrom(var3->entityHit,
                                                          (Entity *)self->field_9397_j, 0);
            }

            World_newExplosion(selfE->worldObj, NULL, selfE->posX, selfE->posY, selfE->posZ, 1.0F,
                               1);
        }

        Entity_setEntityDead(selfE);
    }

    selfE->posX += selfE->motionX;
    selfE->posY += selfE->motionY;
    selfE->posZ += selfE->motionZ;
    float var16 =
        MathHelper_sqrt_double(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
    selfE->rotationYaw =
        (float)(atan2(selfE->motionX, selfE->motionZ) * 180.0 / (double)((float)M_PI));

    for (selfE->rotationPitch =
             (float)(atan2(selfE->motionY, (double)var16) * 180.0 / (double)((float)M_PI));
         selfE->rotationPitch - selfE->prevRotationPitch < -180.0F;
         selfE->prevRotationPitch -= 360.0F) {
    }

    while (selfE->rotationPitch - selfE->prevRotationPitch >= 180.0F) {
        selfE->prevRotationPitch += 360.0F;
    }

    while (selfE->rotationYaw - selfE->prevRotationYaw < -180.0F) {
        selfE->prevRotationYaw -= 360.0F;
    }

    while (selfE->rotationYaw - selfE->prevRotationYaw >= 180.0F) {
        selfE->prevRotationYaw += 360.0F;
    }

    selfE->rotationPitch =
        selfE->prevRotationPitch + (selfE->rotationPitch - selfE->prevRotationPitch) * 0.2F;
    selfE->rotationYaw =
        selfE->prevRotationYaw + (selfE->rotationYaw - selfE->prevRotationYaw) * 0.2F;
    float var17 = 0.95F;
    if (Entity_isInWater(selfE)) {
        for (int var18 = 0; var18 < 4; ++var18) {
            float var19 = 0.25F;
            World_spawnParticle(selfE->worldObj, "bubble",
                                selfE->posX - selfE->motionX * (double)var19,
                                selfE->posY - selfE->motionY * (double)var19,
                                selfE->posZ - selfE->motionZ * (double)var19, selfE->motionX,
                                selfE->motionY, selfE->motionZ);
        }
        var17 = 0.8F;
    }

    selfE->motionX += self->field_9405_b;
    selfE->motionY += self->field_9404_c;
    selfE->motionZ += self->field_9403_d;
    selfE->motionX *= (double)var17;
    selfE->motionY *= (double)var17;
    selfE->motionZ *= (double)var17;
    World_spawnParticle(selfE->worldObj, "smoke", selfE->posX, selfE->posY + 0.5, selfE->posZ, 0.0,
                        0.0, 0.0);
    selfE->vtable->setPosition(selfE, selfE->posX, selfE->posY, selfE->posZ);
}

void EntityFireball_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityFireball *self = (EntityFireball *)selfE;
    NBTTagCompound_setShort(var1, "xTile", (int16_t)self->field_9402_e);
    NBTTagCompound_setShort(var1, "yTile", (int16_t)self->field_9401_f);
    NBTTagCompound_setShort(var1, "zTile", (int16_t)self->field_9400_g);
    NBTTagCompound_setByte(var1, "inTile", (int8_t)self->field_9399_h);
    NBTTagCompound_setByte(var1, "shake", (int8_t)self->field_9406_a);
    NBTTagCompound_setByte(var1, "inGround", (int8_t)(self->field_9398_i ? 1 : 0));
}

void EntityFireball_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityFireball *self = (EntityFireball *)selfE;
    self->field_9402_e = NBTTagCompound_getShort(var1, "xTile");
    self->field_9401_f = NBTTagCompound_getShort(var1, "yTile");
    self->field_9400_g = NBTTagCompound_getShort(var1, "zTile");
    self->field_9399_h = NBTTagCompound_getByte(var1, "inTile") & 255;
    self->field_9406_a = NBTTagCompound_getByte(var1, "shake") & 255;
    self->field_9398_i = NBTTagCompound_getByte(var1, "inGround") == 1;
}

int EntityFireball_canBeCollidedWith(Entity *self) {
    (void)self;
    return 1;
}

float EntityFireball_getCollisionBorderSize(EntityFireball *self) {
    (void)self;
    return 1.0F;
}
static float EntityFireball_getCollisionBorderSizeVt(Entity *self) {
    (void)self;
    return 1.0F;
}

int EntityFireball_attackEntityFrom(Entity *selfE, Entity *var1, int var2) {
    (void)var2;
    EntityFireball *self = (EntityFireball *)selfE;
    Entity_setBeenAttacked(selfE);
    if (var1 != NULL) {
        Vec3D *var3 = var1->vtable->getLookVec(var1);
        if (var3 != NULL) {
            selfE->motionX = var3->xCoord;
            selfE->motionY = var3->yCoord;
            selfE->motionZ = var3->zCoord;
            self->field_9405_b = selfE->motionX * 0.1;
            self->field_9404_c = selfE->motionY * 0.1;
            self->field_9403_d = selfE->motionZ * 0.1;
        }
        return 1;
    } else {
        return 0;
    }
}

float EntityFireball_getShadowSize(Entity *self) {
    (void)self;
    return 0.0F;
}

EntityFireball *EntityFireball_create3(World *var1, EntityLiving *var2, double var3, double var5,
                                       double var7) {
    EntityFireball *self = (EntityFireball *)calloc(1, sizeof(EntityFireball));
    EntityFireball_construct3(self, var1, var2, var3, var5, var7);
    return self;
}
