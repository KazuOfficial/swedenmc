#include "EntityEgg.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EntityChicken.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "Item.h"
#include "MathHelper.h"
#include "Vec3D.h"
#include "AxisAlignedBB.h"
#include "MovingObjectPosition.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <math.h>

static EntityVtable EntityEgg_vtable;
static int EntityEgg_vtable_init = 0;

static void ensure_vtable(void) {
    if (!EntityEgg_vtable_init) {
        Entity_initVtable(&EntityEgg_vtable);
        EntityEgg_vtable.entityInit = EntityEgg_entityInit;
        EntityEgg_vtable.isInRangeToRenderDist = EntityEgg_isInRangeToRenderDist;
        EntityEgg_vtable.onUpdate = EntityEgg_onUpdate;
        EntityEgg_vtable.writeEntityToNBT = EntityEgg_writeEntityToNBT;
        EntityEgg_vtable.readEntityFromNBT = EntityEgg_readEntityFromNBT;
        EntityEgg_vtable.onCollideWithPlayer = EntityEgg_onCollideWithPlayer;
        EntityEgg_vtable.getShadowSize = EntityEgg_getShadowSize;
        EntityEgg_vtable_init = 1;
    }
}

void EntityEgg_construct1(EntityEgg *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityEgg_vtable;

    self->field_20056_b = -1;
    self->field_20055_c = -1;
    self->field_20054_d = -1;
    self->field_20053_e = 0;
    self->field_20052_f = 0;
    self->field_20057_a = 0;
    self->field_20051_g = NULL;
    self->field_20050_h = 0;
    self->field_20049_i = 0;
    Entity_setSize(&self->base, 0.25F, 0.25F);
    self->base.isEgg = 1;
}

void EntityEgg_construct2(EntityEgg *self, World *var1, EntityLiving *var2) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityEgg_vtable;

    self->field_20056_b = -1;
    self->field_20055_c = -1;
    self->field_20054_d = -1;
    self->field_20053_e = 0;
    self->field_20052_f = 0;
    self->field_20057_a = 0;
    self->field_20049_i = 0;

    self->field_20051_g = var2;
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
    EntityEgg_setEggHeading(self, self->base.motionX, self->base.motionY, self->base.motionZ, 1.5F,
                            1.0F);
    self->base.isEgg = 1;
}

void EntityEgg_construct3(EntityEgg *self, World *var1, double var2, double var4, double var6) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityEgg_vtable;

    self->field_20056_b = -1;
    self->field_20055_c = -1;
    self->field_20054_d = -1;
    self->field_20053_e = 0;
    self->field_20052_f = 0;
    self->field_20057_a = 0;
    self->field_20051_g = NULL;
    self->field_20049_i = 0;

    self->field_20050_h = 0;
    Entity_setSize(&self->base, 0.25F, 0.25F);
    self->base.vtable->setPosition(&self->base, var2, var4, var6);
    self->base.yOffset = 0.0F;
    self->base.isEgg = 1;
}

void EntityEgg_entityInit(Entity *self) { (void)self; }

int EntityEgg_isInRangeToRenderDist(Entity *selfE, double var1) {
    double var3 = AxisAlignedBB_getAverageEdgeLength(&selfE->boundingBox) * 4.0;
    var3 *= 64.0;
    return var1 < var3 * var3;
}

void EntityEgg_setEggHeading(EntityEgg *self, double var1, double var3, double var5, float var7,
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
    self->field_20050_h = 0;
}

void EntityEgg_setVelocity(EntityEgg *self, double var1, double var3, double var5) {
    self->base.motionX = var1;
    self->base.motionY = var3;
    self->base.motionZ = var5;
    if (self->base.prevRotationPitch == 0.0F && self->base.prevRotationYaw == 0.0F) {
        float var7 = MathHelper_sqrt_double(var1 * var1 + var5 * var5);
        self->base.prevRotationYaw = self->base.rotationYaw =
            (float)(atan2(var1, var5) * 180.0 / (double)((float)M_PI));
        self->base.prevRotationPitch = self->base.rotationPitch =
            (float)(atan2(var3, (double)var7) * 180.0 / (double)((float)M_PI));
    }
}

