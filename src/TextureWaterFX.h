#ifndef TEXTURE_WATER_FX_H
#define TEXTURE_WATER_FX_H

#include "TextureFX.h"

typedef struct TextureWaterFX {
    TextureFX base;

    float *field_1158_g;
    float *field_1157_h;

    float field_1156_i[256];

    float field_1155_j[256];

    int tickCounter;
} TextureWaterFX;

TextureWaterFX *TextureWaterFX_create(void);
void TextureWaterFX_destroy(TextureWaterFX *self);

void TextureWaterFX_onTick(TextureFX *self);

extern const TextureFXVtable TextureWaterFX_vtable;

#endif
