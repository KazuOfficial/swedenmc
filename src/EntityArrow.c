#include "EntityArrow.h"
#include "World.h"
#include "MathHelper.h"
#include "Item.h"
#include "Block.h"
#include "Vec3D.h"
#include "MovingObjectPosition.h"
#include "AxisAlignedBB.h"
#include "InventoryPlayer.h"
#include "EntityPlayer.h"
#include <math.h>
#include <stdlib.h>

static void vtable_entityInit(Entity *self) { EntityArrow_entityInit((EntityArrow *)self); }
static void vtable_onUpdate(Entity *self) { EntityArrow_onUpdate((EntityArrow *)self); }
static void vtable_writeEntityToNBT(Entity *self, NBTTagCompound *var1) {
    EntityArrow_writeEntityToNBT((EntityArrow *)self, var1);
}
static void vtable_readEntityFromNBT(Entity *self, NBTTagCompound *var1) {
    EntityArrow_readEntityFromNBT((EntityArrow *)self, var1);
}
static void vtable_setVelocity(Entity *self, double var1, double var3, double var5) {
    EntityArrow_setVelocity((EntityArrow *)self, var1, var3, var5);
}
static void vtable_onCollideWithPlayer(Entity *self, EntityPlayer *var1) {
    EntityArrow_onCollideWithPlayer((EntityArrow *)self, var1);
}
static float vtable_getShadowSize(Entity *self) {
    return EntityArrow_getShadowSize((EntityArrow *)self);
}

static EntityVtable EntityArrow_vtable;
static int EntityArrow_vtable_init = 0;

static const char *arrow_getEntityString(Entity *self) {
    (void)self;
    return "Arrow";
}

static void ensure_vtable(void) {
    if (EntityArrow_vtable_init)
        return;
    EntityArrow_vtable_init = 1;
    Entity_initVtable(&EntityArrow_vtable);
    EntityArrow_vtable.getEntityString = arrow_getEntityString;
    EntityArrow_vtable.entityInit = vtable_entityInit;
    EntityArrow_vtable.onUpdate = vtable_onUpdate;
    EntityArrow_vtable.setVelocity = vtable_setVelocity;
    EntityArrow_vtable.writeEntityToNBT = vtable_writeEntityToNBT;
    EntityArrow_vtable.readEntityFromNBT = vtable_readEntityFromNBT;
    EntityArrow_vtable.onCollideWithPlayer = vtable_onCollideWithPlayer;
    EntityArrow_vtable.getShadowSize = vtable_getShadowSize;
}

void EntityArrow_construct1(EntityArrow *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityArrow_vtable;
    self->base.isArrow = 1;
    self->xTile = -1;
    self->yTile = -1;
    self->zTile = -1;
    self->inTile = 0;
    self->field_28019_h = 0;
    self->inGround = 0;
    self->doesArrowBelongToPlayer = 0;
    self->arrowShake = 0;
    self->owner = NULL;
    self->ticksInGround = 0;
    self->ticksInAir = 0;
    Entity_setSize(&self->base, 0.5F, 0.5F);
}

void EntityArrow_construct2(EntityArrow *self, World *var1, double var2, double var4, double var6) {
    EntityArrow_construct1(self, var1);

    self->base.vtable->setPosition(&self->base, var2, var4, var6);
    self->base.yOffset = 0.0F;
}

void EntityArrow_construct3(EntityArrow *self, World *var1, EntityLiving *var2) {
    EntityArrow_construct1(self, var1);
    self->owner = var2;
    self->doesArrowBelongToPlayer = ((Entity *)var2)->isPlayer;

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
    self->base.motionX = (double)(-MathHelper_sin(self->base.rotationYaw / 180.0F * (float)M_PI) *
                                  MathHelper_cos(self->base.rotationPitch / 180.0F * (float)M_PI));
    self->base.motionZ = (double)(MathHelper_cos(self->base.rotationYaw / 180.0F * (float)M_PI) *
                                  MathHelper_cos(self->base.rotationPitch / 180.0F * (float)M_PI));
    self->base.motionY = (double)(-MathHelper_sin(self->base.rotationPitch / 180.0F * (float)M_PI));
    EntityArrow_setArrowHeading(self, self->base.motionX, self->base.motionY, self->base.motionZ,
                                1.5F, 1.0F);
}

EntityArrow *EntityArrow_create1(World *var1) {
    EntityArrow *self = (EntityArrow *)calloc(1, sizeof(EntityArrow));
    EntityArrow_construct1(self, var1);
    return self;
}

