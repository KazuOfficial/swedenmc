#include "EntitySnowball.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "Item.h"
#include "MathHelper.h"
#include "JavaRandom.h"
#include "Vec3D.h"
#include "AxisAlignedBB.h"
#include "MovingObjectPosition.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <math.h>

static EntityVtable EntitySnowball_vtable;
static int EntitySnowball_vtable_init = 0;

static const char *snowball_getEntityString(Entity *self) {
    (void)self;
    return "Snowball";
}

static void ensure_vtable(void) {
    if (!EntitySnowball_vtable_init) {
        Entity_initVtable(&EntitySnowball_vtable);
        EntitySnowball_vtable.getEntityString = snowball_getEntityString;
        EntitySnowball_vtable.entityInit = EntitySnowball_entityInit;
        EntitySnowball_vtable.isInRangeToRenderDist = EntitySnowball_isInRangeToRenderDist;
        EntitySnowball_vtable.setVelocity = EntitySnowball_setVelocity;
        EntitySnowball_vtable.onUpdate = EntitySnowball_onUpdate;
        EntitySnowball_vtable.writeEntityToNBT = EntitySnowball_writeEntityToNBT;
        EntitySnowball_vtable.readEntityFromNBT = EntitySnowball_readEntityFromNBT;
        EntitySnowball_vtable.onCollideWithPlayer = EntitySnowball_onCollideWithPlayer;
        EntitySnowball_vtable.getShadowSize = EntitySnowball_getShadowSize;
        EntitySnowball_vtable_init = 1;
    }
}

void EntitySnowball_construct1(EntitySnowball *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntitySnowball_vtable;

    self->xTileSnowball = -1;
    self->yTileSnowball = -1;
    self->zTileSnowball = -1;
    self->inTileSnowball = 0;
    self->inGroundSnowball = 0;
    self->shakeSnowball = 0;
    self->thrower = NULL;
    self->ticksInGroundSnowball = 0;
    self->ticksInAirSnowball = 0;
    Entity_setSize(&self->base, 0.25F, 0.25F);
    self->base.isSnowball = 1;
}

void EntitySnowball_construct2(EntitySnowball *self, World *var1, EntityLiving *var2) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntitySnowball_vtable;

    self->xTileSnowball = -1;
    self->yTileSnowball = -1;
    self->zTileSnowball = -1;
    self->inTileSnowball = 0;
    self->inGroundSnowball = 0;
    self->shakeSnowball = 0;
    self->ticksInGroundSnowball = 0;
    self->ticksInAirSnowball = 0;

    self->thrower = var2;
    Entity_setSize(&self->base, 0.25F, 0.25F);
    self->base.vtable->setLocationAndAngles(
        &self->base, var2->base.posX,
        var2->base.posY + (double)var2->base.vtable->getEyeHeight(&var2->base), var2->base.posZ,
        var2->base.rotationYaw, var2->base.rotationPitch);
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
    EntitySnowball_setSnowballHeading(self, self->base.motionX, self->base.motionY,
                                      self->base.motionZ, 1.5F, 1.0F);
    self->base.isSnowball = 1;
}

void EntitySnowball_construct3(EntitySnowball *self, World *var1, double var2, double var4,
                               double var6) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntitySnowball_vtable;

    self->xTileSnowball = -1;
    self->yTileSnowball = -1;
    self->zTileSnowball = -1;
    self->inTileSnowball = 0;
    self->inGroundSnowball = 0;
    self->shakeSnowball = 0;
    self->thrower = NULL;
    self->ticksInAirSnowball = 0;
    self->ticksInGroundSnowball = 0;
    Entity_setSize(&self->base, 0.25F, 0.25F);
    self->base.vtable->setPosition(&self->base, var2, var4, var6);
    self->base.yOffset = 0.0F;
    self->base.isSnowball = 1;
}

void EntitySnowball_entityInit(Entity *self) { (void)self; }

int EntitySnowball_isInRangeToRenderDist(Entity *selfE, double var1) {
    double var3 = AxisAlignedBB_getAverageEdgeLength(&selfE->boundingBox) * 4.0;
    var3 *= 64.0;
    return var1 < var3 * var3;
}

void EntitySnowball_setSnowballHeading(EntitySnowball *self, double var1, double var3, double var5,
                                       float var7, float var8) {
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
    self->ticksInGroundSnowball = 0;
}

void EntitySnowball_setVelocity(Entity *selfE, double var1, double var3, double var5) {
    EntitySnowball *self = (EntitySnowball *)selfE;
    selfE->motionX = var1;
    selfE->motionY = var3;
    selfE->motionZ = var5;
    if (selfE->prevRotationPitch == 0.0F && selfE->prevRotationYaw == 0.0F) {
        float var7 = MathHelper_sqrt_double(var1 * var1 + var5 * var5);
        selfE->prevRotationYaw = selfE->rotationYaw =
            (float)(atan2(var1, var5) * 180.0 / (double)((float)M_PI));
        selfE->prevRotationPitch = selfE->rotationPitch =
            (float)(atan2(var3, (double)var7) * 180.0 / (double)((float)M_PI));
    }
    (void)self;
}

