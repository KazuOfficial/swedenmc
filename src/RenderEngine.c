#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#include "RenderEngine.h"
#include "TexturePackBase.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int RenderEngine_useMipmaps = 0;

static int *func_28147_a(unsigned char *rgba, int w, int h, int *var2);
static int *func_28148_b(unsigned char *rgba, int w, int h);

static unsigned int str_hash(const char *s, unsigned int mod) {
    unsigned int h = 5381;
    while (*s)
        h = h * 33 ^ (unsigned char)*s++;
    return h % mod;
}

static unsigned int int_hash(int k, unsigned int mod) { return (unsigned int)k % mod; }

static unsigned int texmap_get(RenderEngine *self, const char *name) {
    unsigned int h = str_hash(name, RENDER_ENGINE_MAP_SIZE);
    for (TextureEntry *e = self->textureMap[h]; e; e = e->next)
        if (!strcmp(e->name, name))
            return e->texId;
    return 0;
}
static void texmap_put(RenderEngine *self, const char *name, unsigned int id) {
    unsigned int h = str_hash(name, RENDER_ENGINE_MAP_SIZE);
    TextureEntry *e = (TextureEntry *)calloc(1, sizeof(TextureEntry));
    e->name = strdup(name);
    e->texId = id;
    e->next = self->textureMap[h];
    self->textureMap[h] = e;
}

static TexNameEntry *namemap_find(RenderEngine *self, int key) {
    unsigned int h = int_hash(key, TEX_NAME_MAP_SIZE);
    for (TexNameEntry *e = self->textureNameToImageMap[h]; e; e = e->next)
        if (e->key == key)
            return e;
    return NULL;
}
static void namemap_put(RenderEngine *self, int key, void *val, int w, int h) {
    unsigned int bkt = int_hash(key, TEX_NAME_MAP_SIZE);
    TexNameEntry *e = (TexNameEntry *)calloc(1, sizeof(TexNameEntry));
    e->key = key;
    e->value = val;
    e->width = w;
    e->height = h;
    e->next = self->textureNameToImageMap[bkt];
    self->textureNameToImageMap[bkt] = e;
}
static void namemap_remove(RenderEngine *self, int key) {
    unsigned int h = int_hash(key, TEX_NAME_MAP_SIZE);
    TexNameEntry **pp = &self->textureNameToImageMap[h];
    while (*pp) {
        if ((*pp)->key == key) {
            TexNameEntry *tmp = *pp;
            *pp = tmp->next;
            free(tmp);
            return;
        }
        pp = &(*pp)->next;
    }
}

static ThreadDownloadImageData *urlmap_get(RenderEngine *self, const char *key) {
    unsigned int h = str_hash(key, URL_MAP_SIZE);
    for (UrlImageEntry *e = self->urlToImageDataMap[h]; e; e = e->next)
        if (!strcmp(e->key, key))
            return e->value;
    return NULL;
}
static void urlmap_put(RenderEngine *self, const char *key, ThreadDownloadImageData *val) {
    unsigned int h = str_hash(key, URL_MAP_SIZE);
    UrlImageEntry *e = (UrlImageEntry *)calloc(1, sizeof(UrlImageEntry));
    e->key = strdup(key);
    e->value = val;
    e->next = self->urlToImageDataMap[h];
    self->urlToImageDataMap[h] = e;
}
static void urlmap_remove(RenderEngine *self, const char *key) {
    unsigned int h = str_hash(key, URL_MAP_SIZE);
    UrlImageEntry **pp = &self->urlToImageDataMap[h];
    while (*pp) {
        if (!strcmp((*pp)->key, key)) {
            UrlImageEntry *tmp = *pp;
            *pp = tmp->next;
            free(tmp->key);
            free(tmp);
            return;
        }
        pp = &(*pp)->next;
    }
}

static int averageColor(int var1, int var2) {
    int var3 = (var1 & -16777216) >> 24 & 255;
    int var4 = (var2 & -16777216) >> 24 & 255;
    return (int)((unsigned int)((var3 + var4) >> 1) << 24) +
           (((var1 & 16711422) + (var2 & 16711422)) >> 1);
}

