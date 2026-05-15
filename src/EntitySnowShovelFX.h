#ifndef ENTITY_SNOW_SHOVEL_FX_H
#define ENTITY_SNOW_SHOVEL_FX_H

#include "EntityFX.h"

typedef struct EntitySnowShovelFX {
    EntityFX base;

    float field_27017_a;
} EntitySnowShovelFX;

void EntitySnowShovelFX_construct1(EntitySnowShovelFX *self, World *var1, double var2, double var4,
                                   double var6, double var8, double var10, double var12);

void EntitySnowShovelFX_construct2(EntitySnowShovelFX *self, World *var1, double var2, double var4,
                                   double var6, double var8, double var10, double var12,
                                   float var14);

EntitySnowShovelFX *EntitySnowShovelFX_create1(World *var1, double var2, double var4, double var6,
                                               double var8, double var10, double var12);
EntitySnowShovelFX *EntitySnowShovelFX_create2(World *var1, double var2, double var4, double var6,
                                               double var8, double var10, double var12,
                                               float var14);

void EntitySnowShovelFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                       float var4, float var5, float var6, float var7);
void EntitySnowShovelFX_onUpdate(Entity *self);

#endif