void EntitySnowball_onUpdate(Entity *selfE) {
    EntitySnowball *self = (EntitySnowball *)selfE;

    selfE->lastTickPosX = selfE->posX;
    selfE->lastTickPosY = selfE->posY;
    selfE->lastTickPosZ = selfE->posZ;
    Entity_defaultVtable.onUpdate(selfE);

    if (self->shakeSnowball > 0) {
        --self->shakeSnowball;
    }

    if (self->inGroundSnowball) {
        int var1 = World_getBlockId(selfE->worldObj, self->xTileSnowball, self->yTileSnowball,
                                    self->zTileSnowball);
        if (var1 == self->inTileSnowball) {
            ++self->ticksInGroundSnowball;
            if (self->ticksInGroundSnowball == 1200) {
                Entity_setEntityDead(selfE);
            }
            return;
        }

        self->inGroundSnowball = 0;
        selfE->motionX *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        selfE->motionY *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        selfE->motionZ *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        self->ticksInGroundSnowball = 0;
        self->ticksInAirSnowball = 0;
    } else {
        ++self->ticksInAirSnowball;
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

    if (!selfE->worldObj->isRemote) {
        Entity *var4 = NULL;
        AxisAlignedBB *searchBB =
            AxisAlignedBB_expand(AxisAlignedBB_addCoord(&selfE->boundingBox, selfE->motionX,
                                                        selfE->motionY, selfE->motionZ),
                                 1.0, 1.0, 1.0);
        Entity *nearby[256];
        int count = World_getEntitiesWithinAABBExcludingEntity(selfE->worldObj, selfE, searchBB,
                                                               nearby, 256);
        double var6 = 0.0;

        for (int var8 = 0; var8 < count; ++var8) {
            Entity *var9 = nearby[var8];
            if (var9->vtable->canBeCollidedWith(var9) &&
                (var9 != (Entity *)self->thrower || self->ticksInAirSnowball >= 5)) {
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
    }

    if (var3 != NULL) {
        if (var3->entityHit != NULL) {
            var3->entityHit->vtable->attackEntityFrom(var3->entityHit, &self->thrower->base, 0);
        }

        for (int var16 = 0; var16 < 8; ++var16) {
            World_spawnParticle(selfE->worldObj, "snowballpoof", selfE->posX, selfE->posY,
                                selfE->posZ, 0.0, 0.0, 0.0);
        }

        Entity_setEntityDead(selfE);
    }

    selfE->posX += selfE->motionX;
    selfE->posY += selfE->motionY;
    selfE->posZ += selfE->motionZ;
    float var17 =
        MathHelper_sqrt_double(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
    selfE->rotationYaw =
        (float)(atan2(selfE->motionX, selfE->motionZ) * 180.0 / (double)((float)M_PI));

    for (selfE->rotationPitch =
             (float)(atan2(selfE->motionY, (double)var17) * 180.0 / (double)((float)M_PI));
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
    float var18 = 0.99F;
    float var19 = 0.03F;
    if (Entity_isInWater(selfE)) {
        for (int var7 = 0; var7 < 4; ++var7) {
            float var20 = 0.25F;
            World_spawnParticle(selfE->worldObj, "bubble",
                                selfE->posX - selfE->motionX * (double)var20,
                                selfE->posY - selfE->motionY * (double)var20,
                                selfE->posZ - selfE->motionZ * (double)var20, selfE->motionX,
                                selfE->motionY, selfE->motionZ);
        }
        var18 = 0.8F;
    }

    selfE->motionX *= (double)var18;
    selfE->motionY *= (double)var18;
    selfE->motionZ *= (double)var18;
    selfE->motionY -= (double)var19;
    selfE->vtable->setPosition(selfE, selfE->posX, selfE->posY, selfE->posZ);
}

void EntitySnowball_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntitySnowball *self = (EntitySnowball *)selfE;
    NBTTagCompound_setShort(var1, "xTile", (int16_t)self->xTileSnowball);
    NBTTagCompound_setShort(var1, "yTile", (int16_t)self->yTileSnowball);
    NBTTagCompound_setShort(var1, "zTile", (int16_t)self->zTileSnowball);
    NBTTagCompound_setByte(var1, "inTile", (int8_t)self->inTileSnowball);
    NBTTagCompound_setByte(var1, "shake", (int8_t)self->shakeSnowball);
    NBTTagCompound_setByte(var1, "inGround", (int8_t)(self->inGroundSnowball ? 1 : 0));
}

void EntitySnowball_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntitySnowball *self = (EntitySnowball *)selfE;
    self->xTileSnowball = NBTTagCompound_getShort(var1, "xTile");
    self->yTileSnowball = NBTTagCompound_getShort(var1, "yTile");
    self->zTileSnowball = NBTTagCompound_getShort(var1, "zTile");
    self->inTileSnowball = NBTTagCompound_getByte(var1, "inTile") & 255;
    self->shakeSnowball = NBTTagCompound_getByte(var1, "shake") & 255;
    self->inGroundSnowball = NBTTagCompound_getByte(var1, "inGround") == 1;
}

void EntitySnowball_onCollideWithPlayer(Entity *selfE, EntityPlayer *var1) {
    EntitySnowball *self = (EntitySnowball *)selfE;
    if (self->inGroundSnowball && (Entity *)self->thrower == (Entity *)var1 &&
        self->shakeSnowball <= 0) {
        ItemStack tmp = ItemStack_fromItemID(Item_arrow->shiftedIndex, 1, 0);
        if (InventoryPlayer_addItemStackToInventory(&var1->inventory, &tmp)) {
            World_playSoundAtEntity(
                selfE->worldObj, selfE, "random.pop", 0.2F,
                ((JavaRandom_nextFloat(&selfE->rand) - JavaRandom_nextFloat(&selfE->rand)) * 0.7F +
                 1.0F) *
                    2.0F);
            ((EntityPlayerVtable *)var1->base.base.vtable)->onItemPickup(var1, selfE, 1);
            Entity_setEntityDead(selfE);
        }
    }
}

float EntitySnowball_getShadowSize(Entity *self) {
    (void)self;
    return 0.0F;
}