static int weightedAverageColor(int var1, int var2) {
    int var3 = (var1 & -16777216) >> 24 & 255;
    int var4 = (var2 & -16777216) >> 24 & 255;
    int var5 = 255;
    if (var3 + var4 == 0) {
        var3 = 1;
        var4 = 1;
        var5 = 0;
    }
    int var6 = (var1 >> 16 & 255) * var3;
    int var7 = (var1 >> 8 & 255) * var3;
    int var8 = (var1 & 255) * var3;
    int var9 = (var2 >> 16 & 255) * var4;
    int var10 = (var2 >> 8 & 255) * var4;
    int var11 = (var2 & 255) * var4;
    int var12 = (var6 + var9) / (var3 + var4);
    int var13 = (var7 + var10) / (var3 + var4);
    int var14 = (var8 + var11) / (var3 + var4);
    return (int)(((unsigned int)var5 << 24) | ((unsigned int)var12 << 16) |
                 ((unsigned int)var13 << 8) | (unsigned int)var14);
}

static unsigned char *unwrapImageByColumns(const unsigned char *src, int srcW, int srcH, int *outW,
                                           int *outH) {
    int var2 = srcW / 16;
    *outW = 16;
    *outH = srcH * var2;
    unsigned char *dst = (unsigned char *)malloc((size_t)(*outW) * (size_t)(*outH) * 4);
    if (!dst)
        return NULL;
    for (int var5 = 0; var5 < var2; ++var5) {
        for (int y = 0; y < srcH; ++y) {
            for (int x = 0; x < 16; ++x) {
                int srcIdx = (y * srcW + var5 * 16 + x) * 4;
                int dstIdx = ((var5 * srcH + y) * 16 + x) * 4;
                dst[dstIdx + 0] = src[srcIdx + 0];
                dst[dstIdx + 1] = src[srcIdx + 1];
                dst[dstIdx + 2] = src[srcIdx + 2];
                dst[dstIdx + 3] = src[srcIdx + 3];
            }
        }
    }
    return dst;
}

static unsigned char *read_texture_image(ResourceStream *rs, int *w, int *h) {
    if (!rs)
        return NULL;
    int comp;
    unsigned char *img = stbi_load_from_memory(rs->data, rs->size, w, h, &comp, 4);
    ResourceStream_free(rs);
    return img;
}

RenderEngine *RenderEngine_create(TexturePackList *texturePack, GameSettings *options) {
    RenderEngine *self = (RenderEngine *)calloc(1, sizeof(RenderEngine));
    self->texturePack = texturePack;
    self->options = options;

    return self;
}

void RenderEngine_destroy(RenderEngine *self) {
    for (int i = 0; i < RENDER_ENGINE_MAP_SIZE; i++) {
        TextureEntry *e = self->textureMap[i];
        while (e) {
            TextureEntry *next = e->next;
            R_deleteTexture(e->texId);
            free(e->name);
            free(e);
            e = next;
        }
    }
    for (int i = 0; i < TEX_NAME_MAP_SIZE; i++) {
        TexNameEntry *e = self->textureNameToImageMap[i];
        while (e) {
            TexNameEntry *next = e->next;
            free(e);
            e = next;
        }
    }
    for (int i = 0; i < URL_MAP_SIZE; i++) {
        UrlImageEntry *e = self->urlToImageDataMap[i];
        while (e) {
            UrlImageEntry *next = e->next;
            free(e->key);
            free(e);
            e = next;
        }
    }
    for (int i = 0; i < PIXEL_DATA_MAP_SIZE; i++) {
        PixelDataEntry *e = self->field_28151_c[i];
        while (e) {
            PixelDataEntry *next = e->next;
            free(e->key);
            free(e->value);
            free(e);
            e = next;
        }
    }
    free(self->textureList);
    free(self);
}

