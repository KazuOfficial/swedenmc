#include "Entity.h"
#include "EntityCreature.h"
#include "EntityMinecart.h"
#include "EntityPlayerSP.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "ContainerPlayer.h"
#include "InventoryCrafting.h"
#include "InventoryCraftResult.h"
#include "PathEntity.h"
#include "EntityList.h"
#include "EntityItem.h"
#include "MathHelper.h"
#include "Material.h"
#include "Block.h"
#include "World.h"
#include "NBTTagList.h"
#include "NBTTagDouble.h"
#include "NBTTagFloat.h"
#include "BlockFluid.h"
#include "StepSound.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

int Entity_nextEntityID = 0;

static Vec3D *entityLiving_getLook(Entity *self, float var1);

static void default_entityInit(Entity *self) { (void)self; }

static void default_onUpdate(Entity *self) { self->vtable->onEntityUpdate(self); }

static void default_onEntityUpdate(Entity *self) {
    if (self->ridingEntity && self->ridingEntity->isDead)
        self->ridingEntity = NULL;

    ++self->ticksExisted;
    self->prevDistanceWalkedModified = self->distanceWalkedModified;
    self->prevPosX = self->posX;
    self->prevPosY = self->posY;
    self->prevPosZ = self->posZ;
    self->prevRotationPitch = self->rotationPitch;
    self->prevRotationYaw = self->rotationYaw;

    if (self->vtable->handleWaterMovement(self)) {
        if (!self->inWater && !self->isFirstUpdate) {
            float var1 = (float)MathHelper_sqrt_double(self->motionX * self->motionX * 0.2 +
                                                       self->motionY * self->motionY +
                                                       self->motionZ * self->motionZ * 0.2) *
                         0.2f;
            if (var1 > 1.0f)
                var1 = 1.0f;
            World_playSoundAtEntity(
                self->worldObj, self, "random.splash", var1,
                1.0f +
                    (JavaRandom_nextFloat(&self->rand) - JavaRandom_nextFloat(&self->rand)) * 0.4f);
            float var2 = (float)MathHelper_floor_double(self->boundingBox.minY);
            int var3;
            float var4, var5;
            for (var3 = 0; (float)var3 < 1.0f + self->width * 20.0f; ++var3) {
                var4 = (JavaRandom_nextFloat(&self->rand) * 2.0f - 1.0f) * self->width;
                var5 = (JavaRandom_nextFloat(&self->rand) * 2.0f - 1.0f) * self->width;
                World_spawnParticle(self->worldObj, "bubble", self->posX + (double)var4,
                                    (double)(var2 + 1.0f), self->posZ + (double)var5, self->motionX,
                                    self->motionY -
                                        (double)(JavaRandom_nextFloat(&self->rand) * 0.2f),
                                    self->motionZ);
            }
            for (var3 = 0; (float)var3 < 1.0f + self->width * 20.0f; ++var3) {
                var4 = (JavaRandom_nextFloat(&self->rand) * 2.0f - 1.0f) * self->width;
                var5 = (JavaRandom_nextFloat(&self->rand) * 2.0f - 1.0f) * self->width;
                World_spawnParticle(self->worldObj, "splash", self->posX + (double)var4,
                                    (double)(var2 + 1.0f), self->posZ + (double)var5, self->motionX,
                                    self->motionY, self->motionZ);
            }
        }
        self->fallDistance = 0.0f;
        self->inWater = 1;
        self->fire = 0;
    } else {
        self->inWater = 0;
    }

    if (self->worldObj->multiplayerWorld) {
        self->fire = 0;
    } else if (self->fire > 0) {
        if (self->isImmuneToFire) {
            self->fire -= 4;
            if (self->fire < 0)
                self->fire = 0;
        } else {
            if (self->fire % 20 == 0) {
                self->vtable->attackEntityFrom(self, NULL, 1);
            }
            --self->fire;
        }
    }

    if (Entity_handleLavaMovement(self)) {
        Entity_setOnFireFromLava(self);
    }

    if (self->posY < -64.0) {
        self->vtable->kill(self);
    }

    if (!self->worldObj->multiplayerWorld) {
        Entity_setEntityFlag(self, 0, self->fire > 0);
        Entity_setEntityFlag(self, 2, self->ridingEntity != NULL);
    }

    self->isFirstUpdate = 0;
}

