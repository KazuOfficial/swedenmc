#ifndef ENTITY_SLIME_H
#define ENTITY_SLIME_H

#include "EntityLiving.h"

typedef struct EntitySlime {
    EntityLiving base;

    float field_768_a;

    float field_767_b;

    int slimeJumpDelay;
} EntitySlime;

void EntitySlime_construct(EntitySlime *self, World *var1);
EntitySlime *EntitySlime_create(World *var1);

void EntitySlime_initVtable(EntityLivingVtable *vt);
extern EntityLivingVtable EntitySlime_defaultVtable;

void EntitySlime_setSlimeSize(EntitySlime *self, int var1);
int EntitySlime_getSlimeSize(EntitySlime *self);

void EntitySlime_entityInit(Entity *self);
void EntitySlime_onUpdate(Entity *self);
void EntitySlime_updatePlayerActionState(EntityLiving *self);
void EntitySlime_setEntityDead(Entity *self);
void EntitySlime_onCollideWithPlayer(Entity *self, struct EntityPlayer *var1);
void EntitySlime_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
void EntitySlime_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
const char *EntitySlime_getHurtSound(EntityLiving *self);
const char *EntitySlime_getDeathSound(EntityLiving *self);
int EntitySlime_getDropItemId(EntityLiving *self);
int EntitySlime_getCanSpawnHere(EntityLiving *self);
float EntitySlime_getSoundVolume(EntityLiving *self);

#endif
