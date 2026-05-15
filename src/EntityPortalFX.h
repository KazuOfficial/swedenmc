#ifndef ENTITY_PORTAL_FX_H
#define ENTITY_PORTAL_FX_H

#include "EntityFX.h"

typedef struct EntityPortalFX {
    EntityFX base;

    float field_4083_a;

    double field_4086_p;

    double field_4085_q;

    double field_4084_r;
} EntityPortalFX;

void EntityPortalFX_construct(EntityPortalFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12);
EntityPortalFX *EntityPortalFX_create(World *var1, double var2, double var4, double var6,
                                      double var8, double var10, double var12);

void EntityPortalFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                   float var4, float var5, float var6, float var7);
float EntityPortalFX_getEntityBrightness(Entity *self, float var1);
void EntityPortalFX_onUpdate(Entity *self);

#endif
