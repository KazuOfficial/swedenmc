#ifndef TEXTURE_PACK_BASE_H
#define TEXTURE_PACK_BASE_H

#include <stddef.h>

struct Minecraft;
typedef struct TexturePackBase TexturePackBase;

typedef struct {
    unsigned char *data;
    int size;
} ResourceStream;

void ResourceStream_free(ResourceStream *rs);

typedef struct TexturePackBaseVtable {

    void (*func_6482_a)(TexturePackBase *self);

    void (*closeTexturePackFile)(TexturePackBase *self);

    void (*func_6485_a)(TexturePackBase *self, struct Minecraft *var1);

    void (*func_6484_b)(TexturePackBase *self, struct Minecraft *var1);

    void (*bindThumbnailTexture)(TexturePackBase *self, struct Minecraft *var1);

    ResourceStream *(*getResourceAsStream)(TexturePackBase *self, const char *var1);

    void (*destroy)(TexturePackBase *self);
} TexturePackBaseVtable;

struct TexturePackBase {
    const TexturePackBaseVtable *vtable;

    char *texturePackFileName;

    char *firstDescriptionLine;

    char *secondDescriptionLine;

    char *field_6488_d;
};

void TexturePackBase_func_6482_a(TexturePackBase *self);
void TexturePackBase_closeTexturePackFile(TexturePackBase *self);
void TexturePackBase_func_6485_a(TexturePackBase *self, struct Minecraft *var1);
void TexturePackBase_func_6484_b(TexturePackBase *self, struct Minecraft *var1);
void TexturePackBase_bindThumbnailTexture(TexturePackBase *self, struct Minecraft *var1);
ResourceStream *TexturePackBase_getResourceAsStream(TexturePackBase *self, const char *var1);
void TexturePackBase_destroy_impl(TexturePackBase *self);

void TexturePackBase_init(TexturePackBase *self);

void TexturePackBase_freeFields(TexturePackBase *self);

extern const TexturePackBaseVtable TexturePackBase_vtable;

#endif
