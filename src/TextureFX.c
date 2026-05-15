#include "Renderer.h"

#include "TextureFX.h"
#include "RenderEngine.h"
#include <stdlib.h>

static void TextureFX_onTick_impl(TextureFX *self) { (void)self; }

const TextureFXVtable TextureFX_vtable = {TextureFX_onTick_impl};

TextureFX *TextureFX_create(int iconIndex) {
    TextureFX *self = (TextureFX *)calloc(1, sizeof(TextureFX));
    self->vtable = &TextureFX_vtable;
    self->iconIndex = iconIndex;

    self->tileSize = 1;
    return self;
}

void TextureFX_destroy(TextureFX *self) { free(self); }

void TextureFX_onTick(TextureFX *self) { self->vtable->onTick(self); }

void TextureFX_bindImage(TextureFX *self, struct RenderEngine *var1) {
    if (self->tileImage == 0) {
        R_bindTexture(RenderEngine_getTexture(var1, "/terrain.png"));
    } else if (self->tileImage == 1) {
        R_bindTexture(RenderEngine_getTexture(var1, "/gui/items.png"));
    }
}
