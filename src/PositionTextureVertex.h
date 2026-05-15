#ifndef POSITION_TEXTURE_VERTEX_H
#define POSITION_TEXTURE_VERTEX_H

#include "Vec3D.h"

typedef struct PositionTextureVertex {

    Vec3D *vector3D;

    float texturePositionX;

    float texturePositionY;
} PositionTextureVertex;

void PositionTextureVertex_initF(PositionTextureVertex *self, float var1, float var2, float var3,
                                 float var4, float var5);

void PositionTextureVertex_initV(PositionTextureVertex *self, Vec3D *var1, float var2, float var3);

void PositionTextureVertex_initCopy(PositionTextureVertex *self, PositionTextureVertex *var1,
                                    float var2, float var3);

PositionTextureVertex *PositionTextureVertex_setTexturePosition(PositionTextureVertex *self,
                                                                float var1, float var2);

#endif
