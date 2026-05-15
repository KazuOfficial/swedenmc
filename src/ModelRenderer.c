#include "ModelRenderer.h"
#include "Renderer.h"
#include "Tessellator.h"
#include <math.h>
#include <stdlib.h>

void ModelRenderer_init(ModelRenderer *self, int var1, int var2) {
    self->corners = NULL;
    self->faces = NULL;
    self->facesLen = 0;
    self->textureOffsetX = var1;
    self->textureOffsetY = var2;
    self->rotationPointX = 0.0F;
    self->rotationPointY = 0.0F;
    self->rotationPointZ = 0.0F;
    self->rotateAngleX = 0.0F;
    self->rotateAngleY = 0.0F;
    self->rotateAngleZ = 0.0F;
    self->mirror = 0;
    self->showModel = 1;
    self->field_1402_i = 0;
}

void ModelRenderer_addBox(ModelRenderer *self, float var1, float var2, float var3, int var4,
                          int var5, int var6) {
    ModelRenderer_addBoxExpanded(self, var1, var2, var3, var4, var5, var6, 0.0F);
}

void ModelRenderer_addBoxExpanded(ModelRenderer *self, float var1, float var2, float var3, int var4,
                                  int var5, int var6, float var7) {

    free(self->corners);
    self->corners = malloc(8 * sizeof(PositionTextureVertex *));
    for (int i = 0; i < 8; ++i)
        self->corners[i] = calloc(1, sizeof(PositionTextureVertex));

    if (self->faces) {
        for (int i = 0; i < self->facesLen; ++i)
            free(self->faces[i]);
        free(self->faces);
    }
    self->faces = malloc(6 * sizeof(TexturedQuad *));
    self->facesLen = 6;
    for (int i = 0; i < 6; ++i)
        self->faces[i] = calloc(1, sizeof(TexturedQuad));

    float var8 = var1 + (float)var4;
    float var9 = var2 + (float)var5;
    float var10 = var3 + (float)var6;
    var1 -= var7;
    var2 -= var7;
    var3 -= var7;
    var8 += var7;
    var9 += var7;
    var10 += var7;
    if (self->mirror) {
        float var11 = var8;
        var8 = var1;
        var1 = var11;
    }

    PositionTextureVertex_initF(self->corners[0], var1, var2, var3, 0.0F, 0.0F);
    PositionTextureVertex_initF(self->corners[1], var8, var2, var3, 0.0F, 8.0F);
    PositionTextureVertex_initF(self->corners[2], var8, var9, var3, 8.0F, 8.0F);
    PositionTextureVertex_initF(self->corners[3], var1, var9, var3, 8.0F, 0.0F);
    PositionTextureVertex_initF(self->corners[4], var1, var2, var10, 0.0F, 0.0F);
    PositionTextureVertex_initF(self->corners[5], var8, var2, var10, 0.0F, 8.0F);
    PositionTextureVertex_initF(self->corners[6], var8, var9, var10, 8.0F, 8.0F);
    PositionTextureVertex_initF(self->corners[7], var1, var9, var10, 8.0F, 0.0F);

    PositionTextureVertex *f0[4] = {self->corners[5], self->corners[1], self->corners[2],
                                    self->corners[6]};
    TexturedQuad_initUV(self->faces[0], f0, 4, self->textureOffsetX + var6 + var4,
                        self->textureOffsetY + var6, self->textureOffsetX + var6 + var4 + var6,
                        self->textureOffsetY + var6 + var5);

    PositionTextureVertex *f1[4] = {self->corners[0], self->corners[4], self->corners[7],
                                    self->corners[3]};
    TexturedQuad_initUV(self->faces[1], f1, 4, self->textureOffsetX + 0,
                        self->textureOffsetY + var6, self->textureOffsetX + var6,
                        self->textureOffsetY + var6 + var5);

    PositionTextureVertex *f2[4] = {self->corners[5], self->corners[4], self->corners[0],
                                    self->corners[1]};
    TexturedQuad_initUV(self->faces[2], f2, 4, self->textureOffsetX + var6,
                        self->textureOffsetY + 0, self->textureOffsetX + var6 + var4,
                        self->textureOffsetY + var6);

    PositionTextureVertex *f3[4] = {self->corners[2], self->corners[3], self->corners[7],
                                    self->corners[6]};
    TexturedQuad_initUV(self->faces[3], f3, 4, self->textureOffsetX + var6 + var4,
                        self->textureOffsetY + 0, self->textureOffsetX + var6 + var4 + var4,
                        self->textureOffsetY + var6);

    PositionTextureVertex *f4[4] = {self->corners[1], self->corners[0], self->corners[3],
                                    self->corners[2]};
    TexturedQuad_initUV(self->faces[4], f4, 4, self->textureOffsetX + var6,
                        self->textureOffsetY + var6, self->textureOffsetX + var6 + var4,
                        self->textureOffsetY + var6 + var5);

    PositionTextureVertex *f5[4] = {self->corners[4], self->corners[5], self->corners[6],
                                    self->corners[7]};
    TexturedQuad_initUV(self->faces[5], f5, 4, self->textureOffsetX + var6 + var4 + var6,
                        self->textureOffsetY + var6,
                        self->textureOffsetX + var6 + var4 + var6 + var4,
                        self->textureOffsetY + var6 + var5);

    if (self->mirror) {
        for (int var19 = 0; var19 < self->facesLen; ++var19) {
            TexturedQuad_flipFace(self->faces[var19]);
        }
    }
}

