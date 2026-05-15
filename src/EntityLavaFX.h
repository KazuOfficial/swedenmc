#ifndef ENTITY_LAVA_FX_H
#define ENTITY_LAVA_FX_H

#include "EntityFX.h"

typedef struct EntityLavaFX {
    EntityFX base;

    float field_674_a;
} EntityLavaFX;

void EntityLavaFX_construct(EntityLavaFX *self, World *var1, double var2, double var4, double var6);
EntityLavaFX *EntityLavaFX_create(World *var1, double var2, double var4, double var6);

float EntityLavaFX_getEntityBrightness(Entity *self, float var1);
void EntityLavaFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                 float var4, float var5, float var6, float var7);
void EntityLavaFX_onUpdate(Entity *self);

#endif