void RenderEngine_setupTextureRaw(RenderEngine *self, unsigned int texId, int width, int height,
                                  const unsigned char *rgba) {
    R_bindTexture(texId);
    if (RenderEngine_useMipmaps) {
        R_texParameteri(R_TEXTURE_MIN_FILTER, R_NEAREST_MIPMAP_LINEAR);
        R_texParameteri(R_TEXTURE_MAG_FILTER, R_NEAREST);
    } else {
        R_texParameteri(R_TEXTURE_MIN_FILTER, R_NEAREST);
        R_texParameteri(R_TEXTURE_MAG_FILTER, R_NEAREST);
    }
    if (self->blurTexture) {
        R_texParameteri(R_TEXTURE_MIN_FILTER, R_LINEAR);
        R_texParameteri(R_TEXTURE_MAG_FILTER, R_LINEAR);
    }
    if (self->clampTexture) {
        R_texParameteri(R_TEXTURE_WRAP_S, R_CLAMP);
        R_texParameteri(R_TEXTURE_WRAP_T, R_CLAMP);
    } else {
        R_texParameteri(R_TEXTURE_WRAP_S, R_REPEAT);
        R_texParameteri(R_TEXTURE_WRAP_T, R_REPEAT);
    }

    unsigned char *converted = NULL;
    const unsigned char *data = rgba;
    if (rgba && self->options && self->options->anaglyph) {
        int n = width * height * 4;
        converted = (unsigned char *)malloc((size_t)n);
        if (converted) {
            for (int i = 0; i < n; i += 4) {
                int r = rgba[i + 0], g = rgba[i + 1], b = rgba[i + 2];
                converted[i + 0] = (unsigned char)((r * 30 + g * 59 + b * 11) / 100);
                converted[i + 1] = (unsigned char)((r * 30 + g * 70) / 100);
                converted[i + 2] = (unsigned char)((r * 30 + b * 70) / 100);
                converted[i + 3] = rgba[i + 3];
            }
            data = converted;
        }
    }
    R_texImage2D(0, R_RGBA, width, height, R_RGBA, R_UNSIGNED_BYTE, data);

    if (RenderEngine_useMipmaps) {
        unsigned char *buf = (unsigned char *)malloc((size_t)(width * height * 4));
        memcpy(buf, data, (size_t)(width * height * 4));
        for (int var7 = 1; var7 <= 4; ++var7) {
            int var8 = width >> (var7 - 1);
            int var9 = width >> var7;
            int var10 = height >> var7;
            for (int var11 = 0; var11 < var9; ++var11) {
                for (int var12 = 0; var12 < var10; ++var12) {

#define GET_PX(i)                                                                                  \
    (((int)buf[(i) * 4 + 0] << 24) | ((int)buf[(i) * 4 + 1] << 16) |                               \
     ((int)buf[(i) * 4 + 2] << 8) | (int)buf[(i) * 4 + 3])
                    int var13 = GET_PX((var11 * 2 + 0) + (var12 * 2 + 0) * var8);
                    int var14 = GET_PX((var11 * 2 + 1) + (var12 * 2 + 0) * var8);
                    int var15 = GET_PX((var11 * 2 + 1) + (var12 * 2 + 1) * var8);
                    int var16 = GET_PX((var11 * 2 + 0) + (var12 * 2 + 1) * var8);
#undef GET_PX
                    int var17 = weightedAverageColor(weightedAverageColor(var13, var14),
                                                     weightedAverageColor(var15, var16));

                    int dst = (var11 + var12 * var9) * 4;
                    buf[dst + 0] = (unsigned char)((var17 >> 24) & 0xFF);
                    buf[dst + 1] = (unsigned char)((var17 >> 16) & 0xFF);
                    buf[dst + 2] = (unsigned char)((var17 >> 8) & 0xFF);
                    buf[dst + 3] = (unsigned char)(var17 & 0xFF);
                }
            }
            R_texImage2D(var7, R_RGBA, var9, var10, R_RGBA, R_UNSIGNED_BYTE, buf);
        }
        free(buf);
    }
    free(converted);
}

