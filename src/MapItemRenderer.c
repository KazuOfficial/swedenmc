#include "Renderer.h"

#include "MapItemRenderer.h"
#include "RenderEngine.h"
#include "FontRenderer.h"
#include "GameSettings.h"
#include "MapColor.h"
#include "MapData.h"
#include "Tessellator.h"
#include "EntityPlayer.h"
#include <string.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void MapItemRenderer_init(MapItemRenderer *self, struct FontRenderer *fontRenderer,
                          struct GameSettings *gameSettings, struct RenderEngine *renderEngine) {
    self->field_28161_c = gameSettings;
    self->field_28160_d = fontRenderer;
    self->field_28158_b = RenderEngine_allocateAndSetupTexture(renderEngine, NULL, 128, 128);
    for (int var4 = 0; var4 < 16384; ++var4) {
        self->field_28159_a[var4] = 0;
    }
}

void MapItemRenderer_func_28157_a(MapItemRenderer *self, struct EntityPlayer *var1,
                                  struct RenderEngine *var2, struct MapData *var3) {
    (void)var1;

    for (int var4 = 0; var4 < 16384; ++var4) {
        int8_t var5 = var3->field_28176_f[var4];
        if (var5 / 4 == 0) {
            self->field_28159_a[var4] = ((var4 + var4 / 128 & 1) * 8 + 16) << 24;
        } else {
            int var6 = MapColor_mapColorArray[var5 / 4]->colorValue;
            int var7 = var5 & 3;
            short var8 = 220;
            if (var7 == 2) {
                var8 = 255;
            }
            if (var7 == 0) {
                var8 = 180;
            }

            int var9 = (var6 >> 16 & 255) * var8 / 255;
            int var10 = (var6 >> 8 & 255) * var8 / 255;
            int var11 = (var6 & 255) * var8 / 255;
            if (self->field_28161_c->anaglyph) {
                int var12 = (var9 * 30 + var10 * 59 + var11 * 11) / 100;
                int var13 = (var9 * 30 + var10 * 70) / 100;
                int var14 = (var9 * 30 + var11 * 70) / 100;
                var9 = var12;
                var10 = var13;
                var11 = var14;
            }

            self->field_28159_a[var4] = -16777216 | var9 << 16 | var10 << 8 | var11;
        }
    }

    RenderEngine_func_28150_a(var2, self->field_28159_a, 128, 128, self->field_28158_b);
    int8_t var15 = 0;
    int8_t var16 = 0;
    Tessellator *var17 = &Tessellator_instance;
    float var18 = 0.0f;
    R_bindTexture((unsigned int)self->field_28158_b);
    R_enable(R_BLEND);
    R_disable(R_ALPHA_TEST);
    Tessellator_startDrawingQuads(var17);
    Tessellator_addVertexWithUV(var17, (double)((float)(var15 + 0) + var18),
                                (double)((float)(var16 + 128) - var18), (double)-0.01f, 0.0, 1.0);
    Tessellator_addVertexWithUV(var17, (double)((float)(var15 + 128) - var18),
                                (double)((float)(var16 + 128) - var18), (double)-0.01f, 1.0, 1.0);
    Tessellator_addVertexWithUV(var17, (double)((float)(var15 + 128) - var18),
                                (double)((float)(var16 + 0) + var18), (double)-0.01f, 1.0, 0.0);
    Tessellator_addVertexWithUV(var17, (double)((float)(var15 + 0) + var18),
                                (double)((float)(var16 + 0) + var18), (double)-0.01f, 0.0, 0.0);
    Tessellator_draw(var17);
    R_enable(R_ALPHA_TEST);
    R_disable(R_BLEND);
    RenderEngine_bindTexture(var2, RenderEngine_getTexture(var2, "/misc/mapicons.png"));

    for (int vi = 0; vi < var3->field_28173_i_size; ++vi) {
        MapCoord *var20 = var3->field_28173_i[vi];
        R_pushMatrix();
        R_translatef((float)var15 + (float)var20->field_28216_b / 2.0f + 64.0f,
                     (float)var16 + (float)var20->field_28220_c / 2.0f + 64.0f, -0.02f);
        R_rotatef((float)(var20->field_28219_d * 360) / 16.0f, 0.0f, 0.0f, 1.0f);
        R_scalef(4.0f, 4.0f, 3.0f);
        R_translatef(-(2.0f / 16.0f), 2.0f / 16.0f, 0.0f);
        float var21 = (float)(var20->field_28217_a % 4 + 0) / 4.0f;
        float var22 = (float)(var20->field_28217_a / 4 + 0) / 4.0f;
        float var23 = (float)(var20->field_28217_a % 4 + 1) / 4.0f;
        float var24 = (float)(var20->field_28217_a / 4 + 1) / 4.0f;
        Tessellator_startDrawingQuads(var17);
        Tessellator_addVertexWithUV(var17, -1.0, 1.0, 0.0, (double)var21, (double)var22);
        Tessellator_addVertexWithUV(var17, 1.0, 1.0, 0.0, (double)var23, (double)var22);
        Tessellator_addVertexWithUV(var17, 1.0, -1.0, 0.0, (double)var23, (double)var24);
        Tessellator_addVertexWithUV(var17, -1.0, -1.0, 0.0, (double)var21, (double)var24);
        Tessellator_draw(var17);
        R_popMatrix();
    }

    R_pushMatrix();
    R_translatef(0.0f, 0.0f, -0.04f);
    R_scalef(1.0f, 1.0f, 1.0f);
    FontRenderer_drawString(self->field_28160_d, var3->base.field_28168_a, var15, var16, -16777216);
    R_popMatrix();
}
