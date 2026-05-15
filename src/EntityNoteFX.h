#ifndef ENTITY_NOTE_FX_H
#define ENTITY_NOTE_FX_H

#include "EntityFX.h"

typedef struct EntityNoteFX {
    EntityFX base;

    float field_21065_a;
} EntityNoteFX;

void EntityNoteFX_construct1(EntityNoteFX *self, World *var1, double var2, double var4, double var6,
                             double var8, double var10, double var12);

void EntityNoteFX_construct2(EntityNoteFX *self, World *var1, double var2, double var4, double var6,
                             double var8, double var10, double var12, float var14);

EntityNoteFX *EntityNoteFX_create(World *var1, double var2, double var4, double var6, double var8,
                                  double var10, double var12);

void EntityNoteFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                 float var4, float var5, float var6, float var7);
void EntityNoteFX_onUpdate(Entity *self);

#endif
