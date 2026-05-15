#ifndef ENTITY_HEART_FX_H
#define ENTITY_HEART_FX_H

#include "EntityFX.h"

typedef struct EntityHeartFX {
    EntityFX base;

    float field_25022_a;
} EntityHeartFX;

void EntityHeartFX_construct1(EntityHeartFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12);

void EntityHeartFX_construct2(EntityHeartFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12, float var14);

EntityHeartFX *EntityHeartFX_create(World *var1, double var2, double var4, double var6, double var8,
                                    double var10, double var12);

void EntityHeartFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                  float var4, float var5, float var6, float var7);
void EntityHeartFX_onUpdate(Entity *self);

#endif