unsigned int RenderEngine_getTexture(RenderEngine *self, const char *name) {

    unsigned int id = texmap_get(self, name);
    if (id)
        return id;

    TexturePackBase *var2 = self->texturePack ? self->texturePack->selectedTexturePack : NULL;

    id = R_genTexture();

    int loaded = 0;
    if (name[0] == '#' && name[1] == '#') {

        int sw, sh;
        unsigned char *raw =
            var2 ? read_texture_image(var2->vtable->getResourceAsStream(var2, name + 2), &sw, &sh)
                 : NULL;
        if (raw) {
            int uw, uh;
            unsigned char *unwrapped = unwrapImageByColumns(raw, sw, sh, &uw, &uh);
            stbi_image_free(raw);
            if (unwrapped) {
                RenderEngine_setupTextureRaw(self, id, uw, uh, unwrapped);
                free(unwrapped);
                loaded = 1;
            }
        }
    } else if (strncmp(name, "%clamp%", 7) == 0) {

        self->clampTexture = 1;
        int w, h;
        unsigned char *img =
            var2 ? read_texture_image(var2->vtable->getResourceAsStream(var2, name + 7), &w, &h)
                 : NULL;
        if (img) {
            RenderEngine_setupTextureRaw(self, id, w, h, img);
            stbi_image_free(img);
            loaded = 1;
        }
        self->clampTexture = 0;
    } else if (strncmp(name, "%blur%", 6) == 0) {

        self->blurTexture = 1;
        int w, h;
        unsigned char *img =
            var2 ? read_texture_image(var2->vtable->getResourceAsStream(var2, name + 6), &w, &h)
                 : NULL;
        if (img) {
            RenderEngine_setupTextureRaw(self, id, w, h, img);
            stbi_image_free(img);
            loaded = 1;
        }
        self->blurTexture = 0;
    } else {

        ResourceStream *var7 = var2 ? var2->vtable->getResourceAsStream(var2, name) : NULL;
        if (var7 == NULL) {

            unsigned char miss[16] = {255, 0, 255, 255, 0,   0, 0,   255,
                                      0,   0, 0,   255, 255, 0, 255, 255};
            RenderEngine_setupTextureRaw(self, id, 2, 2, miss);
        } else {
            int w, h;
            unsigned char *img = read_texture_image(var7, &w, &h);
            if (img) {
                RenderEngine_setupTextureRaw(self, id, w, h, img);
                stbi_image_free(img);
            } else {
                unsigned char miss[16] = {255, 0, 255, 255, 0,   0, 0,   255,
                                          0,   0, 0,   255, 255, 0, 255, 255};
                RenderEngine_setupTextureRaw(self, id, 2, 2, miss);
            }
        }
        loaded = 1;
    }

    if (!loaded) {
        unsigned char miss[16] = {255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255};
        RenderEngine_setupTextureRaw(self, id, 2, 2, miss);
    }

    texmap_put(self, name, id);
    return id;
}

void RenderEngine_bindTexture(RenderEngine *self, int var1) {
    (void)self;
    if (var1 >= 0)
        R_bindTexture((unsigned int)var1);
}

int RenderEngine_allocateAndSetupTexture(RenderEngine *self, void *var1, int w, int h) {

    unsigned int var2;
    var2 = R_genTexture();

    if (w > 0 && h > 0) {
        RenderEngine_setupTextureRaw(self, var2, w, h, (unsigned char *)var1);
    }

    namemap_put(self, (int)var2, var1, w, h);
    return (int)var2;
}

void RenderEngine_deleteTexture(RenderEngine *self, int var1) {
    namemap_remove(self, var1);
    unsigned int tex = (unsigned int)var1;
    R_deleteTexture(tex);
}

