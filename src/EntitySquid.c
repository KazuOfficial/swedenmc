#include "EntitySquid.h"
#include "World.h"
#include "Material.h"
#include "MathHelper.h"
#include "JavaRandom.h"
#include "Item.h"
#include "ItemStack.h"
#include "AxisAlignedBB.h"
#include <math.h>
#include <stdlib.h>

static EntityCreatureVtable EntitySquid_vtable;
static int EntitySquid_vtable_init = 0;

static const char *squid_getEntityString(Entity *self) {
    (void)self;
    return "Squid";
}

static void ensure_vtable(void) {
    if (EntitySquid_vtable_init)
        return;
    EntityWaterMob_initVtable(&EntitySquid_vtable);
    EntitySquid_vtable.base.base.getEntityString = squid_getEntityString;
    EntitySquid_vtable.base.base.writeEntityToNBT = EntitySquid_writeEntityToNBT;
    EntitySquid_vtable.base.base.readEntityFromNBT = EntitySquid_readEntityFromNBT;
    EntitySquid_vtable.base.getLivingSound = EntitySquid_getLivingSound;
    EntitySquid_vtable.base.getHurtSound = EntitySquid_getHurtSound;
    EntitySquid_vtable.base.getDeathSound = EntitySquid_getDeathSound;
    EntitySquid_vtable.base.getSoundVolume = EntitySquid_getSoundVolume;
    EntitySquid_vtable.base.getDropItemId = EntitySquid_getDropItemId;
    EntitySquid_vtable.base.dropFewItems = EntitySquid_dropFewItems;
    EntitySquid_vtable.base.base.interact = EntitySquid_interact;
    EntitySquid_vtable.base.base.isInWater = EntitySquid_isInWater;
    EntitySquid_vtable.base.onLivingUpdate = EntitySquid_onLivingUpdate;
    EntitySquid_vtable.base.moveEntityWithHeading = EntitySquid_moveEntityWithHeading;
    EntitySquid_vtable.base.updatePlayerActionState = EntitySquid_updatePlayerActionState;
    EntitySquid_vtable_init = 1;
}

void EntitySquid_construct(EntitySquid *self, World *var1) {
    ensure_vtable();
    EntityWaterMob_construct(&self->base, var1);
    self->base.base.base.base.vtable = (const EntityVtable *)&EntitySquid_vtable;

    self->field_21089_a = 0.0F;
    self->field_21088_b = 0.0F;
    self->field_21087_c = 0.0F;
    self->field_21086_f = 0.0F;
    self->field_21085_g = 0.0F;
    self->field_21084_h = 0.0F;
    self->field_21083_i = 0.0F;
    self->field_21082_j = 0.0F;
    self->randomMotionSpeed = 0.0F;
    self->field_21080_l = 0.0F;
    self->field_21079_m = 0.0F;
    self->randomMotionVecX = 0.0F;
    self->randomMotionVecY = 0.0F;
    self->randomMotionVecZ = 0.0F;

    self->base.base.base.texture = "/mob/squid.png";
    Entity_setSize(&self->base.base.base.base, 0.95F, 0.95F);
    self->field_21080_l =
        1.0F / (JavaRandom_nextFloat(&self->base.base.base.base.rand) + 1.0F) * 0.2F;
    self->base.base.base.base.isSquid = 1;
}

EntitySquid *EntitySquid_create(World *var1) {
    EntitySquid *self = (EntitySquid *)calloc(1, sizeof(EntitySquid));
    EntitySquid_construct(self, var1);
    return self;
}

void EntitySquid_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityWaterMob_writeEntityToNBT(selfE, var1);
}

void EntitySquid_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityWaterMob_readEntityFromNBT(selfE, var1);
}

const char *EntitySquid_getLivingSound(EntityLiving *self) {
    (void)self;
    return NULL;
}

const char *EntitySquid_getHurtSound(EntityLiving *self) {
    (void)self;
    return NULL;
}

const char *EntitySquid_getDeathSound(EntityLiving *self) {
    (void)self;
    return NULL;
}

float EntitySquid_getSoundVolume(EntityLiving *self) {
    (void)self;
    return 0.4F;
}

int EntitySquid_getDropItemId(EntityLiving *self) {
    (void)self;
    return 0;
}

void EntitySquid_dropFewItems(EntityLiving *self) {
    Entity *selfE = &self->base;
    int var1 = JavaRandom_nextInt(&selfE->rand, 3) + 1;
    for (int var2 = 0; var2 < var1; ++var2) {
        Entity_entityDropItem(selfE, ItemStack_fromItemID(Item_dyePowder->shiftedIndex, 1, 0),
                              0.0F);
    }
}

