#include "TexturePackBase.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void ResourceStream_free(ResourceStream *rs) {
    if (rs) {
        free(rs->data);
        free(rs);
    }
}

void TexturePackBase_func_6482_a(TexturePackBase *self) { (void)self; }

void TexturePackBase_closeTexturePackFile(TexturePackBase *self) { (void)self; }

void TexturePackBase_func_6485_a(TexturePackBase *self, struct Minecraft *var1) {
    (void)self;
    (void)var1;
}

void TexturePackBase_func_6484_b(TexturePackBase *self, struct Minecraft *var1) {
    (void)self;
    (void)var1;
}

void TexturePackBase_bindThumbnailTexture(TexturePackBase *self, struct Minecraft *var1) {
    (void)self;
    (void)var1;
}

ResourceStream *TexturePackBase_getResourceAsStream(TexturePackBase *self, const char *var1) {
    (void)self;
    if (var1 == NULL)
        return NULL;
    const char *path = (var1[0] == '/') ? var1 + 1 : var1;
    FILE *f = fopen(path, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0) {
        fclose(f);
        return NULL;
    }
    unsigned char *buf = (unsigned char *)malloc((size_t)sz);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    if ((long)fread(buf, 1, (size_t)sz, f) != sz) {
        free(buf);
        fclose(f);
        return NULL;
    }
    fclose(f);
    ResourceStream *rs = (ResourceStream *)calloc(1, sizeof(ResourceStream));
    rs->data = buf;
    rs->size = (int)sz;
    return rs;
}

void TexturePackBase_destroy_impl(TexturePackBase *self) {
    TexturePackBase_freeFields(self);
    free(self);
}

const TexturePackBaseVtable TexturePackBase_vtable = {
    TexturePackBase_func_6482_a,          TexturePackBase_closeTexturePackFile,
    TexturePackBase_func_6485_a,          TexturePackBase_func_6484_b,
    TexturePackBase_bindThumbnailTexture, TexturePackBase_getResourceAsStream,
    TexturePackBase_destroy_impl};

void TexturePackBase_init(TexturePackBase *self) {
    self->vtable = &TexturePackBase_vtable;
    self->texturePackFileName = NULL;
    self->firstDescriptionLine = NULL;
    self->secondDescriptionLine = NULL;
    self->field_6488_d = NULL;
}

void TexturePackBase_freeFields(TexturePackBase *self) {
    free(self->texturePackFileName);
    free(self->firstDescriptionLine);
    free(self->secondDescriptionLine);
    free(self->field_6488_d);
    self->texturePackFileName = NULL;
    self->firstDescriptionLine = NULL;
    self->secondDescriptionLine = NULL;
    self->field_6488_d = NULL;
}
