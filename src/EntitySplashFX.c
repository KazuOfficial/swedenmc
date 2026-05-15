#include "EntitySplashFX.h"
#include <stdlib.h>

void EntitySplashFX_construct(EntitySplashFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12) {

    EntityRainFX_construct(&self->base, var1, var2, var4, var6);
    self->base.base.particleGravity = 0.04F;
    ++self->base.base.particleTextureIndex;
    if (var10 == 0.0 && (var8 != 0.0 || var12 != 0.0)) {
        self->base.base.base.motionX = var8;
        self->base.base.base.motionY = var10 + 0.1;
        self->base.base.base.motionZ = var12;
    }
}

EntitySplashFX *EntitySplashFX_create(World *var1, double var2, double var4, double var6,
                                      double var8, double var10, double var12) {
    EntitySplashFX *self = (EntitySplashFX *)calloc(1, sizeof(EntitySplashFX));
    EntitySplashFX_construct(self, var1, var2, var4, var6, var8, var10, var12);
    return self;
}
