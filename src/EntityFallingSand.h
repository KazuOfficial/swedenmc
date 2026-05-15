#ifndef ENTITY_FALLING_SAND_H
#define ENTITY_FALLING_SAND_H

#include "Entity.h"
#include "NBTTagCompound.h"

typedef struct EntityFallingSand {
    Entity base;

    int blockID;

    int fallTime;
} EntityFallingSand;

void EntityFallingSand_construct1(EntityFallingSand *self, World *var1);

void EntityFallingSand_construct2(EntityFallingSand *self, World *var1, double var2, double var4,
                                  double var6, int var8);

int EntityFallingSand_canTriggerWalking(Entity *self);

void EntityFallingSand_entityInit(Entity *self);

int EntityFallingSand_canBeCollidedWith(Entity *self);

void EntityFallingSand_onUpdate(Entity *self);

void EntityFallingSand_writeEntityToNBT(Entity *self, NBTTagCompound *var1);

void EntityFallingSand_readEntityFromNBT(Entity *self, NBTTagCompound *var1);

float EntityFallingSand_getShadowSize(Entity *self);

World *EntityFallingSand_getWorld(EntityFallingSand *self);

#endif
