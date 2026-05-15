#ifndef TEXTURE_FLAMES_FX_H
#define TEXTURE_FLAMES_FX_H

#include "TextureFX.h"

typedef struct TextureFlamesFX {
    TextureFX base;

    float *field_1133_g;
    float *field_1132_h;
} TextureFlamesFX;

TextureFlamesFX *TextureFlamesFX_create(int var1);
void TextureFlamesFX_destroy(TextureFlamesFX *self);

void TextureFlamesFX_onTick(TextureFX *self);

extern const TextureFXVtable TextureFlamesFX_vtable;

#endif