static void default_moveEntity(Entity *self, double var1, double var3, double var5) {

    if (self->noClip) {
        AxisAlignedBB_offset(&self->boundingBox, var1, var3, var5);
        self->posX = (self->boundingBox.minX + self->boundingBox.maxX) / 2.0;
        self->posY = self->boundingBox.minY + (double)self->yOffset - (double)self->ySize;
        self->posZ = (self->boundingBox.minZ + self->boundingBox.maxZ) / 2.0;
        return;
    }

    double var7 = self->posX;
    double var9 = self->posZ;

    self->ySize *= 0.4f;

    if (self->isInWeb) {
        self->isInWeb = 0;
        var1 *= 0.25;
        var3 *= 0.05000000074505806;
        var5 *= 0.25;
        self->motionX = 0.0;
        self->motionY = 0.0;
        self->motionZ = 0.0;
    }

    double var11 = var1;
    double var13 = var3;
    double var15 = var5;

    AxisAlignedBB var17 = self->boundingBox;

    int var18 = self->onGround && self->vtable->isSneaking(self);
    if (var18) {
        double var19 = 0.05;
        for (; var1 != 0.0; var11 = var1) {
            AxisAlignedBB *off =
                AxisAlignedBB_getOffsetBoundingBox(&self->boundingBox, var1, -1.0, 0.0);
            AxisAlignedBB tmp;
            if (World_getCollidingBoundingBoxes(self->worldObj, self, off, &tmp, 1) != 0)
                break;
            if (var1 < var19 && var1 >= -var19) {
                var1 = 0.0;
            } else if (var1 > 0.0) {
                var1 -= var19;
            } else {
                var1 += var19;
            }
        }
        for (; var5 != 0.0; var15 = var5) {
            AxisAlignedBB *off =
                AxisAlignedBB_getOffsetBoundingBox(&self->boundingBox, 0.0, -1.0, var5);
            AxisAlignedBB tmp;
            if (World_getCollidingBoundingBoxes(self->worldObj, self, off, &tmp, 1) != 0)
                break;
            if (var5 < var19 && var5 >= -var19) {
                var5 = 0.0;
            } else if (var5 > 0.0) {
                var5 -= var19;
            } else {
                var5 += var19;
            }
        }
    }

#define MAX_COL_BOXES 64
    AxisAlignedBB var35[MAX_COL_BOXES];
    int var35_n;
    {
        AxisAlignedBB *addcoord = AxisAlignedBB_addCoord(&self->boundingBox, var1, var3, var5);
        var35_n =
            World_getCollidingBoundingBoxes(self->worldObj, self, addcoord, var35, MAX_COL_BOXES);
    }

    for (int var20 = 0; var20 < var35_n; ++var20)
        var3 = AxisAlignedBB_calculateYOffset(&var35[var20], &self->boundingBox, var3);
    AxisAlignedBB_offset(&self->boundingBox, 0.0, var3, 0.0);

    if (!self->field_9293_aM && var13 != var3) {
        var5 = 0.0;
        var3 = 0.0;
        var1 = 0.0;
    }

    int var36 = self->onGround || (var13 != var3 && var13 < 0.0);

    int var21;
    for (var21 = 0; var21 < var35_n; ++var21)
        var1 = AxisAlignedBB_calculateXOffset(&var35[var21], &self->boundingBox, var1);
    AxisAlignedBB_offset(&self->boundingBox, var1, 0.0, 0.0);

    if (!self->field_9293_aM && var11 != var1) {
        var5 = 0.0;
        var3 = 0.0;
        var1 = 0.0;
    }

    for (var21 = 0; var21 < var35_n; ++var21)
        var5 = AxisAlignedBB_calculateZOffset(&var35[var21], &self->boundingBox, var5);
    AxisAlignedBB_offset(&self->boundingBox, 0.0, 0.0, var5);

    if (!self->field_9293_aM && var15 != var5) {
        var5 = 0.0;
        var3 = 0.0;
        var1 = 0.0;
    }

    if (self->stepHeight > 0.0f && var36 && (var18 || self->ySize < 0.05f) &&
        (var11 != var1 || var15 != var5)) {
        double var37 = var1;
        double var23 = var3;
        double var25 = var5;
        var1 = var11;
        var3 = (double)self->stepHeight;
        var5 = var15;
        AxisAlignedBB var27 = self->boundingBox;
        self->boundingBox = var17;

        AxisAlignedBB var35b[MAX_COL_BOXES];
        int var35b_n;
        {
            AxisAlignedBB *ac = AxisAlignedBB_addCoord(&self->boundingBox, var11, var3, var15);
            var35b_n =
                World_getCollidingBoundingBoxes(self->worldObj, self, ac, var35b, MAX_COL_BOXES);
        }

        int var28;
        for (var28 = 0; var28 < var35b_n; ++var28)
            var3 = AxisAlignedBB_calculateYOffset(&var35b[var28], &self->boundingBox, var3);
        AxisAlignedBB_offset(&self->boundingBox, 0.0, var3, 0.0);
        if (!self->field_9293_aM && var13 != var3) {
            var5 = 0.0;
            var3 = 0.0;
            var1 = 0.0;
        }

        for (var28 = 0; var28 < var35b_n; ++var28)
            var1 = AxisAlignedBB_calculateXOffset(&var35b[var28], &self->boundingBox, var1);
        AxisAlignedBB_offset(&self->boundingBox, var1, 0.0, 0.0);
        if (!self->field_9293_aM && var11 != var1) {
            var5 = 0.0;
            var3 = 0.0;
            var1 = 0.0;
        }

        for (var28 = 0; var28 < var35b_n; ++var28)
            var5 = AxisAlignedBB_calculateZOffset(&var35b[var28], &self->boundingBox, var5);
        AxisAlignedBB_offset(&self->boundingBox, 0.0, 0.0, var5);
        if (!self->field_9293_aM && var15 != var5) {
            var5 = 0.0;
            var3 = 0.0;
            var1 = 0.0;
        }

        if (!self->field_9293_aM && var13 != var3) {
            var5 = 0.0;
            var3 = 0.0;
            var1 = 0.0;
        } else {

            var3 = (double)(-self->stepHeight);
            for (var28 = 0; var28 < var35b_n; ++var28)
                var3 = AxisAlignedBB_calculateYOffset(&var35b[var28], &self->boundingBox, var3);
            AxisAlignedBB_offset(&self->boundingBox, 0.0, var3, 0.0);
        }

        if (var37 * var37 + var25 * var25 >= var1 * var1 + var5 * var5) {
            var1 = var37;
            var3 = var23;
            var5 = var25;
            self->boundingBox = var27;
        } else {
            double var41 = self->boundingBox.minY - (double)((int)self->boundingBox.minY);
            if (var41 > 0.0) {
                self->ySize = (float)((double)self->ySize + var41 + 0.01);
            }
        }
    }
#undef MAX_COL_BOXES

    self->posX = (self->boundingBox.minX + self->boundingBox.maxX) / 2.0;
    self->posY = self->boundingBox.minY + (double)self->yOffset - (double)self->ySize;
    self->posZ = (self->boundingBox.minZ + self->boundingBox.maxZ) / 2.0;

    self->isCollidedHorizontally = (var11 != var1 || var15 != var5);
    self->isCollidedVertically = (var13 != var3);
    self->onGround = (var13 != var3 && var13 < 0.0);
    self->isCollided = (self->isCollidedHorizontally || self->isCollidedVertically);

    Entity_updateFallState(self, var3, self->onGround);

    if (var11 != var1)
        self->motionX = 0.0;
    if (var13 != var3)
        self->motionY = 0.0;
    if (var15 != var5)
        self->motionZ = 0.0;

    double var37 = self->posX - var7;
    double var23 = self->posZ - var9;
    if (self->vtable->canTriggerWalking(self) && !var18 && self->ridingEntity == NULL) {
        self->distanceWalkedModified =
            (float)((double)self->distanceWalkedModified +
                    MathHelper_sqrt_double(var37 * var37 + var23 * var23) * 0.6);
        int var38 = MathHelper_floor_double(self->posX);
        int var26 = MathHelper_floor_double(self->posY - (double)0.2f - (double)self->yOffset);
        int var39 = MathHelper_floor_double(self->posZ);
        int var28 = World_getBlockId(self->worldObj, var38, var26, var39);
        if (World_getBlockId(self->worldObj, var38, var26 - 1, var39) == Block_fence->blockID) {
            var28 = World_getBlockId(self->worldObj, var38, var26 - 1, var39);
        }
        if (self->distanceWalkedModified > (float)self->nextStepDistance && var28 > 0) {
            ++self->nextStepDistance;
            StepSound *var29 = Block_blocksList[var28]->stepSound;
            if (World_getBlockId(self->worldObj, var38, var26 + 1, var39) == Block_snow->blockID) {
                var29 = Block_snow->stepSound;
                World_playSoundAtEntity(self->worldObj, self, StepSound_func_1145_d(var29),
                                        StepSound_getVolume(var29) * 0.15f,
                                        StepSound_getPitch(var29));
            } else if (!Material_getIsLiquid(Block_blocksList[var28]->blockMaterial)) {
                World_playSoundAtEntity(self->worldObj, self, StepSound_func_1145_d(var29),
                                        StepSound_getVolume(var29) * 0.15f,
                                        StepSound_getPitch(var29));
            }
            Block_blocksList[var28]->vtable->onEntityWalking(
                Block_blocksList[var28], self->worldObj, var38, var26, var39, self);
        }
    }

    {
        int var38b = MathHelper_floor_double(self->boundingBox.minX + 0.001);
        int var26b = MathHelper_floor_double(self->boundingBox.minY + 0.001);
        int var39b = MathHelper_floor_double(self->boundingBox.minZ + 0.001);
        int var28b = MathHelper_floor_double(self->boundingBox.maxX - 0.001);
        int var40 = MathHelper_floor_double(self->boundingBox.maxY - 0.001);
        int var30 = MathHelper_floor_double(self->boundingBox.maxZ - 0.001);
        if (World_checkChunksExist(self->worldObj, var38b, var26b, var39b, var28b, var40, var30)) {
            for (int var31 = var38b; var31 <= var28b; ++var31) {
                for (int var32 = var26b; var32 <= var40; ++var32) {
                    for (int var33 = var39b; var33 <= var30; ++var33) {
                        int var34 = World_getBlockId(self->worldObj, var31, var32, var33);
                        if (var34 > 0) {
                            Block_blocksList[var34]->vtable->onEntityCollidedWithBlock(
                                Block_blocksList[var34], self->worldObj, var31, var32, var33, self);
                        }
                    }
                }
            }
        }
    }

    {
        int var42 = Entity_isWet(self);
        AxisAlignedBB *shrunk = AxisAlignedBB_func_28195_e(&self->boundingBox, 0.001, 0.001, 0.001);
        if (World_isBoundingBoxBurning(self->worldObj, shrunk)) {
            self->vtable->dealFireDamage(self, 1);
            if (!var42) {
                ++self->fire;
                if (self->fire == 0)
                    self->fire = 300;
            }
        } else if (self->fire <= 0) {
            self->fire = -self->fireResistance;
        }
        if (var42 && self->fire > 0) {
            World_playSoundAtEntity(
                self->worldObj, self, "random.fizz", 0.7f,
                1.6f +
                    (JavaRandom_nextFloat(&self->rand) - JavaRandom_nextFloat(&self->rand)) * 0.4f);
            self->fire = -self->fireResistance;
        }
    }
}

