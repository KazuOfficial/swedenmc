#ifndef ENTITY_EGG_H
#define ENTITY_EGG_H

#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"

typedef struct EntityEgg {
    Entity base;

    int field_20056_b;

    int field_20055_c;

    int field_20054_d;

    int field_20053_e;

    int field_20052_f;

    int field_20057_a;

    EntityLiving *field_20051_g;

    int field_20050_h;

    int field_20049_i;
} EntityEgg;

void EntityEgg_construct1(EntityEgg *self, World *var1);

void EntityEgg_construct2(EntityEgg *self, World *var1, EntityLiving *var2);

void EntityEgg_construct3(EntityEgg *self, World *var1, double var2, double var4, double var6);

void EntityEgg_entityInit(Entity *self);

int EntityEgg_isInRangeToRenderDist(Entity *self, double var1);

void EntityEgg_setEggHeading(EntityEgg *self, double var1, double var3, double var5, float var7,
                             float var8);

void EntityEgg_setVelocity(EntityEgg *self, double var1, double var3, double var5);

void EntityEgg_onUpdate(Entity *self);

void EntityEgg_writeEntityToNBT(Entity *self, NBTTagCompound *var1);

void EntityEgg_readEntityFromNBT(Entity *self, NBTTagCompound *var1);

void EntityEgg_onCollideWithPlayer(Entity *self, EntityPlayer *var1);

float EntityEgg_getShadowSize(Entity *self);

#endif
