#include "TileEntitySpecialRenderer.h"
#include "TileEntityRenderer.h"
#include "RenderEngine.h"

void TileEntitySpecialRenderer_bindTextureByName(TileEntitySpecialRenderer *self,
                                                 const char *var1) {
    RenderEngine *var2 = self->tileEntityRenderer->renderEngine;
    RenderEngine_bindTexture(var2, RenderEngine_getTexture(var2, var1));
}

void TileEntitySpecialRenderer_setTileEntityRenderer(TileEntitySpecialRenderer *self,
                                                     TileEntityRenderer *var1) {
    self->tileEntityRenderer = var1;
}

FontRenderer *TileEntitySpecialRenderer_getFontRenderer(TileEntitySpecialRenderer *self) {
    return TileEntityRenderer_getFontRenderer(self->tileEntityRenderer);
}

void TileEntitySpecialRenderer_func_31069_a_default(TileEntitySpecialRenderer *self, World *var1) {
    (void)self;
    (void)var1;
}
