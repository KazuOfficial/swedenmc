#ifndef ENTITY_SQUID_H
#define ENTITY_SQUID_H

#include "EntityWaterMob.h"
#include "NBTTagCompound.h"

typedef struct EntitySquid {
    EntityWaterMob base;

    float field_21089_a;

    float field_21088_b;

    float field_21087_c;

    float field_21086_f;

    float field_21085_g;

    float field_21084_h;

    float field_21083_i;

    float field_21082_j;

    float randomMotionSpeed;

    float field_21080_l;

    float field_21079_m;

    float randomMotionVecX;

    float randomMotionVecY;

    float randomMotionVecZ;
} EntitySquid;

void EntitySquid_construct(EntitySquid *self, World *var1);
EntitySquid *EntitySquid_create(World *var1);

void EntitySquid_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
void EntitySquid_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
const char *EntitySquid_getLivingSound(EntityLiving *self);
const char *EntitySquid_getHurtSound(EntityLiving *self);
const char *EntitySquid_getDeathSound(EntityLiving *self);
float EntitySquid_getSoundVolume(EntityLiving *self);
int EntitySquid_getDropItemId(EntityLiving *self);
void EntitySquid_dropFewItems(EntityLiving *self);
int EntitySquid_interact(Entity *self, EntityPlayer *player);
int EntitySquid_isInWater(Entity *self);
void EntitySquid_onLivingUpdate(EntityLiving *self);
void EntitySquid_moveEntityWithHeading(EntityLiving *self, float var1, float var2);
void EntitySquid_updatePlayerActionState(EntityLiving *self);

#endif
