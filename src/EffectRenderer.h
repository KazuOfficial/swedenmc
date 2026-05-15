#ifndef EFFECT_RENDERER_H
#define EFFECT_RENDERER_H

#include "World.h"
#include "RenderEngine.h"
#include "JavaRandom.h"

struct Entity;
struct EntityFX;

typedef struct FXLayer {
    struct EntityFX **data;
    int size;
    int cap;
} FXLayer;

typedef struct EffectRenderer {
    World *worldObj;

    FXLayer fxLayers[4];
    RenderEngine *renderer;

    JavaRandom rand;
} EffectRenderer;

EffectRenderer *EffectRenderer_create(World *world, RenderEngine *renderer);
void EffectRenderer_destroy(EffectRenderer *self);

void EffectRenderer_addEffect(EffectRenderer *self, struct EntityFX *var1);

void EffectRenderer_updateEffects(EffectRenderer *self);

void EffectRenderer_renderParticles(EffectRenderer *self, struct Entity *viewer, float partialTick);

void EffectRenderer_func_1187_b(EffectRenderer *self, struct Entity *viewer, float partialTick);

void EffectRenderer_clearEffects(EffectRenderer *self, World *var1);

void EffectRenderer_addBlockDestroyEffects(EffectRenderer *self, int x, int y, int z, int blockID,
                                           int meta);

void EffectRenderer_addBlockHitEffects(EffectRenderer *self, int x, int y, int z, int side);

const char *EffectRenderer_getStatistics(EffectRenderer *self, char *buf, int bufLen);

#endif
