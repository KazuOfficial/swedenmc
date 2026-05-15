#ifndef ENTITY_TNT_PRIMED_H
#define ENTITY_TNT_PRIMED_H

#include "Entity.h"
#include "NBTTagCompound.h"

typedef struct EntityTNTPrimed {
    Entity base;

    int fuse;
} EntityTNTPrimed;

void EntityTNTPrimed_construct(EntityTNTPrimed *self, World *var1);

void EntityTNTPrimed_construct2(EntityTNTPrimed *self, World *var1, double var2, double var4,
                                double var6);

EntityTNTPrimed *EntityTNTPrimed_create(World *var1);
EntityTNTPrimed *EntityTNTPrimed_create2(World *var1, double var2, double var4, double var6);

void EntityTNTPrimed_entityInit(Entity *self);
int EntityTNTPrimed_canTriggerWalking(Entity *self);
int EntityTNTPrimed_canBeCollidedWith(Entity *self);
void EntityTNTPrimed_onUpdate(Entity *self);
void EntityTNTPrimed_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
void EntityTNTPrimed_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
float EntityTNTPrimed_getShadowSize(Entity *self);

#endif
