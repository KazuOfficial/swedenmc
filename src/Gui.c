#include "Renderer.h"

#include "Gui.h"

void Gui_init(Gui *self) { self->zLevel = 0.0F; }

void Gui_func_27100_a(Gui *self, int var1, int var2, int var3, int var4) {
    if (var2 < var1) {
        int var5 = var1;
        var1 = var2;
        var2 = var5;
    }
    Gui_drawRect(self, var1, var3, var2 + 1, var3 + 1, var4);
}

void Gui_func_27099_b(Gui *self, int var1, int var2, int var3, int var4) {
    if (var3 < var2) {
        int var5 = var2;
        var2 = var3;
        var3 = var5;
    }
    Gui_drawRect(self, var1, var2 + 1, var1 + 1, var3, var4);
}

void Gui_drawRect(Gui *self, int var1, int var2, int var3, int var4, int var5) {
    (void)self;
    int var6;
    if (var1 < var3) {
        var6 = var1;
        var1 = var3;
        var3 = var6;
    }
    if (var2 < var4) {
        var6 = var2;
        var2 = var4;
        var4 = var6;
    }
    float var11 = (float)(var5 >> 24 & 255) / 255.0F;
    float var7 = (float)(var5 >> 16 & 255) / 255.0F;
    float var8 = (float)(var5 >> 8 & 255) / 255.0F;
    float var9 = (float)(var5 & 255) / 255.0F;
    Tessellator *var10 = &Tessellator_instance;
    R_enable(R_BLEND);
    R_disable(R_TEXTURE_2D);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_color4f(var7, var8, var9, var11);
    Tessellator_startDrawingQuads(var10);
    Tessellator_addVertex(var10, (double)var1, (double)var4, 0.0);
    Tessellator_addVertex(var10, (double)var3, (double)var4, 0.0);
    Tessellator_addVertex(var10, (double)var3, (double)var2, 0.0);
    Tessellator_addVertex(var10, (double)var1, (double)var2, 0.0);
    Tessellator_draw(var10);
    R_enable(R_TEXTURE_2D);
    R_disable(R_BLEND);
}

void Gui_drawGradientRect(Gui *self, int var1, int var2, int var3, int var4, int var5, int var6) {
    (void)self;
    float var7 = (float)(var5 >> 24 & 255) / 255.0F;
    float var8 = (float)(var5 >> 16 & 255) / 255.0F;
    float var9 = (float)(var5 >> 8 & 255) / 255.0F;
    float var10 = (float)(var5 & 255) / 255.0F;
    float var11 = (float)(var6 >> 24 & 255) / 255.0F;
    float var12 = (float)(var6 >> 16 & 255) / 255.0F;
    float var13 = (float)(var6 >> 8 & 255) / 255.0F;
    float var14 = (float)(var6 & 255) / 255.0F;
    R_disable(R_TEXTURE_2D);
    R_enable(R_BLEND);
    R_disable(R_ALPHA_TEST);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    R_shadeModel(R_SMOOTH);
    Tessellator *var15 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var15);
    Tessellator_setColorRGBA_F(var15, var8, var9, var10, var7);
    Tessellator_addVertex(var15, (double)var3, (double)var2, 0.0);
    Tessellator_addVertex(var15, (double)var1, (double)var2, 0.0);
    Tessellator_setColorRGBA_F(var15, var12, var13, var14, var11);
    Tessellator_addVertex(var15, (double)var1, (double)var4, 0.0);
    Tessellator_addVertex(var15, (double)var3, (double)var4, 0.0);
    Tessellator_draw(var15);
    R_shadeModel(R_FLAT);
    R_disable(R_BLEND);
    R_enable(R_ALPHA_TEST);
    R_enable(R_TEXTURE_2D);
}

void Gui_drawCenteredString(Gui *self, FontRenderer *var1, const char *var2, int var3, int var4,
                            int var5) {
    (void)self;
    FontRenderer_drawStringWithShadow(
        var1, var2, var3 - FontRenderer_getStringWidth(var1, var2) / 2, var4, var5);
}

void Gui_drawString(Gui *self, FontRenderer *var1, const char *var2, int var3, int var4, int var5) {
    (void)self;
    FontRenderer_drawStringWithShadow(var1, var2, var3, var4, var5);
}

void Gui_drawTexturedModalRect(Gui *self, int var1, int var2, int var3, int var4, int var5,
                               int var6) {
    float var7 = 0.00390625F;
    float var8 = 0.00390625F;
    Tessellator *var9 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var9);
    Tessellator_addVertexWithUV(var9, (double)(var1 + 0), (double)(var2 + var6),
                                (double)self->zLevel, (double)((float)(var3 + 0) * var7),
                                (double)((float)(var4 + var6) * var8));
    Tessellator_addVertexWithUV(var9, (double)(var1 + var5), (double)(var2 + var6),
                                (double)self->zLevel, (double)((float)(var3 + var5) * var7),
                                (double)((float)(var4 + var6) * var8));
    Tessellator_addVertexWithUV(var9, (double)(var1 + var5), (double)(var2 + 0),
                                (double)self->zLevel, (double)((float)(var3 + var5) * var7),
                                (double)((float)(var4 + 0) * var8));
    Tessellator_addVertexWithUV(var9, (double)(var1 + 0), (double)(var2 + 0), (double)self->zLevel,
                                (double)((float)(var3 + 0) * var7),
                                (double)((float)(var4 + 0) * var8));
    Tessellator_draw(var9);
}
