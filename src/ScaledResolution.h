#ifndef SCALED_RESOLUTION_H
#define SCALED_RESOLUTION_H

#include "GameSettings.h"

typedef struct ScaledResolution {

    int scaledWidth;

    int scaledHeight;

    double field_25121_a;

    double field_25120_b;

    int scaleFactor;
} ScaledResolution;

void ScaledResolution_init(ScaledResolution *self, GameSettings *var1, int var2, int var3);

int ScaledResolution_getScaledWidth(const ScaledResolution *self);

int ScaledResolution_getScaledHeight(const ScaledResolution *self);

#endif
