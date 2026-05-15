#ifndef ENTITY_EXPLODE_FX_H
#define ENTITY_EXPLODE_FX_H

#include "EntityFX.h"
#include "Tessellator.h"

typedef struct EntityExplodeFX {
    EntityFX base;
} EntityExplodeFX;

void EntityExplodeFX_construct(EntityExplodeFX *self, World *var1, double var2, double var4,
                               double var6, double var8, double var10, double var12);

void EntityExplodeFX_renderParticle(EntityExplodeFX *self, Tessellator *var1, float var2,
                                    float var3, float var4, float var5, float var6, float var7);

void EntityExplodeFX_onUpdate(EntityExplodeFX *self);

#endif