void EntityEgg_onUpdate(Entity *selfE) {
    EntityEgg *self = (EntityEgg *)selfE;

    selfE->lastTickPosX = selfE->posX;
    selfE->lastTickPosY = selfE->posY;
    selfE->lastTickPosZ = selfE->posZ;
    Entity_defaultVtable.onUpdate(selfE);
    if (self->field_20057_a > 0) {
        --self->field_20057_a;
    }

    if (self->field_20052_f) {
        int var1 = World_getBlockId(selfE->worldObj, self->field_20056_b, self->field_20055_c,
                                    self->field_20054_d);
        if (var1 == self->field_20053_e) {
            ++self->field_20050_h;
            if (self->field_20050_h == 1200) {
                Entity_setEntityDead(selfE);
            }
            return;
        }

        self->field_20052_f = 0;
        selfE->motionX *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        selfE->motionY *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        selfE->motionZ *= (double)(JavaRandom_nextFloat(&selfE->rand) * 0.2F);
        self->field_20050_h = 0;
        self->field_20049_i = 0;
    } else {
        ++self->field_20049_i;
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
                (var9 != (Entity *)self->field_20051_g || self->field_20049_i >= 5)) {
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
            var3->entityHit->vtable->attackEntityFrom(var3->entityHit,
                                                      (Entity *)self->field_20051_g, 0);
        }

        if (!selfE->worldObj->isRemote && JavaRandom_nextInt(&selfE->rand, 8) == 0) {
            int8_t var16 = 1;
            if (JavaRandom_nextInt(&selfE->rand, 32) == 0) {
                var16 = 4;
            }

            for (int var17 = 0; var17 < var16; ++var17) {
                EntityChicken *var21 = EntityChicken_create(selfE->worldObj);
                var21->base.base.base.base.vtable->setLocationAndAngles(
                    &var21->base.base.base.base, selfE->posX, selfE->posY, selfE->posZ,
                    selfE->rotationYaw, 0.0F);
                World_entityJoinedWorld(selfE->worldObj, (Entity *)var21);
            }
        }

        for (int var18 = 0; var18 < 8; ++var18) {
            World_spawnParticle(selfE->worldObj, "snowballpoof", selfE->posX, selfE->posY,
                                selfE->posZ, 0.0, 0.0, 0.0);
        }

        Entity_setEntityDead(selfE);
    }

    selfE->posX += selfE->motionX;
    selfE->posY += selfE->motionY;
    selfE->posZ += selfE->motionZ;
    float var20 =
        MathHelper_sqrt_double(selfE->motionX * selfE->motionX + selfE->motionZ * selfE->motionZ);
    selfE->rotationYaw =
        (float)(atan2(selfE->motionX, selfE->motionZ) * 180.0 / (double)((float)M_PI));

    for (selfE->rotationPitch =
             (float)(atan2(selfE->motionY, (double)var20) * 180.0 / (double)((float)M_PI));
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
    float var19 = 0.99F;
    float var22 = 0.03F;
    if (Entity_isInWater(selfE)) {
        for (int var7 = 0; var7 < 4; ++var7) {
            float var23 = 0.25F;
            World_spawnParticle(selfE->worldObj, "bubble",
                                selfE->posX - selfE->motionX * (double)var23,
                                selfE->posY - selfE->motionY * (double)var23,
                                selfE->posZ - selfE->motionZ * (double)var23, selfE->motionX,
                                selfE->motionY, selfE->motionZ);
        }
        var19 = 0.8F;
    }

    selfE->motionX *= (double)var19;
    selfE->motionY *= (double)var19;
    selfE->motionZ *= (double)var19;
    selfE->motionY -= (double)var22;
    selfE->vtable->setPosition(selfE, selfE->posX, selfE->posY, selfE->posZ);
}

void EntityEgg_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityEgg *self = (EntityEgg *)selfE;
    NBTTagCompound_setShort(var1, "xTile", (int16_t)self->field_20056_b);
    NBTTagCompound_setShort(var1, "yTile", (int16_t)self->field_20055_c);
    NBTTagCompound_setShort(var1, "zTile", (int16_t)self->field_20054_d);
    NBTTagCompound_setByte(var1, "inTile", (int8_t)self->field_20053_e);
    NBTTagCompound_setByte(var1, "shake", (int8_t)self->field_20057_a);
    NBTTagCompound_setByte(var1, "inGround", (int8_t)(self->field_20052_f ? 1 : 0));
}

void EntityEgg_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityEgg *self = (EntityEgg *)selfE;
    self->field_20056_b = NBTTagCompound_getShort(var1, "xTile");
    self->field_20055_c = NBTTagCompound_getShort(var1, "yTile");
    self->field_20054_d = NBTTagCompound_getShort(var1, "zTile");
    self->field_20053_e = NBTTagCompound_getByte(var1, "inTile") & 255;
    self->field_20057_a = NBTTagCompound_getByte(var1, "shake") & 255;
    self->field_20052_f = NBTTagCompound_getByte(var1, "inGround") == 1;
}

void EntityEgg_onCollideWithPlayer(Entity *selfE, EntityPlayer *var1) {
    EntityEgg *self = (EntityEgg *)selfE;
    if (self->field_20052_f && (EntityLiving *)var1 == self->field_20051_g &&
        self->field_20057_a <= 0) {
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

float EntityEgg_getShadowSize(Entity *self) {
    (void)self;
    return 0.0F;
}
