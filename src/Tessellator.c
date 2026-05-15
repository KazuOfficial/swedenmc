#include "Tessellator.h"
#include "Renderer.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

__thread Tessellator Tessellator_instance;

static inline int float_to_bits(float f) {
    int i;
    memcpy(&i, &f, 4);
    return i;
}

void Tessellator_init(Tessellator *self) {
    memset(self, 0, sizeof(Tessellator));
    self->bufferSize = TESSELLATOR_BUFFER_SIZE;
}

static void ensure_init(void) {
    if (!Tessellator_instance.bufferSize)
        Tessellator_init(&Tessellator_instance);
}

static void reset(Tessellator *self) {
    self->vertexCount = 0;
    self->rawBufferIndex = 0;
    self->addedVertices = 0;
}

void Tessellator_startDrawing(Tessellator *self, int mode) {
    ensure_init();
    if (self->isDrawing) {
        fprintf(stderr, "Already tesselating!\n");
        return;
    }
    self->isDrawing = 1;
    reset(self);
    self->drawMode = mode;
    self->hasNormals = 0;
    self->hasColor = 0;
    self->hasTexture = 0;
    self->isColorDisabled = 0;
}

void Tessellator_startDrawingQuads(Tessellator *self) { Tessellator_startDrawing(self, 7); }

void Tessellator_draw(Tessellator *self) {
    ensure_init();
    if (!self->isDrawing) {
        fprintf(stderr, "Not tesselating!\n");
        return;
    }
    self->isDrawing = 0;

    if (self->vertexCount > 0) {
        RFlags flags = 0;
        if (self->hasTexture)
            flags |= R_HAS_TEXTURE;
        if (self->hasColor)
            flags |= R_HAS_COLOR;
        if (self->hasNormals)
            flags |= R_HAS_NORMALS;
        int mode = (self->drawMode == 7) ? R_TRIANGLES : self->drawMode;
        R_drawImmediate(self->rawBuffer, self->vertexCount, mode, flags);
    }

    reset(self);
}

void Tessellator_uploadToBuffer(Tessellator *self, RBuffer *buf) {
    ensure_init();
    if (!self->isDrawing)
        return;
    self->isDrawing = 0;

    RFlags flags = 0;
    if (self->hasTexture)
        flags |= R_HAS_TEXTURE;
    if (self->hasColor)
        flags |= R_HAS_COLOR;
    if (self->hasNormals)
        flags |= R_HAS_NORMALS;
    int mode = (self->drawMode == 7) ? R_TRIANGLES : self->drawMode;
    R_bufferUpload(buf, self->rawBuffer, self->vertexCount, mode, flags);

    reset(self);
}

void Tessellator_setTextureUV(Tessellator *self, double u, double v) {
    self->hasTexture = 1;
    self->textureU = u;
    self->textureV = v;
}

void Tessellator_setColorRGBA(Tessellator *self, int r, int g, int b, int a) {
    if (self->isColorDisabled)
        return;
    if (r > 255)
        r = 255;
    if (r < 0)
        r = 0;
    if (g > 255)
        g = 255;
    if (g < 0)
        g = 0;
    if (b > 255)
        b = 255;
    if (b < 0)
        b = 0;
    if (a > 255)
        a = 255;
    if (a < 0)
        a = 0;
    self->hasColor = 1;

    self->color = (int)(((unsigned int)a << 24) | ((unsigned int)b << 16) | ((unsigned int)g << 8) |
                        (unsigned int)r);
}

void Tessellator_setColorOpaque(Tessellator *self, int r, int g, int b) {
    Tessellator_setColorRGBA(self, r, g, b, 255);
}

void Tessellator_setColorOpaque_F(Tessellator *self, float r, float g, float b) {
    Tessellator_setColorOpaque(self, (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f));
}

void Tessellator_setColorRGBA_F(Tessellator *self, float r, float g, float b, float a) {
    Tessellator_setColorRGBA(self, (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f),
                             (int)(a * 255.0f));
}

