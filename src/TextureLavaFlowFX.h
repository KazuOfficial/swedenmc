#ifndef TEXTURE_LAVA_FLOW_FX_H
#define TEXTURE_LAVA_FLOW_FX_H

#include "TextureFX.h"

typedef struct TextureLavaFlowFX {
    TextureFX base;

    float *field_1143_g;
    float *field_1142_h;

    float field_1141_i[256];

    float field_1140_j[256];

    int field_1139_k;
} TextureLavaFlowFX;

TextureLavaFlowFX *TextureLavaFlowFX_create(void);
void TextureLavaFlowFX_destroy(TextureLavaFlowFX *self);

void TextureLavaFlowFX_onTick(TextureFX *self);

extern const TextureFXVtable TextureLavaFlowFX_vtable;

#endif
