#ifndef ENTITY_ARROW_H
#define ENTITY_ARROW_H

#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"

typedef struct EntityArrow {
    Entity base;

    int xTile;

    int yTile;

    int zTile;

    int inTile;

    int field_28019_h;

    int inGround;

    int doesArrowBelongToPlayer;

    int arrowShake;

    EntityLiving *owner;

    int ticksInGround;

    int ticksInAir;
} EntityArrow;

void EntityArrow_construct1(EntityArrow *self, World *var1);
EntityArrow *EntityArrow_create1(World *var1);

void EntityArrow_construct2(EntityArrow *self, World *var1, double var2, double var4, double var6);
EntityArrow *EntityArrow_create2(World *var1, double var2, double var4, double var6);

void EntityArrow_construct3(EntityArrow *self, World *var1, EntityLiving *var2);
EntityArrow *EntityArrow_create3(World *var1, EntityLiving *var2);

void EntityArrow_entityInit(EntityArrow *self);

void EntityArrow_setArrowHeading(EntityArrow *self, double var1, double var3, double var5,
                                 float var7, float var8);

void EntityArrow_setVelocity(EntityArrow *self, double var1, double var3, double var5);

void EntityArrow_onUpdate(EntityArrow *self);

void EntityArrow_writeEntityToNBT(EntityArrow *self, NBTTagCompound *var1);

void EntityArrow_readEntityFromNBT(EntityArrow *self, NBTTagCompound *var1);

void EntityArrow_onCollideWithPlayer(EntityArrow *self, EntityPlayer *var1);

float EntityArrow_getShadowSize(EntityArrow *self);

#endif
