#include "TexturedQuad.h"
#include "Tessellator.h"
#include <stdlib.h>
#include <string.h>

void TexturedQuad_init(TexturedQuad *self, PositionTextureVertex **var1, int count) {
    self->invertNormal = 0;
    self->nVertices = count;

    self->vertexPositions = malloc(count * sizeof(PositionTextureVertex *));
    memcpy(self->vertexPositions, var1, count * sizeof(PositionTextureVertex *));
}

void TexturedQuad_initUV(TexturedQuad *self, PositionTextureVertex **var1, int count, int var2,
                         int var3, int var4, int var5) {
    TexturedQuad_init(self, var1, count);
    float var6 = 0.0015625F;
    float var7 = 0.003125F;
    self->vertexPositions[0] = PositionTextureVertex_setTexturePosition(
        self->vertexPositions[0], (float)var4 / 64.0F - var6, (float)var3 / 32.0F + var7);
    self->vertexPositions[1] = PositionTextureVertex_setTexturePosition(
        self->vertexPositions[1], (float)var2 / 64.0F + var6, (float)var3 / 32.0F + var7);
    self->vertexPositions[2] = PositionTextureVertex_setTexturePosition(
        self->vertexPositions[2], (float)var2 / 64.0F + var6, (float)var5 / 32.0F - var7);
    self->vertexPositions[3] = PositionTextureVertex_setTexturePosition(
        self->vertexPositions[3], (float)var4 / 64.0F - var6, (float)var5 / 32.0F - var7);
}

void TexturedQuad_flipFace(TexturedQuad *self) {
    PositionTextureVertex **var1 = malloc(self->nVertices * sizeof(PositionTextureVertex *));
    for (int var2 = 0; var2 < self->nVertices; ++var2) {
        var1[var2] = self->vertexPositions[self->nVertices - var2 - 1];
    }

    free(self->vertexPositions);
    self->vertexPositions = var1;
}

void TexturedQuad_draw(TexturedQuad *self, struct Tessellator *var1, float var2) {
    Tessellator_startDrawingQuads(var1);
    TexturedQuad_addToTessellator(self, var1, var2);
    Tessellator_draw(var1);
}

void TexturedQuad_addToTessellator(TexturedQuad *self, struct Tessellator *var1, float var2) {
    Vec3D *var3 =
        Vec3D_subtract(self->vertexPositions[1]->vector3D, self->vertexPositions[0]->vector3D);
    Vec3D *var4 =
        Vec3D_subtract(self->vertexPositions[1]->vector3D, self->vertexPositions[2]->vector3D);
    Vec3D *var5 = Vec3D_normalize(Vec3D_crossProduct(var4, var3));
    if (self->invertNormal) {
        Tessellator_setNormal(var1, -(float)var5->xCoord, -(float)var5->yCoord,
                              -(float)var5->zCoord);
    } else {
        Tessellator_setNormal(var1, (float)var5->xCoord, (float)var5->yCoord, (float)var5->zCoord);
    }

    for (int var6 = 0; var6 < 4; ++var6) {
        PositionTextureVertex *var7 = self->vertexPositions[var6];
        Tessellator_addVertexWithUV(var1, (double)((float)var7->vector3D->xCoord * var2),
                                    (double)((float)var7->vector3D->yCoord * var2),
                                    (double)((float)var7->vector3D->zCoord * var2),
                                    (double)var7->texturePositionX, (double)var7->texturePositionY);
    }
}
