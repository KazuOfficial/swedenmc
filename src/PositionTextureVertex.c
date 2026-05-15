#include "PositionTextureVertex.h"
#include <stdlib.h>

void PositionTextureVertex_initF(PositionTextureVertex *self, float var1, float var2, float var3,
                                 float var4, float var5) {

    PositionTextureVertex_initV(
        self, Vec3D_createVectorHelper((double)var1, (double)var2, (double)var3), var4, var5);
}

void PositionTextureVertex_initV(PositionTextureVertex *self, Vec3D *var1, float var2, float var3) {
    self->vector3D = var1;
    self->texturePositionX = var2;
    self->texturePositionY = var3;
}

void PositionTextureVertex_initCopy(PositionTextureVertex *self, PositionTextureVertex *var1,
                                    float var2, float var3) {
    self->vector3D = var1->vector3D;
    self->texturePositionX = var2;
    self->texturePositionY = var3;
}

PositionTextureVertex *PositionTextureVertex_setTexturePosition(PositionTextureVertex *self,
                                                                float var1, float var2) {
    PositionTextureVertex *copy = calloc(1, sizeof(PositionTextureVertex));
    PositionTextureVertex_initCopy(copy, self, var1, var2);
    return copy;
}
