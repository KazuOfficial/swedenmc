#include "TileEntitySignRenderer.h"
#include "Renderer.h"
#include "Block.h"
#include "World.h"
#include "FontRenderer.h"
#include <stdlib.h>
#include <string.h>

static void renderTileEntitySignAt(TileEntitySignRenderer *self, TileEntitySign *var1, double var2,
                                   double var4, double var6, float var8) {
    (void)var8;

    Block *var9 = NULL;
    if (var1->base.worldObj != NULL) {
        int id = World_getBlockId(var1->base.worldObj, var1->base.xCoord, var1->base.yCoord,
                                  var1->base.zCoord);
        var9 = Block_blocksList[id];
    }
    R_pushMatrix();
    float var10 = 2.0f / 3.0f;
    float var12;
    if (var9 == Block_signPost) {
        R_translatef((float)var2 + 0.5f, (float)var4 + 12.0f / 16.0f * var10, (float)var6 + 0.5f);
        float var11 = (float)(TileEntity_getBlockMetadata(&var1->base) * 360) / 16.0f;
        R_rotatef(-var11, 0.0f, 1.0f, 0.0f);
        self->signModel.signStick.showModel = 1;
    } else {
        int var16 = TileEntity_getBlockMetadata(&var1->base);
        var12 = 0.0f;
        if (var16 == 2) {
            var12 = 180.0f;
        }
        if (var16 == 4) {
            var12 = 90.0f;
        }
        if (var16 == 5) {
            var12 = -90.0f;
        }
        R_translatef((float)var2 + 0.5f, (float)var4 + 12.0f / 16.0f * var10, (float)var6 + 0.5f);
        R_rotatef(-var12, 0.0f, 1.0f, 0.0f);
        R_translatef(0.0f, -(5.0f / 16.0f), -(7.0f / 16.0f));
        self->signModel.signStick.showModel = 0;
    }
    TileEntitySpecialRenderer_bindTextureByName(&self->base, "/item/sign.png");
    R_pushMatrix();
    R_scalef(var10, -var10, -var10);
    SignModel_func_887_a(&self->signModel);
    R_popMatrix();
    FontRenderer *var17 = TileEntitySpecialRenderer_getFontRenderer(&self->base);
    var12 = (float)(1.0 / 60.0) * var10;
    R_translatef(0.0f, 0.5f * var10, 0.07f * var10);
    R_scalef(var12, -var12, var12);
    R_normal3f(0.0f, 0.0f, -1.0f * var12);
    R_depthMask(R_FALSE);
    int var13 = 0;
    for (int var14 = 0; var14 < SIGN_LINES; ++var14) {

        char buf[SIGN_LINE_LEN + 8];
        const char *var15;
        if (var14 == var1->lineBeingEdited) {

            snprintf(buf, sizeof(buf), "> %s <", var1->signText[var14]);
            var15 = buf;
        } else {
            var15 = var1->signText[var14];
        }

        int xOff = -FontRenderer_getStringWidth(var17, var15) / 2;
        int yOff = var14 * 10 - SIGN_LINES * 5;
        FontRenderer_drawString(var17, var15, xOff, yOff, var13);
    }
    R_depthMask(R_TRUE);
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    R_popMatrix();
}

static void vtable_renderTileEntityAt(TileEntitySpecialRenderer *base, TileEntity *var1,
                                      double var2, double var4, double var6, float var8) {
    TileEntitySignRenderer *self = (TileEntitySignRenderer *)base;
    renderTileEntitySignAt(self, (TileEntitySign *)var1, var2, var4, var6, var8);
}

static const TileEntitySpecialRendererVtable s_vtable = {
    .renderTileEntityAt = vtable_renderTileEntityAt,
    .func_31069_a = TileEntitySpecialRenderer_func_31069_a_default,
};

TileEntitySpecialRenderer *TileEntitySignRenderer_create(void) {
    TileEntitySignRenderer *self =
        (TileEntitySignRenderer *)calloc(1, sizeof(TileEntitySignRenderer));
    self->base.vtable = &s_vtable;
    self->base.tileEntityRenderer = NULL;
    SignModel_init(&self->signModel);
    return &self->base;
}
