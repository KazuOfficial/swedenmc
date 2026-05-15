#ifndef ENTITY_PICKUP_FX_H
#define ENTITY_PICKUP_FX_H

#include "EntityFX.h"

typedef struct EntityPickupFX {
    EntityFX base;

    Entity *field_675_a;

    Entity *field_679_o;

    int field_678_p;

    int field_677_q;

    float field_676_r;
} EntityPickupFX;

void EntityPickupFX_construct(EntityPickupFX *self, World *var1, Entity *var2, Entity *var3,
                              float var4);
EntityPickupFX *EntityPickupFX_create(World *var1, Entity *var2, Entity *var3, float var4);

void EntityPickupFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                   float var4, float var5, float var6, float var7);
void EntityPickupFX_onUpdate(Entity *self);
int EntityPickupFX_getFXLayer(EntityFX *self);

#endif
