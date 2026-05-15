#ifndef ENTITY_FISH_H
#define ENTITY_FISH_H

#include "Entity.h"
#include "EntityPlayer.h"

typedef struct EntityFish {
    Entity base;

    int xTile;
    int yTile;
    int zTile;
    int inTile;

    int inGround;

    int shake;

    EntityPlayer *angler;

    int ticksInGround;
    int ticksInAir;
    int ticksCatchable;

    Entity *bobber;

    int field_6388_l;

    double field_6387_m;
    double field_6386_n;
    double field_6385_o;
    double field_6384_p;
    double field_6383_q;

    double velocityX;
    double velocityY;
    double velocityZ;
} EntityFish;

void EntityFish_construct(EntityFish *self, World *var1);
void EntityFish_construct2(EntityFish *self, World *var1, double var2, double var4, double var6);
void EntityFish_construct3(EntityFish *self, World *var1, EntityPlayer *var2);
EntityFish *EntityFish_create(World *var1);
EntityFish *EntityFish_create2(World *var1, double var2, double var4, double var6);
EntityFish *EntityFish_create3(World *var1, EntityPlayer *var2);

void EntityFish_entityInit(Entity *self);
int EntityFish_isInRangeToRenderDist(Entity *self, double var1);
void EntityFish_onUpdate(Entity *self);
void EntityFish_writeEntityToNBT(Entity *self, NBTTagCompound *var1);
void EntityFish_readEntityFromNBT(Entity *self, NBTTagCompound *var1);
float EntityFish_getShadowSize(Entity *self);

void EntityFish_func_4042_a(EntityFish *self, double var1, double var3, double var5, float var7,
                            float var8);
void EntityFish_setPositionAndRotation2(Entity *self, double var1, double var3, double var5,
                                        float var7, float var8, int var9);
void EntityFish_setVelocity(Entity *self, double var1, double var3, double var5);
int EntityFish_catchFish(EntityFish *self);

#endif