static void default_setPosition(Entity *self, double x, double y, double z) {
    self->posX = x;
    self->posY = y;
    self->posZ = z;
    float hw = self->width / 2.0f;
    float h = self->height;
    AxisAlignedBB_setBounds(&self->boundingBox, x - hw, y - self->yOffset + self->ySize, z - hw,
                            x + hw, y - self->yOffset + self->ySize + h, z + hw);
}

static void default_setLocationAndAngles(Entity *self, double x, double y, double z, float yaw,
                                         float pitch) {
    self->lastTickPosX = self->prevPosX = self->posX = x;
    self->lastTickPosY = self->prevPosY = self->posY = y + self->yOffset;
    self->lastTickPosZ = self->prevPosZ = self->posZ = z;
    self->rotationYaw = yaw;
    self->rotationPitch = pitch;
    self->vtable->setPosition(self, self->posX, self->posY, self->posZ);
}

static void default_setPositionAndRotation2(Entity *self, double x, double y, double z, float yaw,
                                            float pitch, int steps) {
    Entity_setPositionAndRotation2(self, x, y, z, yaw, pitch, steps);
}
static void default_setVelocity(Entity *self, double vx, double vy, double vz) {
    Entity_setVelocity(self, vx, vy, vz);
}
static void default_addVelocity(Entity *self, double dx, double dy, double dz) {
    Entity_addVelocity(self, dx, dy, dz);
}
static AxisAlignedBB *default_getBoundingBox(Entity *self) {
    (void)self;
    return NULL;
}
static AxisAlignedBB *default_getCollisionBox(Entity *self, Entity *other) {
    (void)self;
    (void)other;
    return NULL;
}
static void default_applyEntityCollision(Entity *self, Entity *other) {
    if (other->riddenByEntity == self || other->ridingEntity == self)
        return;
    double dx = other->posX - self->posX;
    double dz = other->posZ - self->posZ;
    double mx = MathHelper_abs_max(dx, dz);
    if (mx >= 0.01f) {
        mx = MathHelper_sqrt_double(mx);
        dx /= mx;
        dz /= mx;
        double push = 1.0 / mx;
        if (push > 1.0)
            push = 1.0;
        dx *= push * 0.05 * (1.0 - self->entityCollisionReduction);
        dz *= push * 0.05 * (1.0 - self->entityCollisionReduction);
        Entity_addVelocity(self, -dx, 0.0, -dz);
        Entity_addVelocity(other, dx, 0.0, dz);
    }
}
static void default_onCollideWithPlayer(Entity *self, EntityPlayer *player) {
    (void)self;
    (void)player;
}
static int default_attackEntityFrom(Entity *self, Entity *source, int damage) {
    (void)source;
    (void)damage;
    Entity_setBeenAttacked(self);
    return 0;
}
static int default_interact(Entity *self, EntityPlayer *player) {
    (void)self;
    (void)player;
    return 0;
}
static void default_writeEntityToNBT(Entity *self, NBTTagCompound *tag) {
    (void)self;
    (void)tag;
}
static void default_readEntityFromNBT(Entity *self, NBTTagCompound *tag) {
    (void)self;
    (void)tag;
}
static float default_getShadowSize(Entity *self) { return self->height / 2.0f; }
static float default_getEyeHeight(Entity *self) {
    (void)self;
    return 0.0f;
}
static double default_getMountedYOffset(Entity *self) { return (double)self->height * 0.75; }
static double default_getYOffset(Entity *self) { return (double)self->yOffset; }
static int default_canBeCollidedWith(Entity *self) {
    (void)self;
    return 0;
}
static float default_getCollisionBorderSize(Entity *self) {
    (void)self;
    return 0.1f;
}
static int default_canBePushed(Entity *self) {
    (void)self;
    return 0;
}
static int default_canTriggerWalking(Entity *self) {
    (void)self;
    return 1;
}
static int default_isEntityAlive(Entity *self) { return !self->isDead; }
static void default_handleHealthUpdate(Entity *self, int8_t data) {
    (void)self;
    (void)data;
}
static void default_performHurtAnimation(Entity *self) { (void)self; }
static void default_onStruckByLightning(Entity *self, struct EntityLightningBolt *bolt) {
    (void)bolt;
    self->vtable->dealFireDamage(self, 5);
    ++self->fire;
    if (self->fire == 0)
        self->fire = 300;
}
static void default_fall(Entity *self, float distance) {
    if (self->riddenByEntity)
        self->riddenByEntity->vtable->fall(self->riddenByEntity, distance);
}
static void default_setEntityDead(Entity *self) { self->isDead = 1; }
static void default_kill(Entity *self) { self->vtable->setEntityDead(self); }
static double default_getDistanceSq(Entity *self, double x, double y, double z) {
    double dx = self->posX - x;
    double dy = self->posY - y;
    double dz = self->posZ - z;
    return dx * dx + dy * dy + dz * dz;
}
static int default_isInRangeToRenderDist(Entity *self, double distSq) {
    double edge = AxisAlignedBB_getAverageEdgeLength(&self->boundingBox);
    edge *= 64.0 * self->renderDistanceWeight;
    return distSq < edge * edge;
}
static int default_isInRangeToRenderVec3D(Entity *self, Vec3D *vec) {
    double dx = self->posX - vec->xCoord;
    double dy = self->posY - vec->yCoord;
    double dz = self->posZ - vec->zCoord;
    double distSq = dx * dx + dy * dy + dz * dz;
    return self->vtable->isInRangeToRenderDist(self, distSq);
}
static Vec3D *default_getLookVec(Entity *self) {
    (void)self;
    return NULL;
}
static void default_setInPortal(Entity *self) { (void)self; }
static void default_updateRidden(Entity *self) {
    if (self->ridingEntity->isDead) {
        self->ridingEntity = NULL;
        return;
    }
    self->motionX = self->motionY = self->motionZ = 0.0;
    self->vtable->onUpdate(self);
    if (self->ridingEntity) {
        self->ridingEntity->vtable->updateRiderPosition(self->ridingEntity);
        self->entityRiderYawDelta +=
            (self->ridingEntity->rotationYaw - self->ridingEntity->prevRotationYaw);
        self->entityRiderPitchDelta +=
            (self->ridingEntity->rotationPitch - self->ridingEntity->prevRotationPitch);
        while (self->entityRiderYawDelta >= 180.0)
            self->entityRiderYawDelta -= 360.0;
        while (self->entityRiderYawDelta < -180.0)
            self->entityRiderYawDelta += 360.0;
        while (self->entityRiderPitchDelta >= 180.0)
            self->entityRiderPitchDelta -= 360.0;
        while (self->entityRiderPitchDelta < -180.0)
            self->entityRiderPitchDelta += 360.0;
        double maxD = 10.0;
        double dy = self->entityRiderYawDelta * 0.5;
        double dp = self->entityRiderPitchDelta * 0.5;
        if (dy > maxD)
            dy = maxD;
        if (dy < -maxD)
            dy = -maxD;
        if (dp > maxD)
            dp = maxD;
        if (dp < -maxD)
            dp = -maxD;
        self->entityRiderYawDelta -= dy;
        self->entityRiderPitchDelta -= dp;
        self->rotationYaw = (float)((double)self->rotationYaw + dy);
        self->rotationPitch = (float)((double)self->rotationPitch + dp);
    }
}
static void default_updateRiderPosition(Entity *self) {
    if (!self->riddenByEntity)
        return;
    double yo = self->vtable->getMountedYOffset(self) +
                self->riddenByEntity->vtable->getYOffset(self->riddenByEntity);
    self->riddenByEntity->vtable->setPosition(self->riddenByEntity, self->posX, self->posY + yo,
                                              self->posZ);
}
static const char *default_getEntityString(Entity *self) {
    (void)self;
    return NULL;
}

