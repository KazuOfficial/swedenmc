#include "Renderer.h"

#include "TileEntityRendererPiston.h"
#include "Block.h"
#include "Tessellator.h"
#include "RenderHelper.h"
#include "Minecraft.h"
#include <stdlib.h>

static void func_31070_a(TileEntityRendererPiston *self, TileEntityPiston *var1, double var2,
                         double var4, double var6, float var8) {
    Block *var9 = Block_blocksList[TileEntityPiston_getStoredBlockID(var1)];
    if (var9 != NULL && TileEntityPiston_func_31008_a(var1, var8) < 1.0f) {
        Tessellator *var10 = &Tessellator_instance;
        TileEntitySpecialRenderer_bindTextureByName(&self->base, "/terrain.png");
        RenderHelper_disableStandardItemLighting();
        R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
        R_enable(R_BLEND);
        R_disable(R_CULL_FACE);
        if (Minecraft_isAmbientOcclusionEnabled()) {
            R_shadeModel(R_SMOOTH);
        } else {
            R_shadeModel(R_FLAT);
        }
        Tessellator_startDrawingQuads(var10);
        Tessellator_setTranslationD(var10,
                                    (double)((float)var2 - (float)var1->base.xCoord +
                                             TileEntityPiston_func_31017_b(var1, var8)),
                                    (double)((float)var4 - (float)var1->base.yCoord +
                                             TileEntityPiston_func_31014_c(var1, var8)),
                                    (double)((float)var6 - (float)var1->base.zCoord +
                                             TileEntityPiston_func_31013_d(var1, var8)));
        Tessellator_setColorOpaque(var10, 1, 1, 1);
        if (var9 == Block_pistonExtension && TileEntityPiston_func_31008_a(var1, var8) < 0.5f) {
            RenderBlocks_func_31079_a(self->field_31071_b, var9, var1->base.xCoord,
                                      var1->base.yCoord, var1->base.zCoord, 0);
        } else if (TileEntityPiston_func_31012_k(var1) && !TileEntityPiston_func_31015_b(var1)) {
            BlockPistonExtension_func_31052_a_(Block_pistonExtension,
                                               BlockPistonBase_func_31040_i(var9));
            RenderBlocks_func_31079_a(self->field_31071_b, Block_pistonExtension, var1->base.xCoord,
                                      var1->base.yCoord, var1->base.zCoord,
                                      TileEntityPiston_func_31008_a(var1, var8) < 0.5f);
            BlockPistonExtension_func_31051_a(Block_pistonExtension);
            Tessellator_setTranslationD(var10, (double)((float)var2 - (float)var1->base.xCoord),
                                        (double)((float)var4 - (float)var1->base.yCoord),
                                        (double)((float)var6 - (float)var1->base.zCoord));
            RenderBlocks_func_31078_d(self->field_31071_b, var9, var1->base.xCoord,
                                      var1->base.yCoord, var1->base.zCoord);
        } else {
            RenderBlocks_func_31075_a(self->field_31071_b, var9, var1->base.xCoord,
                                      var1->base.yCoord, var1->base.zCoord);
        }
        Tessellator_setTranslationD(var10, 0.0, 0.0, 0.0);
        Tessellator_draw(var10);
        RenderHelper_enableStandardItemLighting();
    }
}

static void vtable_func_31069_a(TileEntitySpecialRenderer *base, World *var1) {
    TileEntityRendererPiston *self = (TileEntityRendererPiston *)base;

    if (self->field_31071_b == NULL) {
        self->field_31071_b = (RenderBlocks *)calloc(1, sizeof(RenderBlocks));
    }
    RenderBlocks_init(self->field_31071_b, (IBlockAccess *)var1);
}

static void vtable_renderTileEntityAt(TileEntitySpecialRenderer *base, TileEntity *var1,
                                      double var2, double var4, double var6, float var8) {
    TileEntityRendererPiston *self = (TileEntityRendererPiston *)base;
    func_31070_a(self, (TileEntityPiston *)var1, var2, var4, var6, var8);
}

static const TileEntitySpecialRendererVtable s_vtable = {
    .renderTileEntityAt = vtable_renderTileEntityAt,
    .func_31069_a = vtable_func_31069_a,
};

TileEntitySpecialRenderer *TileEntityRendererPiston_create(void) {
    TileEntityRendererPiston *self =
        (TileEntityRendererPiston *)calloc(1, sizeof(TileEntityRendererPiston));
    self->base.vtable = &s_vtable;
    self->base.tileEntityRenderer = NULL;
    self->field_31071_b = NULL;
    return &self->base;
}