int EntitySquid_interact(Entity *self, EntityPlayer *player) {
    (void)self;
    (void)player;
    return 0;
}

int EntitySquid_isInWater(Entity *self) {
    return World_handleMaterialAcceleration(
        self->worldObj, AxisAlignedBB_expand(&self->boundingBox, 0.0, (double)-0.6F, 0.0),
        Material_water, self);
}

void EntitySquid_onLivingUpdate(EntityLiving *self) {
    Entity *selfE = &self->base;
    EntitySquid *squid = (EntitySquid *)self;

    EntityLiving_onLivingUpdateBase(self);

    squid->field_21088_b = squid->field_21089_a;
    squid->field_21086_f = squid->field_21087_c;
    squid->field_21084_h = squid->field_21085_g;
    squid->field_21082_j = squid->field_21083_i;
    squid->field_21085_g += squid->field_21080_l;
    if (squid->field_21085_g > (float)M_PI * 2.0F) {
        squid->field_21085_g -= (float)M_PI * 2.0F;
        if (JavaRandom_nextInt(&selfE->rand, 10) == 0) {
            squid->field_21080_l = 1.0F / (JavaRandom_nextFloat(&selfE->rand) + 1.0F) * 0.2F;
        }
    }

    if (selfE->vtable->isInWater(selfE)) {
        float var1;
        if (squid->field_21085_g < (float)M_PI) {
            var1 = squid->field_21085_g / (float)M_PI;
            squid->field_21083_i = MathHelper_sin(var1 * var1 * (float)M_PI) * (float)M_PI * 0.25F;
            if ((double)var1 > 0.75) {
                squid->randomMotionSpeed = 1.0F;
                squid->field_21079_m = 1.0F;
            } else {
                squid->field_21079_m *= 0.8F;
            }
        } else {
            squid->field_21083_i = 0.0F;
            squid->randomMotionSpeed *= 0.9F;
            squid->field_21079_m *= 0.99F;
        }

        if (!self->isMultiplayerEntity) {
            selfE->motionX = (double)(squid->randomMotionVecX * squid->randomMotionSpeed);
            selfE->motionY = (double)(squid->randomMotionVecY * squid->randomMotionSpeed);
            selfE->motionZ = (double)(squid->randomMotionVecZ * squid->randomMotionSpeed);
        }

        var1 = MathHelper_sqrt_double(selfE->motionX * selfE->motionX +
                                      selfE->motionZ * selfE->motionZ);
        self->renderYawOffset +=
            (-(float)atan2(selfE->motionX, selfE->motionZ) * 180.0F / (float)M_PI -
             self->renderYawOffset) *
            0.1F;
        self->base.rotationYaw = self->renderYawOffset;
        squid->field_21087_c += (float)M_PI * squid->field_21079_m * 1.5F;
        squid->field_21089_a +=
            (-(float)atan2((double)var1, selfE->motionY) * 180.0F / (float)M_PI -
             squid->field_21089_a) *
            0.1F;
    } else {
        squid->field_21083_i =
            MathHelper_abs(MathHelper_sin(squid->field_21085_g)) * (float)M_PI * 0.25F;
        if (!self->isMultiplayerEntity) {
            selfE->motionX = 0.0;
            selfE->motionY -= 0.08;
            selfE->motionY *= (double)0.98F;
            selfE->motionZ = 0.0;
        }

        squid->field_21089_a =
            (float)((double)squid->field_21089_a + (double)(-90.0F - squid->field_21089_a) * 0.02);
    }
}

void EntitySquid_moveEntityWithHeading(EntityLiving *self, float var1, float var2) {
    Entity *selfE = &self->base;
    (void)var1;
    (void)var2;
    selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);
}

void EntitySquid_updatePlayerActionState(EntityLiving *self) {
    Entity *selfE = &self->base;
    EntitySquid *squid = (EntitySquid *)self;
    if (JavaRandom_nextInt(&selfE->rand, 50) == 0 || !selfE->inWater ||
        (squid->randomMotionVecX == 0.0F && squid->randomMotionVecY == 0.0F &&
         squid->randomMotionVecZ == 0.0F)) {
        float var1 = JavaRandom_nextFloat(&selfE->rand) * (float)M_PI * 2.0F;
        squid->randomMotionVecX = MathHelper_cos(var1) * 0.2F;
        squid->randomMotionVecY = -0.1F + JavaRandom_nextFloat(&selfE->rand) * 0.2F;
        squid->randomMotionVecZ = MathHelper_sin(var1) * 0.2F;
    }

    EntityLiving_func_27021_X(self);
}