void RenderEngine_func_28150_a(RenderEngine *self, const int *var1, int var2, int var3, int var4) {
    R_bindTexture((unsigned int)var4);
    if (RenderEngine_useMipmaps) {
        R_texParameteri(R_TEXTURE_MIN_FILTER, R_NEAREST_MIPMAP_LINEAR);
        R_texParameteri(R_TEXTURE_MAG_FILTER, R_NEAREST);
    } else {
        R_texParameteri(R_TEXTURE_MIN_FILTER, R_NEAREST);
        R_texParameteri(R_TEXTURE_MAG_FILTER, R_NEAREST);
    }
    if (self->blurTexture) {
        R_texParameteri(R_TEXTURE_MIN_FILTER, R_LINEAR);
        R_texParameteri(R_TEXTURE_MAG_FILTER, R_LINEAR);
    }
    if (self->clampTexture) {
        R_texParameteri(R_TEXTURE_WRAP_S, R_CLAMP);
        R_texParameteri(R_TEXTURE_WRAP_T, R_CLAMP);
    } else {
        R_texParameteri(R_TEXTURE_WRAP_S, R_REPEAT);
        R_texParameteri(R_TEXTURE_WRAP_T, R_REPEAT);
    }
    int len = var2 * var3;
    for (int var6 = 0; var6 < len; ++var6) {
        int var7 = (var1[var6] >> 24) & 255;
        int var8 = (var1[var6] >> 16) & 255;
        int var9 = (var1[var6] >> 8) & 255;
        int var10 = var1[var6] & 255;
        if (self->options != NULL && self->options->anaglyph) {
            int var11 = (var8 * 30 + var9 * 59 + var10 * 11) / 100;
            int var12 = (var8 * 30 + var9 * 70) / 100;
            int var13 = (var8 * 30 + var10 * 70) / 100;
            var8 = var11;
            var9 = var12;
            var10 = var13;
        }
        self->imageData[var6 * 4 + 0] = (unsigned char)var8;
        self->imageData[var6 * 4 + 1] = (unsigned char)var9;
        self->imageData[var6 * 4 + 2] = (unsigned char)var10;
        self->imageData[var6 * 4 + 3] = (unsigned char)var7;
    }
    R_texSubImage2D(0, 0, 0, var2, var3, R_RGBA, R_UNSIGNED_BYTE, self->imageData);
}

void RenderEngine_registerTextureFX(RenderEngine *self, TextureFX *var1) {
    if (self->textureListSize >= self->textureListCapacity) {
        self->textureListCapacity = self->textureListCapacity ? self->textureListCapacity * 2 : 16;
        self->textureList = (TextureFX **)realloc(self->textureList,
                                                  self->textureListCapacity * sizeof(TextureFX *));
    }
    self->textureList[self->textureListSize++] = var1;
    TextureFX_onTick(var1);
}

void RenderEngine_updateDynamicTextures(RenderEngine *self) {
    int var1;
    TextureFX *var2;
    int var3, var4;

    for (var1 = 0; var1 < self->textureListSize; ++var1) {
        var2 = self->textureList[var1];
        var2->anaglyphEnabled = self->options ? self->options->anaglyph : 0;
        TextureFX_onTick(var2);
        memcpy(self->imageData, var2->imageData, sizeof(var2->imageData));
        TextureFX_bindImage(var2, self);

        for (var3 = 0; var3 < var2->tileSize; ++var3) {
            for (var4 = 0; var4 < var2->tileSize; ++var4) {
                R_texSubImage2D(0, var2->iconIndex % 16 * 16 + var3 * 16,
                                var2->iconIndex / 16 * 16 + var4 * 16, 16, 16, R_RGBA,
                                R_UNSIGNED_BYTE, self->imageData);
                if (RenderEngine_useMipmaps) {
                    for (int var5 = 1; var5 <= 4; ++var5) {
                        int var6 = 16 >> (var5 - 1);
                        int var7 = 16 >> var5;
                        for (int var8 = 0; var8 < var7; ++var8) {
                            for (int var9 = 0; var9 < var7; ++var9) {
                                int i10 = *(int *)(self->imageData +
                                                   (var8 * 2 + 0 + (var9 * 2 + 0) * var6) * 4);
                                int i11 = *(int *)(self->imageData +
                                                   (var8 * 2 + 1 + (var9 * 2 + 0) * var6) * 4);
                                int i12 = *(int *)(self->imageData +
                                                   (var8 * 2 + 1 + (var9 * 2 + 1) * var6) * 4);
                                int i13 = *(int *)(self->imageData +
                                                   (var8 * 2 + 0 + (var9 * 2 + 1) * var6) * 4);
                                int i14 =
                                    averageColor(averageColor(i10, i11), averageColor(i12, i13));
                                *(int *)(self->imageData + (var8 + var9 * var7) * 4) = i14;
                            }
                        }
                        R_texSubImage2D(var5, var2->iconIndex % 16 * var7,
                                        var2->iconIndex / 16 * var7, var7, var7, R_RGBA,
                                        R_UNSIGNED_BYTE, self->imageData);
                    }
                }
            }
        }
    }

    for (var1 = 0; var1 < self->textureListSize; ++var1) {
        var2 = self->textureList[var1];
        if (var2->textureId > 0) {
            memcpy(self->imageData, var2->imageData, sizeof(var2->imageData));
            R_bindTexture((unsigned int)var2->textureId);
            R_texSubImage2D(0, 0, 0, 16, 16, R_RGBA, R_UNSIGNED_BYTE, self->imageData);
            if (RenderEngine_useMipmaps) {
                for (int var3b = 1; var3b <= 4; ++var3b) {
                    int var4b = 16 >> (var3b - 1);
                    int var5b = 16 >> var3b;
                    for (int var6b = 0; var6b < var5b; ++var6b) {
                        for (int var7b = 0; var7b < var5b; ++var7b) {
                            int i8 = *(int *)(self->imageData +
                                              (var6b * 2 + 0 + (var7b * 2 + 0) * var4b) * 4);
                            int i9 = *(int *)(self->imageData +
                                              (var6b * 2 + 1 + (var7b * 2 + 0) * var4b) * 4);
                            int i10 = *(int *)(self->imageData +
                                               (var6b * 2 + 1 + (var7b * 2 + 1) * var4b) * 4);
                            int i11 = *(int *)(self->imageData +
                                               (var6b * 2 + 0 + (var7b * 2 + 1) * var4b) * 4);
                            int i12 = averageColor(averageColor(i8, i9), averageColor(i10, i11));
                            *(int *)(self->imageData + (var6b + var7b * var5b) * 4) = i12;
                        }
                    }
                    R_texSubImage2D(var3b, 0, 0, var5b, var5b, R_RGBA, R_UNSIGNED_BYTE,
                                    self->imageData);
                }
            }
        }
    }
}

