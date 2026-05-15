#ifndef TEXTURE_WATER_FLOW_FX_H
#define TEXTURE_WATER_FLOW_FX_H

#include "TextureFX.h"

typedef struct TextureWaterFlowFX {
    TextureFX base;

    float *field_1138_g;
    float *field_1137_h;

    float field_1136_i[256];

    float field_1135_j[256];

    int field_1134_k;
} TextureWaterFlowFX;

TextureWaterFlowFX *TextureWaterFlowFX_create(void);
void TextureWaterFlowFX_destroy(TextureWaterFlowFX *self);

void TextureWaterFlowFX_onTick(TextureFX *self);

extern const TextureFXVtable TextureWaterFlowFX_vtable;

#endif
