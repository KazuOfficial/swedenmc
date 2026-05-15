#include "TexturePackDefault.h"
#include "Renderer.h"
#include "Minecraft.h"
#include "RenderEngine.h"
#include <stdlib.h>
#include <string.h>
#include "../lib/stb_image.h"

static void TexturePackDefault_func_6484_b(TexturePackBase *base, struct Minecraft *var1) {
    TexturePackDefault *self = (TexturePackDefault *)base;
    if (self->texturePackThumbnail != NULL) {
        RenderEngine_deleteTexture(var1->renderEngine, self->texturePackName);
    }
}

static void TexturePackDefault_bindThumbnailTexture(TexturePackBase *base, struct Minecraft *var1) {
    TexturePackDefault *self = (TexturePackDefault *)base;
    if (self->texturePackThumbnail != NULL && self->texturePackName < 0) {
        self->texturePackName =
            RenderEngine_allocateAndSetupTexture(var1->renderEngine, self->texturePackThumbnail,
                                                 self->thumbnailWidth, self->thumbnailHeight);
    }
    if (self->texturePackThumbnail != NULL) {
        RenderEngine_bindTexture(var1->renderEngine, (unsigned int)self->texturePackName);
    } else {
        R_bindTexture(RenderEngine_getTexture(var1->renderEngine, "/gui/unknown_pack.png"));
    }
}

static void TexturePackDefault_destroy_impl(TexturePackBase *base) {
    TexturePackDefault *self = (TexturePackDefault *)base;
    TexturePackDefault_destroy(self);
}

const TexturePackBaseVtable TexturePackDefault_vtable = {TexturePackBase_func_6482_a,
                                                         TexturePackBase_closeTexturePackFile,
                                                         TexturePackBase_func_6485_a,
                                                         TexturePackDefault_func_6484_b,
                                                         TexturePackDefault_bindThumbnailTexture,
                                                         TexturePackBase_getResourceAsStream,
                                                         TexturePackDefault_destroy_impl};

TexturePackDefault *TexturePackDefault_create(void) {
    TexturePackDefault *self = (TexturePackDefault *)calloc(1, sizeof(TexturePackDefault));
    TexturePackBase_init(&self->base);
    self->base.vtable = &TexturePackDefault_vtable;
    self->base.texturePackFileName = strdup("Default");
    self->base.firstDescriptionLine = strdup("The default look of Minecraft");

    {
        int w, h, comp;
        unsigned char *img = stbi_load("pack.png", &w, &h, &comp, 4);
        if (img) {
            self->texturePackThumbnail = img;
            self->thumbnailWidth = w;
            self->thumbnailHeight = h;
        }
    }

    self->texturePackName = -1;
    return self;
}

void TexturePackDefault_destroy(TexturePackDefault *self) {
    TexturePackBase_freeFields(&self->base);
    free(self);
}
