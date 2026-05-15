#ifndef TESSELLATOR_H
#define TESSELLATOR_H

#include <stdint.h>
#include "Renderer.h"

#define TESSELLATOR_BUFFER_SIZE 2097152

typedef struct Tessellator {
    int rawBuffer[TESSELLATOR_BUFFER_SIZE];
    int vertexCount;
    double textureU;
    double textureV;
    int color;
    int hasColor;
    int hasTexture;
    int hasNormals;
    int rawBufferIndex;
    int addedVertices;
    int isColorDisabled;
    int drawMode;
    double xOffset;
    double yOffset;
    double zOffset;
    int normal;
    int isDrawing;
    int bufferSize;
} Tessellator;

extern __thread Tessellator Tessellator_instance;

void Tessellator_init(Tessellator *self);

void Tessellator_startDrawing(Tessellator *self, int mode);
void Tessellator_startDrawingQuads(Tessellator *self);
void Tessellator_draw(Tessellator *self);

void Tessellator_addVertex(Tessellator *self, double x, double y, double z);
void Tessellator_addVertexWithUV(Tessellator *self, double x, double y, double z, double u,
                                 double v);

void Tessellator_setTextureUV(Tessellator *self, double u, double v);
void Tessellator_setColorOpaque_F(Tessellator *self, float r, float g, float b);
void Tessellator_setColorRGBA_F(Tessellator *self, float r, float g, float b, float a);
void Tessellator_setColorOpaque(Tessellator *self, int r, int g, int b);
void Tessellator_setColorRGBA(Tessellator *self, int r, int g, int b, int a);
void Tessellator_setColorOpaque_I(Tessellator *self, int rgb);
void Tessellator_setColorRGBA_I(Tessellator *self, int rgb, int a);
void Tessellator_disableColor(Tessellator *self);
void Tessellator_setNormal(Tessellator *self, float x, float y, float z);
void Tessellator_setTranslationD(Tessellator *self, double x, double y, double z);
void Tessellator_setTranslationF(Tessellator *self, float x, float y, float z);

void Tessellator_uploadToBuffer(Tessellator *self, RBuffer *buf);

#endif
