#ifndef ENTITY_FOOT_STEP_FX_H
#define ENTITY_FOOT_STEP_FX_H

#include "EntityFX.h"
#include "RenderEngine.h"

typedef struct EntityFootStepFX {
    EntityFX base;

    int field_27018_a;

    int field_27020_o;

    RenderEngine *field_27019_p;
} EntityFootStepFX;

void EntityFootStepFX_construct(EntityFootStepFX *self, RenderEngine *var1, World *var2,
                                double var3, double var5, double var7);
EntityFootStepFX *EntityFootStepFX_create(RenderEngine *var1, World *var2, double var3, double var5,
                                          double var7);

void EntityFootStepFX_renderParticle(EntityFX *self, Tessellator *var1, float var2, float var3,
                                     float var4, float var5, float var6, float var7);
void EntityFootStepFX_onUpdate(Entity *self);
int EntityFootStepFX_getFXLayer(EntityFX *self);

#endif
