#ifndef ENTITY_SMOKE_FX_H
#define ENTITY_SMOKE_FX_H

#include "EntityFX.h"

typedef struct EntitySmokeFX {
    EntityFX base;

    float field_671_a;
} EntitySmokeFX;

void EntitySmokeFX_construct1(EntitySmokeFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12);

void EntitySmokeFX_construct2(EntitySmokeFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12, float var14);

EntitySmokeFX *EntitySmokeFX_create1(World *var1, double var2, double var4, double var6,
                                     double var8, double var10, double var12);
EntitySmokeFX *EntitySmokeFX_create2(World *var1, double var2, double var4, double var6,
                                     double var8, double var10, double var12, float var14);

void EntitySmokeFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                  float var4, float var5, float var6, float var7);
void EntitySmokeFX_onUpdate(Entity *self);

#endif
