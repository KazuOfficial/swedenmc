#ifndef ENTITY_WATER_MOB_H
#define ENTITY_WATER_MOB_H

#include "EntityCreature.h"
#include "NBTTagCompound.h"

typedef struct EntityWaterMob {
    EntityCreature base;
} EntityWaterMob;

void EntityWaterMob_construct(EntityWaterMob *self, World *var1);

void EntityWaterMob_initVtable(EntityCreatureVtable *vt);

int EntityWaterMob_canBreatheUnderwater(EntityLiving *self);

void EntityWaterMob_writeEntityToNBT(Entity *self, NBTTagCompound *var1);

void EntityWaterMob_readEntityFromNBT(Entity *self, NBTTagCompound *var1);

int EntityWaterMob_getCanSpawnHere(EntityLiving *self);

int EntityWaterMob_getTalkInterval(EntityLiving *self);

#endif
