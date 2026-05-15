#ifndef TEXTURE_PACK_DEFAULT_H
#define TEXTURE_PACK_DEFAULT_H

#include "TexturePackBase.h"

typedef struct TexturePackDefault {
    TexturePackBase base;

    int texturePackName;

    void *texturePackThumbnail;
    int thumbnailWidth;
    int thumbnailHeight;
} TexturePackDefault;

TexturePackDefault *TexturePackDefault_create(void);
void TexturePackDefault_destroy(TexturePackDefault *self);

extern const TexturePackBaseVtable TexturePackDefault_vtable;

#endif
