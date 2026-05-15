#ifndef TEXTURE_WATCH_FX_H
#define TEXTURE_WATCH_FX_H

#include "TextureFX.h"

struct Minecraft;

typedef struct TextureWatchFX {
    TextureFX base;

    struct Minecraft *mc;

    int watchIconImageData[256];

    int dialImageData[256];

    double field_4222_j;

    double field_4221_k;
} TextureWatchFX;

TextureWatchFX *TextureWatchFX_create(struct Minecraft *var1);
void TextureWatchFX_destroy(TextureWatchFX *self);

void TextureWatchFX_onTick(TextureFX *self);

extern const TextureFXVtable TextureWatchFX_vtable;

#endif