EntityArrow *EntityArrow_create2(World *var1, double var2, double var4, double var6) {
    EntityArrow *self = (EntityArrow *)calloc(1, sizeof(EntityArrow));
    EntityArrow_construct2(self, var1, var2, var4, var6);
    return self;
}

EntityArrow *EntityArrow_create3(World *var1, EntityLiving *var2) {
    EntityArrow *self = (EntityArrow *)calloc(1, sizeof(EntityArrow));
    EntityArrow_construct3(self, var1, var2);
    return self;
}

void EntityArrow_entityInit(EntityArrow *self) { (void)self; }

void EntityArrow_setArrowHeading(EntityArrow *self, double var1, double var3, double var5,
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
    self->ticksInGround = 0;
}

void EntityArrow_setVelocity(EntityArrow *self, double var1, double var3, double var5) {
    self->base.motionX = var1;
    self->base.motionY = var3;
    self->base.motionZ = var5;
    if (self->base.prevRotationPitch == 0.0F && self->base.prevRotationYaw == 0.0F) {
        float var7 = MathHelper_sqrt_double(var1 * var1 + var5 * var5);
        self->base.prevRotationYaw = self->base.rotationYaw =
            (float)(atan2(var1, var5) * 180.0 / (double)((float)M_PI));
        self->base.prevRotationPitch = self->base.rotationPitch =
            (float)(atan2(var3, (double)var7) * 180.0 / (double)((float)M_PI));
        self->base.prevRotationPitch = self->base.rotationPitch;
        self->base.prevRotationYaw = self->base.rotationYaw;
        self->base.vtable->setLocationAndAngles(&self->base, self->base.posX, self->base.posY,
                                                self->base.posZ, self->base.rotationYaw,
                                                self->base.rotationPitch);
        self->ticksInGround = 0;
    }
}

