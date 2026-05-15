#ifndef ENTITY_RAIN_FX_H
#define ENTITY_RAIN_FX_H

#include "EntityFX.h"

typedef struct EntityRainFX {
    EntityFX base;
} EntityRainFX;

void EntityRainFX_construct(EntityRainFX *self, World *var1, double var2, double var4, double var6);
EntityRainFX *EntityRainFX_create(World *var1, double var2, double var4, double var6);

void EntityRainFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                 float var4, float var5, float var6, float var7);
void EntityRainFX_onUpdate(Entity *self);

#endif