void RenderEngine_refreshTextures(RenderEngine *self) {

    for (int i = 0; i < TEX_NAME_MAP_SIZE; i++) {
        for (TexNameEntry *e = self->textureNameToImageMap[i]; e; e = e->next) {
            if (e->value != NULL && e->width > 0 && e->height > 0) {
                RenderEngine_setupTextureRaw(self, (unsigned int)e->key, e->width, e->height,
                                             (unsigned char *)e->value);
            }

            (void)namemap_find;
        }
    }

    for (int i = 0; i < URL_MAP_SIZE; i++) {
        for (UrlImageEntry *e = self->urlToImageDataMap[i]; e; e = e->next) {
            e->value->textureSetupComplete = 0;
        }
    }

    TexturePackBase *var1 = self->texturePack ? self->texturePack->selectedTexturePack : NULL;

    for (int i = 0; i < RENDER_ENGINE_MAP_SIZE; i++) {
        for (TextureEntry *e = self->textureMap[i]; e; e = e->next) {
            const char *var9 = e->name;
            int w = 0, h = 0;
            unsigned char *var4 = NULL;
            int var4_is_mallocd = 0;
            if (var9[0] == '#' && var9[1] == '#') {
                int sw, sh;
                unsigned char *raw =
                    var1 ? read_texture_image(var1->vtable->getResourceAsStream(var1, var9 + 2),
                                              &sw, &sh)
                         : NULL;
                if (raw) {
                    int uw, uh;
                    var4 = unwrapImageByColumns(raw, sw, sh, &uw, &uh);
                    stbi_image_free(raw);
                    if (var4) {
                        w = uw;
                        h = uh;
                        var4_is_mallocd = 1;
                    }
                }
            } else if (strncmp(var9, "%clamp%", 7) == 0) {
                self->clampTexture = 1;
                var4 = var1 ? read_texture_image(var1->vtable->getResourceAsStream(var1, var9 + 7),
                                                 &w, &h)
                            : NULL;
            } else if (strncmp(var9, "%blur%", 6) == 0) {
                self->blurTexture = 1;
                var4 = var1 ? read_texture_image(var1->vtable->getResourceAsStream(var1, var9 + 6),
                                                 &w, &h)
                            : NULL;
            } else {
                var4 =
                    var1 ? read_texture_image(var1->vtable->getResourceAsStream(var1, var9), &w, &h)
                         : NULL;
            }
            if (var4) {
                RenderEngine_setupTextureRaw(self, e->texId, w, h, var4);
                if (var4_is_mallocd) {
                    free(var4);
                } else {
                    stbi_image_free(var4);
                }
            }
            self->blurTexture = 0;
            self->clampTexture = 0;
        }
    }

    for (int i = 0; i < PIXEL_DATA_MAP_SIZE; i++) {
        for (PixelDataEntry *e = self->field_28151_c[i]; e; e = e->next) {
            const char *var9 = e->key;
            int w = 0, h = 0;
            unsigned char *var4 = NULL;
            int var4_is_mallocd = 0;
            if (var9[0] == '#' && var9[1] == '#') {
                int sw, sh;
                unsigned char *raw =
                    var1 ? read_texture_image(var1->vtable->getResourceAsStream(var1, var9 + 2),
                                              &sw, &sh)
                         : NULL;
                if (raw) {
                    int uw, uh;
                    var4 = unwrapImageByColumns(raw, sw, sh, &uw, &uh);
                    stbi_image_free(raw);
                    if (var4) {
                        w = uw;
                        h = uh;
                        var4_is_mallocd = 1;
                    }
                }
            } else if (strncmp(var9, "%clamp%", 7) == 0) {
                self->clampTexture = 1;
                var4 = var1 ? read_texture_image(var1->vtable->getResourceAsStream(var1, var9 + 7),
                                                 &w, &h)
                            : NULL;
            } else if (strncmp(var9, "%blur%", 6) == 0) {
                self->blurTexture = 1;
                var4 = var1 ? read_texture_image(var1->vtable->getResourceAsStream(var1, var9 + 6),
                                                 &w, &h)
                            : NULL;
            } else {
                var4 =
                    var1 ? read_texture_image(var1->vtable->getResourceAsStream(var1, var9), &w, &h)
                         : NULL;
            }
            if (var4) {

                func_28147_a(var4, w, h, e->value);
                if (var4_is_mallocd) {
                    free(var4);
                } else {
                    stbi_image_free(var4);
                }
            }
            self->blurTexture = 0;
            self->clampTexture = 0;
        }
    }
}

