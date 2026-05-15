#ifndef TEXTURE_COMPASS_FX_H
#define TEXTURE_COMPASS_FX_H

#include "TextureFX.h"

struct Minecraft;

typedef struct TextureCompassFX {
    TextureFX base;

    struct Minecraft *mc;

    int compassIconImageData[256];

    double field_4229_i;

    double field_4228_j;
} TextureCompassFX;

TextureCompassFX *TextureCompassFX_create(struct Minecraft *var1);
void TextureCompassFX_destroy(TextureCompassFX *self);

void TextureCompassFX_onTick(TextureFX *self);

extern const TextureFXVtable TextureCompassFX_vtable;

#endif
