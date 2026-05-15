#ifndef MODEL_RENDERER_H
#define MODEL_RENDERER_H

#include "PositionTextureVertex.h"
#include "TexturedQuad.h"

typedef struct ModelRenderer {

    PositionTextureVertex **corners;

    TexturedQuad **faces;
    int facesLen;

    int textureOffsetX;

    int textureOffsetY;

    float rotationPointX;
    float rotationPointY;
    float rotationPointZ;

    float rotateAngleX;
    float rotateAngleY;
    float rotateAngleZ;

    int mirror;

    int showModel;

    int field_1402_i;
} ModelRenderer;

void ModelRenderer_init(ModelRenderer *self, int var1, int var2);

void ModelRenderer_addBox(ModelRenderer *self, float var1, float var2, float var3, int var4,
                          int var5, int var6);

void ModelRenderer_addBoxExpanded(ModelRenderer *self, float var1, float var2, float var3, int var4,
                                  int var5, int var6, float var7);

void ModelRenderer_setRotationPoint(ModelRenderer *self, float var1, float var2, float var3);

void ModelRenderer_render(ModelRenderer *self, float var1);

void ModelRenderer_renderWithRotation(ModelRenderer *self, float var1);

void ModelRenderer_postRender(ModelRenderer *self, float var1);

void ModelRenderer_destroy(ModelRenderer *self);

#endif
