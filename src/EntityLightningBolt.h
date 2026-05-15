#ifndef ENTITY_LIGHTNING_BOLT_H
#define ENTITY_LIGHTNING_BOLT_H

#include "EntityWeatherEffect.h"
#include "NBTTagCompound.h"

typedef struct EntityLightningBolt {
    EntityWeatherEffect base;

    int field_27028_b;

    int64_t field_27029_a;

    int field_27030_c;
} EntityLightningBolt;

void EntityLightningBolt_construct(EntityLightningBolt *self, World *var1, double var2, double var4,
                                   double var6);
EntityLightningBolt *EntityLightningBolt_create(World *var1, double var2, double var4, double var6);

void EntityLightningBolt_onUpdate(Entity *self);
void EntityLightningBolt_entityInit(Entity *self);
void EntityLightningBolt_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
void EntityLightningBolt_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
int EntityLightningBolt_isInRangeToRenderVec3D(Entity *self, Vec3D *var1);

#endif
