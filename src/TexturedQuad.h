#ifndef TEXTURED_QUAD_H
#define TEXTURED_QUAD_H

#include "PositionTextureVertex.h"

struct Tessellator;

typedef struct TexturedQuad {

    PositionTextureVertex **vertexPositions;

    int nVertices;

    int invertNormal;
} TexturedQuad;

void TexturedQuad_init(TexturedQuad *self, PositionTextureVertex **var1, int count);

void TexturedQuad_initUV(TexturedQuad *self, PositionTextureVertex **var1, int count, int var2,
                         int var3, int var4, int var5);

void TexturedQuad_flipFace(TexturedQuad *self);

void TexturedQuad_draw(TexturedQuad *self, struct Tessellator *var1, float var2);

void TexturedQuad_addToTessellator(TexturedQuad *self, struct Tessellator *var1, float var2);

#endif