int RenderEngine_getTextureForDownloadableImage(RenderEngine *self, const char *var1,
                                                const char *var2) {
    ThreadDownloadImageData *var3 = urlmap_get(self, var1);
    if (var3 != NULL && var3->image != NULL && !var3->textureSetupComplete) {
        if (var3->textureName < 0) {

            var3->textureName = RenderEngine_allocateAndSetupTexture(
                self, var3->image, var3->imageWidth, var3->imageHeight);
        } else {

            if (var3->imageWidth > 0 && var3->imageHeight > 0) {
                RenderEngine_setupTextureRaw(self, (unsigned int)var3->textureName,
                                             var3->imageWidth, var3->imageHeight,
                                             (unsigned char *)var3->image);
            }
        }
        var3->textureSetupComplete = 1;
    }
    if (var3 != NULL && var3->textureName >= 0)
        return var3->textureName;
    if (var2 == NULL)
        return -1;
    return (int)RenderEngine_getTexture(self, var2);
}

ThreadDownloadImageData *RenderEngine_obtainImageData(RenderEngine *self, const char *var1,
                                                      ImageBuffer *var2) {
    ThreadDownloadImageData *var3 = urlmap_get(self, var1);
    if (var3 == NULL) {
        urlmap_put(self, var1, ThreadDownloadImageData_create(var1, var2));
    } else {
        ++var3->referenceCount;
        free(var2);
    }
    return var3;
}

static int *pixeldata_get(RenderEngine *self, const char *key) {
    unsigned int h = str_hash(key, PIXEL_DATA_MAP_SIZE);
    for (PixelDataEntry *e = self->field_28151_c[h]; e; e = e->next)
        if (!strcmp(e->key, key))
            return e->value;
    return NULL;
}
static void pixeldata_put(RenderEngine *self, const char *key, int *val) {
    unsigned int h = str_hash(key, PIXEL_DATA_MAP_SIZE);
    PixelDataEntry *e = (PixelDataEntry *)calloc(1, sizeof(PixelDataEntry));
    e->key = strdup(key);
    e->value = val;
    e->next = self->field_28151_c[h];
    self->field_28151_c[h] = e;
}

