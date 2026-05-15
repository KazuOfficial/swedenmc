#ifndef TEXTURE_PORTAL_FX_H
#define TEXTURE_PORTAL_FX_H

#include "TextureFX.h"

typedef struct TexturePortalFX {
    TextureFX base;

    int portalTickCounter;

    signed char portalTextureData[32][1024];
} TexturePortalFX;

TexturePortalFX *TexturePortalFX_create(void);
void TexturePortalFX_destroy(TexturePortalFX *self);

void TexturePortalFX_onTick(TextureFX *self);

extern const TextureFXVtable TexturePortalFX_vtable;

#endif