int Entity_isEntityInsideOpaqueBlock_base(Entity *self);

EntityVtable Entity_defaultVtable = {
    .entityInit = default_entityInit,
    .onUpdate = default_onUpdate,
    .onEntityUpdate = default_onEntityUpdate,
    .moveEntity = default_moveEntity,
    .setPosition = default_setPosition,
    .setLocationAndAngles = default_setLocationAndAngles,
    .setPositionAndRotation2 = default_setPositionAndRotation2,
    .setVelocity = default_setVelocity,
    .addVelocity = default_addVelocity,
    .getBoundingBox = default_getBoundingBox,
    .getCollisionBox = default_getCollisionBox,
    .applyEntityCollision = default_applyEntityCollision,
    .onCollideWithPlayer = default_onCollideWithPlayer,
    .attackEntityFrom = default_attackEntityFrom,
    .interact = default_interact,
    .writeEntityToNBT = default_writeEntityToNBT,
    .readEntityFromNBT = default_readEntityFromNBT,
    .getShadowSize = default_getShadowSize,
    .getEyeHeight = default_getEyeHeight,
    .getMountedYOffset = default_getMountedYOffset,
    .getYOffset = default_getYOffset,
    .canBeCollidedWith = default_canBeCollidedWith,
    .getCollisionBorderSize = default_getCollisionBorderSize,
    .canBePushed = default_canBePushed,
    .canTriggerWalking = default_canTriggerWalking,
    .isEntityAlive = default_isEntityAlive,
    .handleHealthUpdate = default_handleHealthUpdate,
    .performHurtAnimation = default_performHurtAnimation,
    .onStruckByLightning = default_onStruckByLightning,
    .fall = default_fall,
    .setEntityDead = default_setEntityDead,
    .kill = default_kill,
    .getDistanceSq = default_getDistanceSq,
    .isInRangeToRenderDist = default_isInRangeToRenderDist,
    .isInRangeToRenderVec3D = default_isInRangeToRenderVec3D,
    .getLookVec = default_getLookVec,
    .setInPortal = default_setInPortal,
    .updateRiderPosition = default_updateRiderPosition,
    .updateRidden = default_updateRidden,
    .getEntityString = default_getEntityString,
    .getEntityBrightness = Entity_getEntityBrightness,
    .handleWaterMovement = Entity_handleWaterMovement,
    .dealFireDamage = Entity_dealFireDamage,
    .isInWater = Entity_isInWater,
    .preparePlayerToSpawn = Entity_preparePlayerToSpawn_base,
    .isEntityInsideOpaqueBlock = Entity_isEntityInsideOpaqueBlock_base,
    .isSneaking = Entity_isSneaking,
};

void Entity_initVtable(EntityVtable *vt) { *vt = Entity_defaultVtable; }

void Entity_construct(Entity *self, World *world) {
    memset(self, 0, sizeof(*self));
    self->vtable = &Entity_defaultVtable;
    self->entityId = Entity_nextEntityID++;
    self->renderDistanceWeight = 1.0;
    self->worldObj = world;
    self->width = 0.6f;
    self->height = 1.8f;
    self->field_9293_aM = 1;
    self->fireResistance = 1;
    self->nextStepDistance = 1;
    self->maxAir = 300;
    self->air = 300;
    self->isFirstUpdate = 1;
    JavaRandom_init(&self->rand, (int64_t)(intptr_t)self ^ Entity_nextEntityID);
    DataWatcher_init(&self->dataWatcher);
    DataWatcher_addByte(&self->dataWatcher, 0, 0);
    self->vtable->setPosition(self, 0.0, 0.0, 0.0);
}

#define ENTITY_FREE_DELAY 5