void Tessellator_setColorOpaque_I(Tessellator *self, int rgb) {
    Tessellator_setColorOpaque(self, (rgb >> 16) & 255, (rgb >> 8) & 255, rgb & 255);
}

void Tessellator_setColorRGBA_I(Tessellator *self, int rgb, int a) {
    Tessellator_setColorRGBA(self, (rgb >> 16) & 255, (rgb >> 8) & 255, rgb & 255, a);
}

void Tessellator_disableColor(Tessellator *self) { self->isColorDisabled = 1; }

void Tessellator_setNormal(Tessellator *self, float x, float y, float z) {
    if (!self->isDrawing) {
        printf("But..\n");
    }
    self->hasNormals = 1;

    signed char nx = (signed char)((int)(x * 127.0f));
    signed char ny = (signed char)((int)(y * 127.0f));
    signed char nz = (signed char)((int)(z * 127.0f));
    self->normal =
        (int)((unsigned int)(int)nx | ((unsigned int)(int)ny << 8) | ((unsigned int)(int)nz << 16));
}

void Tessellator_setTranslationD(Tessellator *self, double x, double y, double z) {
    self->xOffset = x;
    self->yOffset = y;
    self->zOffset = z;
}

void Tessellator_setTranslationF(Tessellator *self, float x, float y, float z) {
    self->xOffset += (double)x;
    self->yOffset += (double)y;
    self->zOffset += (double)z;
}

void Tessellator_addVertex(Tessellator *self, double x, double y, double z) {
    ensure_init();
    ++self->addedVertices;

    if (self->drawMode == 7 && self->addedVertices % 4 == 0) {
        for (int var7 = 0; var7 < 2; ++var7) {
            int var8 = 8 * (3 - var7);
            int src = self->rawBufferIndex - var8;
            if (self->hasTexture) {
                self->rawBuffer[self->rawBufferIndex + 3] = self->rawBuffer[src + 3];
                self->rawBuffer[self->rawBufferIndex + 4] = self->rawBuffer[src + 4];
            }
            if (self->hasColor) {
                self->rawBuffer[self->rawBufferIndex + 5] = self->rawBuffer[src + 5];
            }
            if (self->hasNormals) {
                self->rawBuffer[self->rawBufferIndex + 6] = self->rawBuffer[src + 6];
            }
            self->rawBuffer[self->rawBufferIndex + 0] = self->rawBuffer[src + 0];
            self->rawBuffer[self->rawBufferIndex + 1] = self->rawBuffer[src + 1];
            self->rawBuffer[self->rawBufferIndex + 2] = self->rawBuffer[src + 2];
            ++self->vertexCount;
            self->rawBufferIndex += 8;
        }
    }

    if (self->hasTexture) {
        self->rawBuffer[self->rawBufferIndex + 3] = float_to_bits((float)self->textureU);
        self->rawBuffer[self->rawBufferIndex + 4] = float_to_bits((float)self->textureV);
    }
    if (self->hasColor) {
        self->rawBuffer[self->rawBufferIndex + 5] = self->color;
    }
    if (self->hasNormals) {
        self->rawBuffer[self->rawBufferIndex + 6] = self->normal;
    }
    self->rawBuffer[self->rawBufferIndex + 0] = float_to_bits((float)(x + self->xOffset));
    self->rawBuffer[self->rawBufferIndex + 1] = float_to_bits((float)(y + self->yOffset));
    self->rawBuffer[self->rawBufferIndex + 2] = float_to_bits((float)(z + self->zOffset));
    self->rawBufferIndex += 8;
    ++self->vertexCount;

    if (self->vertexCount % 4 == 0 && self->rawBufferIndex >= self->bufferSize - 32) {
        Tessellator_draw(self);
        self->isDrawing = 1;
    }
}

void Tessellator_addVertexWithUV(Tessellator *self, double x, double y, double z, double u,
                                 double v) {
    Tessellator_setTextureUV(self, u, v);
    Tessellator_addVertex(self, x, y, z);
}
