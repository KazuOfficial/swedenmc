#ifndef TEXTURE_FX_H
#define TEXTURE_FX_H

struct RenderEngine;
typedef struct TextureFX TextureFX;

typedef struct TextureFXVtable {
    void (*onTick)(TextureFX *self);
} TextureFXVtable;

struct TextureFX {
    const TextureFXVtable *vtable;

    signed char imageData[1024];

    int iconIndex;

    int anaglyphEnabled;

    int textureId;

    int tileSize;

    int tileImage;
};

TextureFX *TextureFX_create(int iconIndex);
void TextureFX_destroy(TextureFX *self);

void TextureFX_onTick(TextureFX *self);

void TextureFX_bindImage(TextureFX *self, struct RenderEngine *var1);

extern const TextureFXVtable TextureFX_vtable;

#endif
