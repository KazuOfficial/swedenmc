#ifndef TEXTURE_LAVA_FX_H
#define TEXTURE_LAVA_FX_H

#include "TextureFX.h"

typedef struct TextureLavaFX {
    TextureFX base;

    float *field_1147_g;
    float *field_1146_h;

    float field_1145_i[256];

    float field_1144_j[256];
} TextureLavaFX;

TextureLavaFX *TextureLavaFX_create(void);
void TextureLavaFX_destroy(TextureLavaFX *self);

void TextureLavaFX_onTick(TextureFX *self);

extern const TextureFXVtable TextureLavaFX_vtable;

#endif
