#include "Renderer.h"

#include "TexturePackCustom.h"
#include "Minecraft.h"
#include "RenderEngine.h"
#include <stdlib.h>
#include <string.h>
#ifndef __EMSCRIPTEN__
#include <minizip/unzip.h>
#include "../lib/stb_image.h"
#endif

#ifdef __EMSCRIPTEN__

static void tp_noop(TexturePackBase *b, struct Minecraft *mc) {
    (void)b;
    (void)mc;
}
static void tp_noop1(TexturePackBase *b) { (void)b; }
static ResourceStream *tp_nostream(TexturePackBase *b, const char *p) {
    (void)b;
    (void)p;
    return NULL;
}
static void tp_destroy_impl(TexturePackBase *b) {
    TexturePackCustom_destroy((TexturePackCustom *)b);
}
const TexturePackBaseVtable TexturePackCustom_vtable = {
    tp_noop1, tp_noop1, tp_noop, tp_noop, tp_noop, tp_nostream, tp_destroy_impl};
#else

static unsigned char *zip_read_entry(unzFile zf, const char *entryName, size_t *out_size) {
    if (unzLocateFile(zf, entryName, 0) != UNZ_OK)
        return NULL;
    if (unzOpenCurrentFile(zf) != UNZ_OK)
        return NULL;
    unz_file_info fi;
    if (unzGetCurrentFileInfo(zf, &fi, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK) {
        unzCloseCurrentFile(zf);
        return NULL;
    }
    size_t sz = (size_t)fi.uncompressed_size;
    unsigned char *buf = (unsigned char *)malloc(sz + 1);
    if (!buf) {
        unzCloseCurrentFile(zf);
        return NULL;
    }
    int got = unzReadCurrentFile(zf, buf, (unsigned)sz);
    unzCloseCurrentFile(zf);
    if (got < 0) {
        free(buf);
        return NULL;
    }
    buf[sz] = '\0';
    if (out_size)
        *out_size = (size_t)got;
    return buf;
}

static char *truncateString(const char *var1) {
    if (var1 == NULL)
        return NULL;
    size_t len = strlen(var1);
    if (len > 34)
        len = 34;
    char *result = (char *)malloc(len + 1);
    memcpy(result, var1, len);
    result[len] = '\0';
    return result;
}

static void TexturePackCustom_func_6485_a(TexturePackBase *base, struct Minecraft *var1) {
    (void)var1;
    TexturePackCustom *self = (TexturePackCustom *)base;

    unzFile var2 = NULL;
    var2 = unzOpen(self->texturePackFile);
    if (var2 == NULL)
        return;

    {
        size_t sz = 0;
        unsigned char *buf = zip_read_entry(var2, "pack.txt", &sz);
        if (buf) {

            char *line1 = (char *)buf;
            char *newline = strchr(line1, '\n');
            char *line2 = NULL;
            if (newline) {
                *newline = '\0';
                line2 = newline + 1;

                size_t l1 = strlen(line1);
                if (l1 > 0 && line1[l1 - 1] == '\r')
                    line1[l1 - 1] = '\0';
                char *nl2 = strchr(line2, '\n');
                if (nl2) {
                    *nl2 = '\0';
                    size_t l2 = strlen(line2);
                    if (l2 > 0 && line2[l2 - 1] == '\r')
                        line2[l2 - 1] = '\0';
                }
            }
            free(base->firstDescriptionLine);
            base->firstDescriptionLine = truncateString(line1);
            free(base->secondDescriptionLine);
            base->secondDescriptionLine = truncateString(line2);
            free(buf);
        }
    }

    {
        size_t sz = 0;
        unsigned char *pngbuf = zip_read_entry(var2, "pack.png", &sz);
        if (pngbuf) {
            int w, h, comp;
            unsigned char *img = stbi_load_from_memory(pngbuf, (int)sz, &w, &h, &comp, 4);
            free(pngbuf);
            if (img) {
                stbi_image_free(self->texturePackThumbnail);
                self->texturePackThumbnail = img;
                self->thumbnailWidth = w;
                self->thumbnailHeight = h;
            }
        }
    }

    unzClose(var2);
}

static void TexturePackCustom_func_6484_b(TexturePackBase *base, struct Minecraft *var1) {
    TexturePackCustom *self = (TexturePackCustom *)base;
    if (self->texturePackThumbnail != NULL) {
        RenderEngine_deleteTexture(var1->renderEngine, self->texturePackName);
    }
    base->vtable->closeTexturePackFile(base);
}

static void TexturePackCustom_bindThumbnailTexture(TexturePackBase *base, struct Minecraft *var1) {
    TexturePackCustom *self = (TexturePackCustom *)base;
    if (self->texturePackThumbnail != NULL && self->texturePackName < 0) {
        self->texturePackName =
            RenderEngine_allocateAndSetupTexture(var1->renderEngine, self->texturePackThumbnail,
                                                 self->thumbnailWidth, self->thumbnailHeight);
    }
    if (self->texturePackThumbnail != NULL) {
        RenderEngine_bindTexture(var1->renderEngine, (unsigned int)self->texturePackName);
    } else {
        R_bindTexture(RenderEngine_getTexture(var1->renderEngine, "/gui/unknown_pack.png"));
    }
}

static void TexturePackCustom_func_6482_a(TexturePackBase *base) {

    TexturePackCustom *self = (TexturePackCustom *)base;
    if (self->texturePackZipFile != NULL) {
        unzClose((unzFile)self->texturePackZipFile);
        self->texturePackZipFile = NULL;
    }
    self->texturePackZipFile = (void *)unzOpen(self->texturePackFile);
}

static void TexturePackCustom_closeTexturePackFile(TexturePackBase *base) {

    TexturePackCustom *self = (TexturePackCustom *)base;
    if (self->texturePackZipFile != NULL) {
        unzClose((unzFile)self->texturePackZipFile);
        self->texturePackZipFile = NULL;
    }
}

static ResourceStream *TexturePackCustom_getResourceAsStream(TexturePackBase *base,
                                                             const char *var1) {
    TexturePackCustom *self = (TexturePackCustom *)base;

    const char *entryName = (var1 && var1[0] == '/') ? var1 + 1 : var1;
    if (self->texturePackZipFile != NULL) {

        size_t sz = 0;
        unsigned char *buf = zip_read_entry((unzFile)self->texturePackZipFile, entryName, &sz);
        if (buf) {
            ResourceStream *rs = (ResourceStream *)calloc(1, sizeof(ResourceStream));
            rs->data = buf;
            rs->size = (int)sz;
            return rs;
        }
    }

    return TexturePackBase_getResourceAsStream(base, var1);
}

static void TexturePackCustom_destroy_impl(TexturePackBase *base) {
    TexturePackCustom *self = (TexturePackCustom *)base;
    TexturePackCustom_destroy(self);
}

const TexturePackBaseVtable TexturePackCustom_vtable = {
    TexturePackCustom_func_6482_a,          TexturePackCustom_closeTexturePackFile,
    TexturePackCustom_func_6485_a,          TexturePackCustom_func_6484_b,
    TexturePackCustom_bindThumbnailTexture, TexturePackCustom_getResourceAsStream,
    TexturePackCustom_destroy_impl};

#endif

TexturePackCustom *TexturePackCustom_create(const char *filePath) {
    TexturePackCustom *self = (TexturePackCustom *)calloc(1, sizeof(TexturePackCustom));
    TexturePackBase_init(&self->base);
    self->base.vtable = &TexturePackCustom_vtable;

    const char *name = strrchr(filePath, '/');
    name = name ? name + 1 : filePath;
    self->base.texturePackFileName = strdup(name);
    self->texturePackFile = strdup(filePath);

    self->texturePackName = -1;
    return self;
}

void TexturePackCustom_destroy(TexturePackCustom *self) {
    free(self->texturePackFile);
    TexturePackBase_freeFields(&self->base);
    free(self);
}