typedef struct {
    Entity *e;
    int countdown;
} DeferredFree;
static DeferredFree *s_queue = NULL;
static int s_queue_len = 0;
static int s_queue_cap = 0;
static EntityLiving **s_view_entity_ref = NULL;
static EntityLiving **s_player_ref = NULL;

void Entity_registerViewEntityRef(EntityLiving **ref) { s_view_entity_ref = ref; }
void Entity_registerPlayerRef(EntityLiving **ref) { s_player_ref = ref; }

static void entity_do_destroy(Entity *self) {
    if (s_view_entity_ref && *s_view_entity_ref == (EntityLiving *)self)
        *s_view_entity_ref = NULL;
    if (s_player_ref && *s_player_ref == (EntityLiving *)self)
        *s_player_ref = NULL;
    if (self->worldObj && self->worldObj->deadPlayer == self)
        self->worldObj->deadPlayer = NULL;
    DataWatcher_destroy(&self->dataWatcher);
    if (self->isMob || self->isAnimal || self->isWaterMob) {
        EntityCreature *ec = (EntityCreature *)self;
        PathEntity_destroy(ec->pathToEntity);
        ec->pathToEntity = NULL;
    }
    if (self->isPlayer) {
        EntityPlayer *ep = (EntityPlayer *)self;
        InventoryPlayer_destroy(&ep->inventory);
        if (ep->inventorySlots) {
            ContainerPlayer *cp = (ContainerPlayer *)ep->inventorySlots;
            for (int i = 0; i < cp->craftMatrix->stackListLength; i++)
                ItemStack_free(cp->craftMatrix->stackList[i]);
            free(cp->craftMatrix->stackList);
            free(cp->craftMatrix);
            InventoryCraftResult *cr = (InventoryCraftResult *)cp->craftResult;
            ItemStack_free(cr->stackResult[0]);
            free(cr);
            for (int i = 0; i < cp->base.slots.count; i++)
                free(cp->base.slots.entries[i]);
            free(cp->base.slots.entries);
            for (int i = 0; i < cp->base.field_20123_d.count; i++)
                ItemStack_free(cp->base.field_20123_d.entries[i]);
            free(cp->base.field_20123_d.entries);
            free(cp->base.field_20121_g.entries);
            free(cp);
            ep->inventorySlots = NULL;
            ep->craftingInventory = NULL;
        }
    }
    if (self->isPlayerSP) {
        EntityPlayerSP *sp = (EntityPlayerSP *)self;
        free(sp->movementInput);
        sp->movementInput = NULL;
    }
    if (self->isMinecart) {
        EntityMinecart *em = (EntityMinecart *)self;
        for (int i = 0; i < 36; i++)
            ItemStack_free(em->cargoItems[i]);
    }
    free(self);
}

void Entity_destroy(Entity *self) {
    if (!self)
        return;
    for (int _i = 0; _i < s_queue_len; _i++)
        if (s_queue[_i].e == self)
            return;
    if (s_queue_len >= s_queue_cap) {
        int newcap = s_queue_cap ? s_queue_cap * 2 : 64;
        s_queue = realloc(s_queue, (size_t)newcap * sizeof(DeferredFree));
        s_queue_cap = newcap;
    }
    s_queue[s_queue_len].e = self;
    s_queue[s_queue_len].countdown = ENTITY_FREE_DELAY;
    s_queue_len++;
}

void Entity_flush_destroy_queue_for_world(World *world) {
    int i = 0;
    while (i < s_queue_len) {
        if (s_queue[i].e->worldObj == world) {
            entity_do_destroy(s_queue[i].e);
            s_queue[i] = s_queue[--s_queue_len];
        } else {
            i++;
        }
    }
}

void Entity_flush_destroy_queue(void) {
    int i = 0;
    while (i < s_queue_len) {
        if (--s_queue[i].countdown <= 0) {
            entity_do_destroy(s_queue[i].e);
            s_queue[i] = s_queue[--s_queue_len];
        } else {
            i++;
        }
    }
}

void Entity_flush_destroy_queue_force(void) {
    while (s_queue_len > 0) {
        entity_do_destroy(s_queue[--s_queue_len].e);
    }
}

void Entity_setEntityDead(Entity *self) { self->vtable->setEntityDead(self); }

void Entity_setSize(Entity *self, float w, float h) {
    self->width = w;
    self->height = h;
}

void Entity_setRotation(Entity *self, float yaw, float pitch) {
    self->rotationYaw = fmodf(yaw, 360.0f);
    self->rotationPitch = fmodf(pitch, 360.0f);
}

void Entity_setPositionAndRotation(Entity *self, double x, double y, double z, float yaw,
                                   float pitch) {
    self->prevPosX = self->posX = x;
    self->prevPosY = self->posY = y;
    self->prevPosZ = self->posZ = z;
    self->prevRotationYaw = self->rotationYaw = yaw;
    self->prevRotationPitch = self->rotationPitch = pitch;
    self->ySize = 0.0f;
    double d = (double)(self->prevRotationYaw - yaw);
    if (d < -180.0)
        self->prevRotationYaw += 360.0f;
    if (d >= 180.0)
        self->prevRotationYaw -= 360.0f;
    self->vtable->setPosition(self, x, y, z);
    Entity_setRotation(self, yaw, pitch);
}

void Entity_setPositionAndRotation2(Entity *self, double x, double y, double z, float yaw,
                                    float pitch, int steps) {
    (void)steps;
    self->vtable->setPosition(self, x, y, z);
    Entity_setRotation(self, yaw, pitch);
    AxisAlignedBB *exp =
        AxisAlignedBB_func_28195_e(&self->boundingBox, 1.0 / 32.0, 0.0, 1.0 / 32.0);
#define MAX_R2_BOXES 64
    AxisAlignedBB boxes[MAX_R2_BOXES];
    int n = World_getCollidingBoundingBoxes(self->worldObj, self, exp, boxes, MAX_R2_BOXES);
    if (n > 0) {
        double maxY = 0.0;
        for (int i = 0; i < n; ++i) {
            if (boxes[i].maxY > maxY)
                maxY = boxes[i].maxY;
        }
        y += maxY - self->boundingBox.minY;
        self->vtable->setPosition(self, x, y, z);
    }
#undef MAX_R2_BOXES
}

void Entity_addVelocity(Entity *self, double dx, double dy, double dz) {
    self->motionX += dx;
    self->motionY += dy;
    self->motionZ += dz;
}

void Entity_setVelocity(Entity *self, double vx, double vy, double vz) {
    self->motionX = vx;
    self->motionY = vy;
    self->motionZ = vz;
}

