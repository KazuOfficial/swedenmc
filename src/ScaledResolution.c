#include "ScaledResolution.h"
#include <math.h>

void ScaledResolution_init(ScaledResolution *self, GameSettings *var1, int var2, int var3) {
    self->scaledWidth = var2;
    self->scaledHeight = var3;
    self->scaleFactor = 1;
    int var4 = var1->guiScale;
    if (var4 == 0) {
        var4 = 1000;
    }
    while (self->scaleFactor < var4 && self->scaledWidth / (self->scaleFactor + 1) >= 320 &&
           self->scaledHeight / (self->scaleFactor + 1) >= 240) {
        ++self->scaleFactor;
    }
    self->field_25121_a = (double)self->scaledWidth / (double)self->scaleFactor;
    self->field_25120_b = (double)self->scaledHeight / (double)self->scaleFactor;
    self->scaledWidth = (int)ceil(self->field_25121_a);
    self->scaledHeight = (int)ceil(self->field_25120_b);
}

int ScaledResolution_getScaledWidth(const ScaledResolution *self) { return self->scaledWidth; }

int ScaledResolution_getScaledHeight(const ScaledResolution *self) { return self->scaledHeight; }