static int *func_28148_b(unsigned char *rgba, int w, int h) {
    int n = w * h;
    int *var4 = (int *)malloc((size_t)n * sizeof(int));
    if (!var4)
        return NULL;
    for (int i = 0; i < n; i++) {
        int r = rgba[i * 4 + 0];
        int g = rgba[i * 4 + 1];
        int b = rgba[i * 4 + 2];
        int a = rgba[i * 4 + 3];
        var4[i] = (int)(((unsigned int)a << 24) | ((unsigned int)r << 16) | ((unsigned int)g << 8) |
                        (unsigned int)b);
    }
    return var4;
}

static int *func_28147_a(unsigned char *rgba, int w, int h, int *var2) {
    int n = w * h;
    for (int i = 0; i < n; i++) {
        int r = rgba[i * 4 + 0];
        int g = rgba[i * 4 + 1];
        int b = rgba[i * 4 + 2];
        int a = rgba[i * 4 + 3];
        var2[i] = (int)(((unsigned int)a << 24) | ((unsigned int)r << 16) | ((unsigned int)g << 8) |
                        (unsigned int)b);
    }
    return var2;
}

static int *missing_pixel_data(void) {
    int *p = (int *)malloc(4 * sizeof(int));
    if (!p)
        return NULL;
    p[0] = (int)0xFFFF00FF;
    p[1] = (int)0xFF000000;
    p[2] = (int)0xFF000000;
    p[3] = (int)0xFFFF00FF;
    return p;
}

int *RenderEngine_func_28149_a(RenderEngine *self, const char *var1) {

    TexturePackBase *var2 = self->texturePack ? self->texturePack->selectedTexturePack : NULL;

    int *var3 = pixeldata_get(self, var1);
    if (var3 != NULL)
        return var3;

    int w = 0, h = 0;
    unsigned char *rgba = NULL;
    int rgba_is_mallocd = 0;

    if (var1[0] == '#' && var1[1] == '#') {

        int sw, sh;
        unsigned char *raw =
            var2 ? read_texture_image(var2->vtable->getResourceAsStream(var2, var1 + 2), &sw, &sh)
                 : NULL;
        if (raw) {
            int uw, uh;
            rgba = unwrapImageByColumns(raw, sw, sh, &uw, &uh);
            stbi_image_free(raw);
            if (rgba) {
                w = uw;
                h = uh;
                rgba_is_mallocd = 1;
            }
        }
    } else if (strncmp(var1, "%clamp%", 7) == 0) {

        self->clampTexture = 1;
        rgba = var2 ? read_texture_image(var2->vtable->getResourceAsStream(var2, var1 + 7), &w, &h)
                    : NULL;
        self->clampTexture = 0;
    } else if (strncmp(var1, "%blur%", 6) == 0) {

        self->blurTexture = 1;
        rgba = var2 ? read_texture_image(var2->vtable->getResourceAsStream(var2, var1 + 6), &w, &h)
                    : NULL;
        self->blurTexture = 0;
    } else {

        ResourceStream *var7 = var2 ? var2->vtable->getResourceAsStream(var2, var1) : NULL;
        if (var7 == NULL) {
            var3 = missing_pixel_data();
            if (var3)
                pixeldata_put(self, var1, var3);
            return var3;
        }
        rgba = read_texture_image(var7, &w, &h);
    }

    if (rgba) {

        var3 = func_28148_b(rgba, w, h);
        if (rgba_is_mallocd) {
            free(rgba);
        } else {
            stbi_image_free(rgba);
        }
    }

    if (!var3) {

        var3 = missing_pixel_data();
    }

    if (var3)
        pixeldata_put(self, var1, var3);
    return var3;
}

void RenderEngine_releaseImageData(RenderEngine *self, const char *var1) {
    ThreadDownloadImageData *var2 = urlmap_get(self, var1);
    if (var2 != NULL) {
        --var2->referenceCount;
        if (var2->referenceCount == 0) {
            if (var2->textureName >= 0) {
                RenderEngine_deleteTexture(self, var2->textureName);
            }
            urlmap_remove(self, var1);
            ThreadDownloadImageData_destroy(var2);
        }
    }
}