void Entity_mountEntity(Entity *self, Entity *mount) {
    self->entityRiderPitchDelta = 0.0;
    self->entityRiderYawDelta = 0.0;
    if (mount == NULL) {
        if (self->ridingEntity) {
            self->vtable->setLocationAndAngles(
                self, self->ridingEntity->posX,
                self->ridingEntity->boundingBox.minY + self->ridingEntity->height,
                self->ridingEntity->posZ, self->rotationYaw, self->rotationPitch);
            self->ridingEntity->riddenByEntity = NULL;
        }
        self->ridingEntity = NULL;
    } else if (self->ridingEntity == mount) {
        self->ridingEntity->riddenByEntity = NULL;
        self->ridingEntity = NULL;
        self->vtable->setLocationAndAngles(self, mount->posX,
                                           mount->boundingBox.minY + mount->height, mount->posZ,
                                           self->rotationYaw, self->rotationPitch);
    } else {
        if (self->ridingEntity)
            self->ridingEntity->riddenByEntity = NULL;
        if (mount->riddenByEntity)
            mount->riddenByEntity->ridingEntity = NULL;
        self->ridingEntity = mount;
        mount->riddenByEntity = self;
    }
}

void Entity_updateRidden(Entity *self) { self->vtable->updateRidden(self); }

int Entity_isWet(Entity *self) {
    return self->inWater ||
           World_canBlockBeRainedOn(self->worldObj, MathHelper_floor_double(self->posX),
                                    MathHelper_floor_double(self->posY),
                                    MathHelper_floor_double(self->posZ));
}
int Entity_isInWater(Entity *self) { return self->inWater; }
int Entity_handleWaterMovement(Entity *self) {
    AxisAlignedBB *bb = AxisAlignedBB_expand(&self->boundingBox, 0.0, (double)-0.4f, 0.0);
    AxisAlignedBB *shrunk = AxisAlignedBB_func_28195_e(bb, 0.001, 0.001, 0.001);
    return World_handleMaterialAcceleration(self->worldObj, shrunk, Material_water, self);
}
int Entity_handleLavaMovement(Entity *self) {
    AxisAlignedBB *bb =
        AxisAlignedBB_expand(&self->boundingBox, (double)-0.1f, (double)-0.4f, (double)-0.1f);
    return World_isMaterialInBB(self->worldObj, bb, Material_lava);
}
int Entity_isInsideOfMaterial(Entity *self, Material *mat) {
    double var2 = self->posY + (double)self->vtable->getEyeHeight(self);
    int var4 = MathHelper_floor_double(self->posX);
    int var5 = MathHelper_floor_double(var2);
    int var6 = MathHelper_floor_double(self->posZ);
    int var7 = World_getBlockId(self->worldObj, var4, var5, var6);
    if (var7 != 0 && Block_blocksList[var7] && Block_blocksList[var7]->blockMaterial == mat) {
        float var8 =
            BlockFluid_getPercentAir(World_getBlockMetadata(self->worldObj, var4, var5, var6)) -
            1.0f / 9.0f;
        float var9 = (float)(var5 + 1) - var8;
        return var2 < (double)var9;
    }
    return 0;
}
int Entity_isOffsetPositionInLiquid(Entity *self, double dx, double dy, double dz) {
    AxisAlignedBB *var7 = AxisAlignedBB_getOffsetBoundingBox(&self->boundingBox, dx, dy, dz);
    AxisAlignedBB tmp;
    int n = World_getCollidingBoundingBoxes(self->worldObj, self, var7, &tmp, 1);
    if (n > 0)
        return 0;
    return !World_getIsAnyLiquid(self->worldObj, var7);
}
void Entity_moveFlying(Entity *self, float strafe, float forward, float speed) {
    float len = MathHelper_sqrt_float(strafe * strafe + forward * forward);
    if (len < 0.01f)
        return;
    if (len < 1.0f)
        len = 1.0f;
    float s = speed / len;
    strafe *= s;
    forward *= s;
    float sinY = MathHelper_sin(self->rotationYaw * (float)M_PI / 180.0f);
    float cosY = MathHelper_cos(self->rotationYaw * (float)M_PI / 180.0f);
    self->motionX += (double)(strafe * cosY - forward * sinY);
    self->motionZ += (double)(forward * cosY + strafe * sinY);
}
float Entity_getEntityBrightness(Entity *self, float partialTick) {
    (void)partialTick;
    int var2 = MathHelper_floor_double(self->posX);
    double var3 = (self->boundingBox.maxY - self->boundingBox.minY) * 0.66;
    int var5 = MathHelper_floor_double(self->posY - (double)self->yOffset + var3);
    int var6 = MathHelper_floor_double(self->posZ);
    if (World_checkChunksExist(self->worldObj, MathHelper_floor_double(self->boundingBox.minX),
                               MathHelper_floor_double(self->boundingBox.minY),
                               MathHelper_floor_double(self->boundingBox.minZ),
                               MathHelper_floor_double(self->boundingBox.maxX),
                               MathHelper_floor_double(self->boundingBox.maxY),
                               MathHelper_floor_double(self->boundingBox.maxZ))) {
        float var7 = World_getLightBrightness(self->worldObj, var2, var5, var6);
        if (var7 < self->entityBrightness)
            var7 = self->entityBrightness;
        return var7;
    }
    return self->entityBrightness;
}
Vec3D *Entity_getLook(Entity *self, float partialTick) {
    return entityLiving_getLook(self, partialTick);
}

void Entity_preparePlayerToSpawn(Entity *self) { self->vtable->preparePlayerToSpawn(self); }

