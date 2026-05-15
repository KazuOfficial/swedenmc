#ifndef ENTITY_SPLASH_FX_H
#define ENTITY_SPLASH_FX_H

#include "EntityRainFX.h"

typedef struct EntitySplashFX {
    EntityRainFX base;
} EntitySplashFX;

void EntitySplashFX_construct(EntitySplashFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12);

EntitySplashFX *EntitySplashFX_create(World *var1, double var2, double var4, double var6,
                                      double var8, double var10, double var12);

#endif
