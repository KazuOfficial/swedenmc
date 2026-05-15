#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "Renderer.h"
#include "GameSettings.h"
#include "TextureFX.h"
#include "TexturePackList.h"
#include "ThreadDownloadImageData.h"
#include "ImageBuffer.h"
#include <stdlib.h>

extern int RenderEngine_useMipmaps;

typedef struct TextureEntry {
    char *name;
    unsigned int texId;
    struct TextureEntry *next;
} TextureEntry;
#define RENDER_ENGINE_MAP_SIZE 256

typedef struct TexNameEntry {
    int key;
    void *value;
    int width;
    int height;
    struct TexNameEntry *next;
} TexNameEntry;
#define TEX_NAME_MAP_SIZE 64

typedef struct UrlImageEntry {
    char *key;
    ThreadDownloadImageData *value;
    struct UrlImageEntry *next;
} UrlImageEntry;
#define URL_MAP_SIZE 64

typedef struct PixelDataEntry {
    char *key;
    int *value;
    struct PixelDataEntry *next;
} PixelDataEntry;
#define PIXEL_DATA_MAP_SIZE 64

typedef struct RenderEngine {

    TextureEntry *textureMap[RENDER_ENGINE_MAP_SIZE];

    PixelDataEntry *field_28151_c[PIXEL_DATA_MAP_SIZE];

    TexNameEntry *textureNameToImageMap[TEX_NAME_MAP_SIZE];

    UrlImageEntry *urlToImageDataMap[URL_MAP_SIZE];

    TextureFX **textureList;
    int textureListSize;
    int textureListCapacity;

    GameSettings *options;

    TexturePackList *texturePack;

    int clampTexture;

    int blurTexture;

    unsigned char imageData[1048576];
} RenderEngine;

RenderEngine *RenderEngine_create(TexturePackList *texturePack, GameSettings *options);
void RenderEngine_destroy(RenderEngine *self);

unsigned int RenderEngine_getTexture(RenderEngine *self, const char *name);

void RenderEngine_bindTexture(RenderEngine *self, int var1);

void RenderEngine_deleteTexture(RenderEngine *self, int var1);

int RenderEngine_allocateAndSetupTexture(RenderEngine *self, void *var1, int w, int h);

void RenderEngine_setupTextureRaw(RenderEngine *self, unsigned int texId, int width, int height,
                                  const unsigned char *rgba);

void RenderEngine_func_28150_a(RenderEngine *self, const int *var1, int var2, int var3, int var4);

void RenderEngine_registerTextureFX(RenderEngine *self, TextureFX *var1);

void RenderEngine_updateDynamicTextures(RenderEngine *self);

void RenderEngine_refreshTextures(RenderEngine *self);

int RenderEngine_getTextureForDownloadableImage(RenderEngine *self, const char *var1,
                                                const char *var2);

ThreadDownloadImageData *RenderEngine_obtainImageData(RenderEngine *self, const char *var1,
                                                      ImageBuffer *var2);

void RenderEngine_releaseImageData(RenderEngine *self, const char *var1);

int *RenderEngine_func_28149_a(RenderEngine *self, const char *var1);

#endif