void Entity_preparePlayerToSpawn_base(Entity *self) {
    if (self->worldObj != NULL) {
        while (self->posY > 0.0) {
            self->vtable->setPosition(self, self->posX, self->posY, self->posZ);
            AxisAlignedBB tmp;
            if (World_getCollidingBoundingBoxes(self->worldObj, self, &self->boundingBox, &tmp,
                                                1) == 0) {
                break;
            }
            ++self->posY;
        }
        self->motionX = self->motionY = self->motionZ = 0.0;
        self->rotationPitch = 0.0f;
    }
}
int Entity_isEntityInsideOpaqueBlock_base(Entity *self) {
    for (int var1 = 0; var1 < 8; ++var1) {
        float var2 = ((float)((var1 >> 0) % 2) - 0.5f) * self->width * 0.9f;
        float var3 = ((float)((var1 >> 1) % 2) - 0.5f) * 0.1f;
        float var4 = ((float)((var1 >> 2) % 2) - 0.5f) * self->width * 0.9f;
        int var5 = MathHelper_floor_double(self->posX + (double)var2);
        int var6 = MathHelper_floor_double(self->posY + (double)self->vtable->getEyeHeight(self) +
                                           (double)var3);
        int var7 = MathHelper_floor_double(self->posZ + (double)var4);
        if (World_isBlockNormalCube(self->worldObj, var5, var6, var7)) {
            return 1;
        }
    }
    return 0;
}
int Entity_isEntityInsideOpaqueBlock(Entity *self) {
    return self->vtable->isEntityInsideOpaqueBlock(self);
}
float Entity_getDistanceToEntity(Entity *self, Entity *other) {
    float dx = (float)(self->posX - other->posX);
    float dy = (float)(self->posY - other->posY);
    float dz = (float)(self->posZ - other->posZ);
    return MathHelper_sqrt_float(dx * dx + dy * dy + dz * dz);
}
double Entity_getDistanceSqToEntity(Entity *self, Entity *other) {
    double dx = self->posX - other->posX;
    double dy = self->posY - other->posY;
    double dz = self->posZ - other->posZ;
    return dx * dx + dy * dy + dz * dz;
}
double Entity_getDistance(Entity *self, double x, double y, double z) {
    double dx = self->posX - x;
    double dy = self->posY - y;
    double dz = self->posZ - z;
    return (double)MathHelper_sqrt_double(dx * dx + dy * dy + dz * dz);
}
void Entity_writeToNBT(Entity *self, NBTTagCompound *tag) {

    NBTTagList *pos = NBTTagList_create();
    NBTTagList_setTag(pos, (NBTBase *)NBTTagDouble_createWithValue(self->posX));
    NBTTagList_setTag(pos,
                      (NBTBase *)NBTTagDouble_createWithValue(self->posY + (double)self->ySize));
    NBTTagList_setTag(pos, (NBTBase *)NBTTagDouble_createWithValue(self->posZ));
    NBTTagCompound_setTag(tag, "Pos", (NBTBase *)pos);

    NBTTagList *mot = NBTTagList_create();
    NBTTagList_setTag(mot, (NBTBase *)NBTTagDouble_createWithValue(self->motionX));
    NBTTagList_setTag(mot, (NBTBase *)NBTTagDouble_createWithValue(self->motionY));
    NBTTagList_setTag(mot, (NBTBase *)NBTTagDouble_createWithValue(self->motionZ));
    NBTTagCompound_setTag(tag, "Motion", (NBTBase *)mot);

    NBTTagList *rot = NBTTagList_create();
    NBTTagList_setTag(rot, (NBTBase *)NBTTagFloat_createWithValue(self->rotationYaw));
    NBTTagList_setTag(rot, (NBTBase *)NBTTagFloat_createWithValue(self->rotationPitch));
    NBTTagCompound_setTag(tag, "Rotation", (NBTBase *)rot);

    NBTTagCompound_setFloat(tag, "FallDistance", self->fallDistance);
    NBTTagCompound_setShort(tag, "Fire", (int16_t)self->fire);
    NBTTagCompound_setShort(tag, "Air", (int16_t)self->air);
    NBTTagCompound_setBoolean(tag, "OnGround", self->onGround);
    self->vtable->writeEntityToNBT(self, tag);
}
void Entity_readFromNBT(Entity *self, NBTTagCompound *tag) {
    NBTTagList *var2 = NBTTagCompound_getTagList(tag, "Pos");
    NBTTagList *var3 = NBTTagCompound_getTagList(tag, "Motion");
    NBTTagList *var4 = NBTTagCompound_getTagList(tag, "Rotation");
    self->motionX = ((NBTTagDouble *)NBTTagList_tagAt(var3, 0))->doubleValue;
    self->motionY = ((NBTTagDouble *)NBTTagList_tagAt(var3, 1))->doubleValue;
    self->motionZ = ((NBTTagDouble *)NBTTagList_tagAt(var3, 2))->doubleValue;
    if (fabs(self->motionX) > 10.0)
        self->motionX = 0.0;
    if (fabs(self->motionY) > 10.0)
        self->motionY = 0.0;
    if (fabs(self->motionZ) > 10.0)
        self->motionZ = 0.0;
    self->prevPosX = self->lastTickPosX = self->posX =
        ((NBTTagDouble *)NBTTagList_tagAt(var2, 0))->doubleValue;
    self->prevPosY = self->lastTickPosY = self->posY =
        ((NBTTagDouble *)NBTTagList_tagAt(var2, 1))->doubleValue;
    self->prevPosZ = self->lastTickPosZ = self->posZ =
        ((NBTTagDouble *)NBTTagList_tagAt(var2, 2))->doubleValue;
    self->prevRotationYaw = self->rotationYaw =
        ((NBTTagFloat *)NBTTagList_tagAt(var4, 0))->floatValue;
    self->prevRotationPitch = self->rotationPitch =
        ((NBTTagFloat *)NBTTagList_tagAt(var4, 1))->floatValue;
    self->fallDistance = NBTTagCompound_getFloat(tag, "FallDistance");
    self->fire = NBTTagCompound_getShort(tag, "Fire");
    self->air = NBTTagCompound_getShort(tag, "Air");
    self->onGround = NBTTagCompound_getBoolean(tag, "OnGround");
    self->vtable->setPosition(self, self->posX, self->posY, self->posZ);
    Entity_setRotation(self, self->rotationYaw, self->rotationPitch);
    self->vtable->readEntityFromNBT(self, tag);
}
int Entity_addEntityID(Entity *self, NBTTagCompound *tag) {

    const char *str = EntityList_getEntityString(self);
    if (!self->isDead && str) {
        NBTTagCompound_setString(tag, "id", str);
        Entity_writeToNBT(self, tag);
        return 1;
    }
    return 0;
}
void Entity_addToPlayerScore(Entity *self, Entity *scorer, int amount) {
    (void)self;
    (void)scorer;
    (void)amount;
}
int Entity_isBurning(Entity *self) { return self->fire > 0 || Entity_getEntityFlag(self, 0); }
int Entity_isRiding(Entity *self) {
    return self->ridingEntity != NULL || Entity_getEntityFlag(self, 2);
}
int Entity_isSneaking(Entity *self) { return Entity_getEntityFlag(self, 1); }
int Entity_getEntityFlag(Entity *self, int flag) {
    return (DataWatcher_getWatchableObjectByte(&self->dataWatcher, 0) & (1 << flag)) != 0;
}
void Entity_setEntityFlag(Entity *self, int flag, int val) {
    int8_t b = DataWatcher_getWatchableObjectByte(&self->dataWatcher, 0);
    if (val)
        DataWatcher_updateByte(&self->dataWatcher, 0, (int8_t)(b | (1 << flag)));
    else
        DataWatcher_updateByte(&self->dataWatcher, 0, (int8_t)(b & ~(1 << flag)));
}
struct EntityItem *Entity_dropItem(Entity *self, int itemId, int count) {
    return Entity_dropItemWithOffset(self, itemId, count, 0.0f);
}
struct EntityItem *Entity_dropItemWithOffset(Entity *self, int itemId, int count, float yOff) {
    ItemStack is = ItemStack_fromBlockID(itemId, count, 0);
    return Entity_entityDropItem(self, is, yOff);
}
struct EntityItem *Entity_entityDropItem(Entity *self, ItemStack item, float yOff) {
    EntityItem *var3 =
        EntityItem_create(self->worldObj, self->posX, self->posY + (double)yOff, self->posZ, item);
    var3->delayBeforeCanPickup = 10;
    World_entityJoinedWorld(self->worldObj, (Entity *)var3);
    return var3;
}

