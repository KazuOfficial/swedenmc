#ifndef ENTITY_REDDUST_FX_H
#define ENTITY_REDDUST_FX_H

#include "EntityFX.h"

typedef struct EntityReddustFX {
    EntityFX base;

    float field_673_a;
} EntityReddustFX;

void EntityReddustFX_construct1(EntityReddustFX *self, World *var1, double var2, double var4,
                                double var6, float var8, float var9, float var10);

void EntityReddustFX_construct2(EntityReddustFX *self, World *var1, double var2, double var4,
                                double var6, float var8, float var9, float var10, float var11);

EntityReddustFX *EntityReddustFX_create1(World *var1, double var2, double var4, double var6,
                                         float var8, float var9, float var10);
EntityReddustFX *EntityReddustFX_create2(World *var1, double var2, double var4, double var6,
                                         float var8, float var9, float var10, float var11);

void EntityReddustFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                    float var4, float var5, float var6, float var7);
void EntityReddustFX_onUpdate(Entity *self);

#endif
