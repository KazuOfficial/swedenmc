#ifndef ENTITY_FIREBALL_H
#define ENTITY_FIREBALL_H

#include "Entity.h"
#include "EntityLiving.h"
#include "NBTTagCompound.h"

typedef struct EntityFireball {
    Entity base;

    int field_9402_e;

    int field_9401_f;

    int field_9400_g;

    int field_9399_h;

    int field_9398_i;

    int field_9406_a;

    EntityLiving *field_9397_j;

    int field_9396_k;

    int field_9395_l;

    double field_9405_b;

    double field_9404_c;

    double field_9403_d;
} EntityFireball;

void EntityFireball_construct1(EntityFireball *self, World *var1);

void EntityFireball_construct2(EntityFireball *self, World *var1, double var2, double var4,
                               double var6, double var8, double var10, double var12);

void EntityFireball_construct3(EntityFireball *self, World *var1, EntityLiving *var2, double var3,
                               double var5, double var7);
EntityFireball *EntityFireball_create3(World *var1, EntityLiving *var2, double var3, double var5,
                                       double var7);

void EntityFireball_entityInit(Entity *self);

int EntityFireball_isInRangeToRenderDist(Entity *self, double var1);

void EntityFireball_onUpdate(Entity *self);

void EntityFireball_writeEntityToNBT(Entity *self, NBTTagCompound *var1);

void EntityFireball_readEntityFromNBT(Entity *self, NBTTagCompound *var1);

int EntityFireball_canBeCollidedWith(Entity *self);

float EntityFireball_getCollisionBorderSize(EntityFireball *self);

int EntityFireball_attackEntityFrom(Entity *self, Entity *var1, int var2);

float EntityFireball_getShadowSize(Entity *self);

#endif
