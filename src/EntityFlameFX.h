#ifndef ENTITY_FLAME_FX_H
#define ENTITY_FLAME_FX_H

#include "EntityFX.h"

typedef struct EntityFlameFX {
    EntityFX base;

    float field_672_a;
} EntityFlameFX;

void EntityFlameFX_construct(EntityFlameFX *self, World *var1, double var2, double var4,
                             double var6, double var8, double var10, double var12);
EntityFlameFX *EntityFlameFX_create(World *var1, double var2, double var4, double var6, double var8,
                                    double var10, double var12);

void EntityFlameFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                  float var4, float var5, float var6, float var7);
float EntityFlameFX_getEntityBrightness(Entity *self, float var1);
void EntityFlameFX_onUpdate(Entity *self);

#endif