void Entity_func_346_d(Entity *self, float var1, float var2) {
    float var3 = self->rotationPitch;
    float var4 = self->rotationYaw;
    self->rotationYaw = (float)((double)self->rotationYaw + (double)var1 * 0.15);
    self->rotationPitch = (float)((double)self->rotationPitch - (double)var2 * 0.15);
    if (self->rotationPitch < -90.0f)
        self->rotationPitch = -90.0f;
    if (self->rotationPitch > 90.0f)
        self->rotationPitch = 90.0f;
    self->prevRotationPitch += self->rotationPitch - var3;
    self->prevRotationYaw += self->rotationYaw - var4;
}

void Entity_setWorld(Entity *self, World *world) { self->worldObj = world; }

const char *Entity_getEntityTexture(Entity *self) {
    (void)self;
    return NULL;
}

void Entity_updateCloak(Entity *self) { (void)self; }

void Entity_outfitWithItem(Entity *self, int var1, int var2, int var3) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
}

void Entity_setBeenAttacked(Entity *self) { self->beenAttacked = 1; }
void Entity_setOnFireFromLava(Entity *self) {
    if (!self->isImmuneToFire) {
        self->vtable->attackEntityFrom(self, NULL, 4);
        self->fire = 600;
    }
}
void Entity_dealFireDamage(Entity *self, int amount) {
    if (!self->isImmuneToFire)
        self->vtable->attackEntityFrom(self, NULL, amount);
}
void Entity_updateFallState(Entity *self, double motY, int onGround) {
    if (onGround) {
        if (self->fallDistance > 0.0f) {
            self->vtable->fall(self, self->fallDistance);
            self->fallDistance = 0.0f;
        }
    } else if (motY < 0.0) {
        self->fallDistance -= (float)motY;
    }
}
void Entity_onKillEntity(Entity *self, Entity *killed) {
    (void)self;
    (void)killed;
}
DataWatcher *Entity_getDataWatcher(Entity *self) { return &self->dataWatcher; }

int Entity_isPlayerSleeping(Entity *self) {
    (void)self;
    return 0;
}

static Vec3D *entityLiving_getPosition(Entity *self, float var1) {
    if (var1 == 1.0f) {
        return Vec3D_createVector(self->posX, self->posY, self->posZ);
    } else {
        double var2 = self->prevPosX + (self->posX - self->prevPosX) * (double)var1;
        double var4 = self->prevPosY + (self->posY - self->prevPosY) * (double)var1;
        double var6 = self->prevPosZ + (self->posZ - self->prevPosZ) * (double)var1;
        return Vec3D_createVector(var2, var4, var6);
    }
}

static Vec3D *entityLiving_getLook(Entity *self, float var1) {
    float var2;
    float var3;
    float var4;
    float var5;
    if (var1 == 1.0f) {
        var2 = cosf(-self->rotationYaw * ((float)M_PI / 180.0f) - (float)M_PI);
        var3 = sinf(-self->rotationYaw * ((float)M_PI / 180.0f) - (float)M_PI);
        var4 = -cosf(-self->rotationPitch * ((float)M_PI / 180.0f));
        var5 = sinf(-self->rotationPitch * ((float)M_PI / 180.0f));
        return Vec3D_createVector((double)(var3 * var4), (double)var5, (double)(var2 * var4));
    } else {
        var2 = self->prevRotationPitch + (self->rotationPitch - self->prevRotationPitch) * var1;
        var3 = self->prevRotationYaw + (self->rotationYaw - self->prevRotationYaw) * var1;
        var4 = cosf(-var3 * ((float)M_PI / 180.0f) - (float)M_PI);
        var5 = sinf(-var3 * ((float)M_PI / 180.0f) - (float)M_PI);
        float var6 = -cosf(-var2 * ((float)M_PI / 180.0f));
        float var7 = sinf(-var2 * ((float)M_PI / 180.0f));
        return Vec3D_createVector((double)(var5 * var6), (double)var7, (double)(var4 * var6));
    }
}

MovingObjectPosition *Entity_rayTrace(Entity *self, double var1, float var3) {
    Vec3D *var4 = entityLiving_getPosition(self, var3);
    Vec3D *var5 = entityLiving_getLook(self, var3);
    Vec3D *var6 =
        Vec3D_addVector(var4, var5->xCoord * var1, var5->yCoord * var1, var5->zCoord * var1);
    return World_rayTraceBlocks(self->worldObj, var4, var6);
}

int Entity_pushOutOfBlocks(Entity *self, double var1, double var3, double var5) {
    int var7 = MathHelper_floor_double(var1);
    int var8 = MathHelper_floor_double(var3);
    int var9 = MathHelper_floor_double(var5);
    double var10 = var1 - (double)var7;
    double var12 = var3 - (double)var8;
    double var14 = var5 - (double)var9;
    if (World_isBlockNormalCube(self->worldObj, var7, var8, var9)) {
        int var16 = !World_isBlockNormalCube(self->worldObj, var7 - 1, var8, var9);
        int var17 = !World_isBlockNormalCube(self->worldObj, var7 + 1, var8, var9);
        int var18 = !World_isBlockNormalCube(self->worldObj, var7, var8 - 1, var9);
        int var19 = !World_isBlockNormalCube(self->worldObj, var7, var8 + 1, var9);
        int var20 = !World_isBlockNormalCube(self->worldObj, var7, var8, var9 - 1);
        int var21 = !World_isBlockNormalCube(self->worldObj, var7, var8, var9 + 1);
        int var22 = -1;
        double var23 = 9999.0;
        if (var16 && var10 < var23) {
            var23 = var10;
            var22 = 0;
        }
        if (var17 && 1.0 - var10 < var23) {
            var23 = 1.0 - var10;
            var22 = 1;
        }
        if (var18 && var12 < var23) {
            var23 = var12;
            var22 = 2;
        }
        if (var19 && 1.0 - var12 < var23) {
            var23 = 1.0 - var12;
            var22 = 3;
        }
        if (var20 && var14 < var23) {
            var23 = var14;
            var22 = 4;
        }
        if (var21 && 1.0 - var14 < var23) {
            var23 = 1.0 - var14;
            var22 = 5;
        }
        float var25 = JavaRandom_nextFloat(&self->rand) * 0.2f + 0.1f;
        (void)var23;
        if (var22 == 0) {
            self->motionX = (double)(-var25);
        }
        if (var22 == 1) {
            self->motionX = (double)var25;
        }
        if (var22 == 2) {
            self->motionY = (double)(-var25);
        }
        if (var22 == 3) {
            self->motionY = (double)var25;
        }
        if (var22 == 4) {
            self->motionZ = (double)(-var25);
        }
        if (var22 == 5) {
            self->motionZ = (double)var25;
        }
    }
    return 0;
}
