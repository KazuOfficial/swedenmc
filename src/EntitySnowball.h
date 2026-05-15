#ifndef ENTITY_SNOWBALL_H
#define ENTITY_SNOWBALL_H

#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"

typedef struct EntitySnowball {
    Entity base;

    int xTileSnowball;

    int yTileSnowball;

    int zTileSnowball;

    int inTileSnowball;

    int inGroundSnowball;

    int shakeSnowball;

    EntityLiving *thrower;

    int ticksInGroundSnowball;

    int ticksInAirSnowball;
} EntitySnowball;

void EntitySnowball_construct1(EntitySnowball *self, World *var1);

void EntitySnowball_construct2(EntitySnowball *self, World *var1, EntityLiving *var2);

void EntitySnowball_construct3(EntitySnowball *self, World *var1, double var2, double var4,
                               double var6);

void EntitySnowball_entityInit(Entity *self);

int EntitySnowball_isInRangeToRenderDist(Entity *self, double var1);

void EntitySnowball_setSnowballHeading(EntitySnowball *self, double var1, double var3, double var5,
                                       float var7, float var8);

void EntitySnowball_setVelocity(Entity *self, double var1, double var3, double var5);

void EntitySnowball_onUpdate(Entity *self);

void EntitySnowball_writeEntityToNBT(Entity *self, NBTTagCompound *var1);

void EntitySnowball_readEntityFromNBT(Entity *self, NBTTagCompound *var1);

void EntitySnowball_onCollideWithPlayer(Entity *self, EntityPlayer *var1);

float EntitySnowball_getShadowSize(Entity *self);

#endif
