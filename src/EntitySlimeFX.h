#ifndef ENTITY_SLIME_FX_H
#define ENTITY_SLIME_FX_H

#include "EntityFX.h"
#include "Item.h"

typedef struct EntitySlimeFX {
    EntityFX base;
} EntitySlimeFX;

void EntitySlimeFX_construct(EntitySlimeFX *self, World *var1, double var2, double var4,
                             double var6, Item *var8);
EntitySlimeFX *EntitySlimeFX_create(World *var1, double var2, double var4, double var6, Item *var8);

int EntitySlimeFX_getFXLayer(EntityFX *self);
void EntitySlimeFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                  float var4, float var5, float var6, float var7);

#endif