void ModelRenderer_setRotationPoint(ModelRenderer *self, float var1, float var2, float var3) {
    self->rotationPointX = var1;
    self->rotationPointY = var2;
    self->rotationPointZ = var3;
}

static void ModelRenderer_drawFaces(ModelRenderer *self, float var1) {
    Tessellator *var2 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var2);
    for (int var3 = 0; var3 < self->facesLen; ++var3)
        TexturedQuad_addToTessellator(self->faces[var3], var2, var1);
    Tessellator_draw(var2);
}

void ModelRenderer_render(ModelRenderer *self, float var1) {
    if (self->field_1402_i || !self->showModel)
        return;
    if (self->rotateAngleX == 0.0F && self->rotateAngleY == 0.0F && self->rotateAngleZ == 0.0F) {
        if (self->rotationPointX == 0.0F && self->rotationPointY == 0.0F &&
            self->rotationPointZ == 0.0F) {
            ModelRenderer_drawFaces(self, var1);
        } else {
            R_translatef(self->rotationPointX * var1, self->rotationPointY * var1,
                         self->rotationPointZ * var1);
            ModelRenderer_drawFaces(self, var1);
            R_translatef(-self->rotationPointX * var1, -self->rotationPointY * var1,
                         -self->rotationPointZ * var1);
        }
    } else {
        R_pushMatrix();
        R_translatef(self->rotationPointX * var1, self->rotationPointY * var1,
                     self->rotationPointZ * var1);
        if (self->rotateAngleZ != 0.0F)
            R_rotatef(self->rotateAngleZ * (180.0F / (float)M_PI), 0.0F, 0.0F, 1.0F);
        if (self->rotateAngleY != 0.0F)
            R_rotatef(self->rotateAngleY * (180.0F / (float)M_PI), 0.0F, 1.0F, 0.0F);
        if (self->rotateAngleX != 0.0F)
            R_rotatef(self->rotateAngleX * (180.0F / (float)M_PI), 1.0F, 0.0F, 0.0F);
        ModelRenderer_drawFaces(self, var1);
        R_popMatrix();
    }
}

void ModelRenderer_renderWithRotation(ModelRenderer *self, float var1) {
    if (self->field_1402_i || !self->showModel)
        return;
    R_pushMatrix();
    R_translatef(self->rotationPointX * var1, self->rotationPointY * var1,
                 self->rotationPointZ * var1);
    if (self->rotateAngleY != 0.0F)
        R_rotatef(self->rotateAngleY * (180.0F / (float)M_PI), 0.0F, 1.0F, 0.0F);
    if (self->rotateAngleX != 0.0F)
        R_rotatef(self->rotateAngleX * (180.0F / (float)M_PI), 1.0F, 0.0F, 0.0F);
    if (self->rotateAngleZ != 0.0F)
        R_rotatef(self->rotateAngleZ * (180.0F / (float)M_PI), 0.0F, 0.0F, 1.0F);
    ModelRenderer_drawFaces(self, var1);
    R_popMatrix();
}

void ModelRenderer_postRender(ModelRenderer *self, float var1) {
    if (self->field_1402_i || !self->showModel)
        return;
    if (self->rotateAngleX == 0.0F && self->rotateAngleY == 0.0F && self->rotateAngleZ == 0.0F) {
        if (self->rotationPointX != 0.0F || self->rotationPointY != 0.0F ||
            self->rotationPointZ != 0.0F) {
            R_translatef(self->rotationPointX * var1, self->rotationPointY * var1,
                         self->rotationPointZ * var1);
        }
    } else {
        R_translatef(self->rotationPointX * var1, self->rotationPointY * var1,
                     self->rotationPointZ * var1);
        if (self->rotateAngleZ != 0.0F)
            R_rotatef(self->rotateAngleZ * (180.0F / (float)M_PI), 0.0F, 0.0F, 1.0F);
        if (self->rotateAngleY != 0.0F)
            R_rotatef(self->rotateAngleY * (180.0F / (float)M_PI), 0.0F, 1.0F, 0.0F);
        if (self->rotateAngleX != 0.0F)
            R_rotatef(self->rotateAngleX * (180.0F / (float)M_PI), 1.0F, 0.0F, 0.0F);
    }
}

void ModelRenderer_destroy(ModelRenderer *self) {
    if (self->corners) {
        for (int i = 0; i < 8; i++) {
            if (self->corners[i]) {
                free(self->corners[i]->vector3D);
                free(self->corners[i]);
            }
        }
        free(self->corners);
        self->corners = NULL;
    }
    if (self->faces) {
        for (int i = 0; i < self->facesLen; i++) {
            if (self->faces[i]) {
                for (int j = 0; j < self->faces[i]->nVertices; j++)
                    free(self->faces[i]->vertexPositions[j]);
                free(self->faces[i]->vertexPositions);
                free(self->faces[i]);
            }
        }
        free(self->faces);
        self->faces = NULL;
    }
}