void EntityArrow_onUpdate(EntityArrow *self) {

    Entity_defaultVtable.onUpdate(&self->base);

    if (self->base.prevRotationPitch == 0.0F && self->base.prevRotationYaw == 0.0F) {
        float var1 = MathHelper_sqrt_double(self->base.motionX * self->base.motionX +
                                            self->base.motionZ * self->base.motionZ);
        self->base.prevRotationYaw = self->base.rotationYaw =
            (float)(atan2(self->base.motionX, self->base.motionZ) * 180.0 / (double)((float)M_PI));
        self->base.prevRotationPitch = self->base.rotationPitch =
            (float)(atan2(self->base.motionY, (double)var1) * 180.0 / (double)((float)M_PI));
    }

    int var15 = World_getBlockId(self->base.worldObj, self->xTile, self->yTile, self->zTile);
    if (var15 > 0 && Block_blocksList[var15]) {
        Block_blocksList[var15]->vtable->setBlockBoundsBasedOnState(
            Block_blocksList[var15], (IBlockAccess *)self->base.worldObj, self->xTile, self->yTile,
            self->zTile);
        AxisAlignedBB *var2 = Block_blocksList[var15]->vtable->getCollisionBoundingBoxFromPool(
            Block_blocksList[var15], self->base.worldObj, self->xTile, self->yTile, self->zTile);
        Vec3D *posVec = Vec3D_createVector(self->base.posX, self->base.posY, self->base.posZ);
        if (var2 != NULL && AxisAlignedBB_isVecInside(var2, posVec)) {
            self->inGround = 1;
        }
    }

    if (self->arrowShake > 0) {
        --self->arrowShake;
    }

    if (self->inGround) {
        var15 = World_getBlockId(self->base.worldObj, self->xTile, self->yTile, self->zTile);
        int var18 =
            World_getBlockMetadata(self->base.worldObj, self->xTile, self->yTile, self->zTile);
        if (var15 == self->inTile && var18 == self->field_28019_h) {
            ++self->ticksInGround;
            if (self->ticksInGround == 1200) {
                Entity_setEntityDead(&self->base);
            }
        } else {
            self->inGround = 0;
            self->base.motionX *= (double)(JavaRandom_nextFloat(&self->base.rand) * 0.2F);
            self->base.motionY *= (double)(JavaRandom_nextFloat(&self->base.rand) * 0.2F);
            self->base.motionZ *= (double)(JavaRandom_nextFloat(&self->base.rand) * 0.2F);
            self->ticksInGround = 0;
            self->ticksInAir = 0;
        }
    } else {
        ++self->ticksInAir;
        Vec3D *var16 = Vec3D_createVector(self->base.posX, self->base.posY, self->base.posZ);
        Vec3D *var17 = Vec3D_createVector(self->base.posX + self->base.motionX,
                                          self->base.posY + self->base.motionY,
                                          self->base.posZ + self->base.motionZ);
        MovingObjectPosition *var3 = World_func_28105_a(self->base.worldObj, var16, var17, 0, 1);
        var16 = Vec3D_createVector(self->base.posX, self->base.posY, self->base.posZ);
        var17 = Vec3D_createVector(self->base.posX + self->base.motionX,
                                   self->base.posY + self->base.motionY,
                                   self->base.posZ + self->base.motionZ);
        if (var3 != NULL) {
            var17 = Vec3D_createVector(var3->hitVec->xCoord, var3->hitVec->yCoord,
                                       var3->hitVec->zCoord);
        }

        Entity *var4 = NULL;
        double var6 = 0.0;
        float var10;

        AxisAlignedBB *movBB = AxisAlignedBB_addCoord(&self->base.boundingBox, self->base.motionX,
                                                      self->base.motionY, self->base.motionZ);
        AxisAlignedBB *searchBB = AxisAlignedBB_expand(movBB, 1.0, 1.0, 1.0);
        Entity *entityBuf[64];
        int ecount = World_getEntitiesWithinAABBExcludingEntity(self->base.worldObj, &self->base,
                                                                searchBB, entityBuf, 64);

        for (int var8 = 0; var8 < ecount; ++var8) {
            Entity *var9 = entityBuf[var8];
            if (var9->vtable->canBeCollidedWith(var9) &&
                (self->owner == NULL || var9 != &self->owner->base || self->ticksInAir >= 5)) {
                var10 = 0.3F;
                AxisAlignedBB *var11 = AxisAlignedBB_expand(&var9->boundingBox, (double)var10,
                                                            (double)var10, (double)var10);
                MovingObjectPosition *var12 = AxisAlignedBB_func_1169_a(var11, var16, var17);
                if (var12 != NULL) {
                    double var13 = Vec3D_distanceTo(var16, var12->hitVec);
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

        float var19;
        if (var3 != NULL) {
            if (var3->entityHit != NULL) {
                Entity *owner = self->owner ? &self->owner->base : NULL;
                if (var3->entityHit->vtable->attackEntityFrom(var3->entityHit, owner, 4)) {
                    World_playSoundAtEntity(
                        self->base.worldObj, &self->base, "random.drr", 1.0F,
                        1.2F / (JavaRandom_nextFloat(&self->base.rand) * 0.2F + 0.9F));
                    Entity_setEntityDead(&self->base);
                } else {
                    self->base.motionX *= (double)-0.1F;
                    self->base.motionY *= (double)-0.1F;
                    self->base.motionZ *= (double)-0.1F;
                    self->base.rotationYaw += 180.0F;
                    self->base.prevRotationYaw += 180.0F;
                    self->ticksInAir = 0;
                }
            } else {
                self->xTile = var3->blockX;
                self->yTile = var3->blockY;
                self->zTile = var3->blockZ;
                self->inTile =
                    World_getBlockId(self->base.worldObj, self->xTile, self->yTile, self->zTile);
                self->field_28019_h = World_getBlockMetadata(self->base.worldObj, self->xTile,
                                                             self->yTile, self->zTile);
                self->base.motionX = (double)((float)(var3->hitVec->xCoord - self->base.posX));
                self->base.motionY = (double)((float)(var3->hitVec->yCoord - self->base.posY));
                self->base.motionZ = (double)((float)(var3->hitVec->zCoord - self->base.posZ));
                var19 = MathHelper_sqrt_double(self->base.motionX * self->base.motionX +
                                               self->base.motionY * self->base.motionY +
                                               self->base.motionZ * self->base.motionZ);
                self->base.posX -= self->base.motionX / (double)var19 * (double)0.05F;
                self->base.posY -= self->base.motionY / (double)var19 * (double)0.05F;
                self->base.posZ -= self->base.motionZ / (double)var19 * (double)0.05F;
                World_playSoundAtEntity(self->base.worldObj, &self->base, "random.drr", 1.0F,
                                        1.2F /
                                            (JavaRandom_nextFloat(&self->base.rand) * 0.2F + 0.9F));
                self->inGround = 1;
                self->arrowShake = 7;
            }
        }

        self->base.posX += self->base.motionX;
        self->base.posY += self->base.motionY;
        self->base.posZ += self->base.motionZ;
        var19 = MathHelper_sqrt_double(self->base.motionX * self->base.motionX +
                                       self->base.motionZ * self->base.motionZ);
        self->base.rotationYaw =
            (float)(atan2(self->base.motionX, self->base.motionZ) * 180.0 / (double)((float)M_PI));

        for (self->base.rotationPitch =
                 (float)(atan2(self->base.motionY, (double)var19) * 180.0 / (double)((float)M_PI));
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

        self->base.rotationPitch = self->base.prevRotationPitch +
                                   (self->base.rotationPitch - self->base.prevRotationPitch) * 0.2F;
        self->base.rotationYaw = self->base.prevRotationYaw +
                                 (self->base.rotationYaw - self->base.prevRotationYaw) * 0.2F;

        float var20 = 0.99F;
        var10 = 0.03F;
        if (Entity_isInWater(&self->base)) {
            for (int var21 = 0; var21 < 4; ++var21) {
                float var22 = 0.25F;
                World_spawnParticle(self->base.worldObj, "bubble",
                                    self->base.posX - self->base.motionX * (double)var22,
                                    self->base.posY - self->base.motionY * (double)var22,
                                    self->base.posZ - self->base.motionZ * (double)var22,
                                    self->base.motionX, self->base.motionY, self->base.motionZ);
            }
            var20 = 0.8F;
        }

        self->base.motionX *= (double)var20;
        self->base.motionY *= (double)var20;
        self->base.motionZ *= (double)var20;
        self->base.motionY -= (double)var10;
        self->base.vtable->setPosition(&self->base, self->base.posX, self->base.posY,
                                       self->base.posZ);
    }
}

void EntityArrow_writeEntityToNBT(EntityArrow *self, NBTTagCompound *var1) {
    NBTTagCompound_setShort(var1, "xTile", (short)self->xTile);
    NBTTagCompound_setShort(var1, "yTile", (short)self->yTile);
    NBTTagCompound_setShort(var1, "zTile", (short)self->zTile);
    NBTTagCompound_setByte(var1, "inTile", (int8_t)self->inTile);
    NBTTagCompound_setByte(var1, "inData", (int8_t)self->field_28019_h);
    NBTTagCompound_setByte(var1, "shake", (int8_t)self->arrowShake);
    NBTTagCompound_setByte(var1, "inGround", (int8_t)(self->inGround ? 1 : 0));
    NBTTagCompound_setBoolean(var1, "player", self->doesArrowBelongToPlayer);
}

void EntityArrow_readEntityFromNBT(EntityArrow *self, NBTTagCompound *var1) {
    self->xTile = NBTTagCompound_getShort(var1, "xTile");
    self->yTile = NBTTagCompound_getShort(var1, "yTile");
    self->zTile = NBTTagCompound_getShort(var1, "zTile");
    self->inTile = NBTTagCompound_getByte(var1, "inTile") & 255;
    self->field_28019_h = NBTTagCompound_getByte(var1, "inData") & 255;
    self->arrowShake = NBTTagCompound_getByte(var1, "shake") & 255;
    self->inGround = NBTTagCompound_getByte(var1, "inGround") == 1;
    self->doesArrowBelongToPlayer = NBTTagCompound_getBoolean(var1, "player");
}

void EntityArrow_onCollideWithPlayer(EntityArrow *self, EntityPlayer *var1) {
    if (!self->base.worldObj->multiplayerWorld) {
        if (self->inGround && self->doesArrowBelongToPlayer && self->arrowShake <= 0) {
            ItemStack arrowStack = ItemStack_fromItemID(Item_arrow->shiftedIndex, 1, 0);
            if (InventoryPlayer_addItemStackToInventory(&var1->inventory, &arrowStack)) {
                World_playSoundAtEntity(self->base.worldObj, &self->base, "random.pop", 0.2F,
                                        ((JavaRandom_nextFloat(&self->base.rand) -
                                          JavaRandom_nextFloat(&self->base.rand)) *
                                             0.7F +
                                         1.0F) *
                                            2.0F);
                EntityPlayer_onItemPickup(var1, &self->base, 1);
                Entity_setEntityDead(&self->base);
            }
        }
    }
}

float EntityArrow_getShadowSize(EntityArrow *self) {
    (void)self;
    return 0.0F;
}
