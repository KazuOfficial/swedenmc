#ifndef TEXTURE_PACK_CUSTOM_H
#define TEXTURE_PACK_CUSTOM_H

#include "TexturePackBase.h"

typedef struct TexturePackCustom {
    TexturePackBase base;

    void *texturePackZipFile;

    int texturePackName;

    void *texturePackThumbnail;
    int thumbnailWidth;
    int thumbnailHeight;

    char *texturePackFile;
} TexturePackCustom;

TexturePackCustom *TexturePackCustom_create(const char *filePath);
void TexturePackCustom_destroy(TexturePackCustom *self);

extern const TexturePackBaseVtable TexturePackCustom_vtable;

#endif
