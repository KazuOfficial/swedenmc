#include "Renderer.h"

#include "RenderLightningBolt.h"
#include "Render.h"
#include "Tessellator.h"
#include "JavaRandom.h"
#include <stdlib.h>

static void func_27002_a(RenderLightningBolt *self, EntityLightningBolt *var1, double var2,
                         double var4, double var6, float var8, float var9) {
    (void)self;
    (void)var8;
    (void)var9;
    Tessellator *var10 = &Tessellator_instance;
    R_disable(R_TEXTURE_2D);
    R_disable(R_LIGHTING);
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE);
    double var11[8];
    double var12[8];
    double var13 = 0.0;
    double var15 = 0.0;
    JavaRandom var17;
    JavaRandom_init(&var17, var1->field_27029_a);
    for (int var18 = 7; var18 >= 0; --var18) {
        var11[var18] = var13;
        var12[var18] = var15;
        var13 += (double)(JavaRandom_nextInt(&var17, 11) - 5);
        var15 += (double)(JavaRandom_nextInt(&var17, 11) - 5);
    }
    for (int var45 = 0; var45 < 4; ++var45) {
        JavaRandom var46;
        JavaRandom_init(&var46, var1->field_27029_a);
        for (int var19 = 0; var19 < 3; ++var19) {
            int var20 = 7;
            int var21 = 0;
            if (var19 > 0) {
                var20 = 7 - var19;
            }
            if (var19 > 0) {
                var21 = var20 - 2;
            }
            double var22 = var11[var20] - var13;
            double var24 = var12[var20] - var15;
            for (int var26 = var20; var26 >= var21; --var26) {
                double var27 = var22;
                double var29 = var24;
                if (var19 == 0) {
                    var22 += (double)(JavaRandom_nextInt(&var46, 11) - 5);
                    var24 += (double)(JavaRandom_nextInt(&var46, 11) - 5);
                } else {
                    var22 += (double)(JavaRandom_nextInt(&var46, 31) - 15);
                    var24 += (double)(JavaRandom_nextInt(&var46, 31) - 15);
                }
                Tessellator_startDrawing(var10, 5);
                float var31 = 0.5f;
                Tessellator_setColorRGBA_F(var10, 0.9f * var31, 0.9f * var31, 1.0f * var31, 0.3f);
                double var32 = 0.1 + (double)var45 * 0.2;
                if (var19 == 0) {
                    var32 *= (double)var26 * 0.1 + 1.0;
                }
                double var34 = 0.1 + (double)var45 * 0.2;
                if (var19 == 0) {
                    var34 *= (double)(var26 - 1) * 0.1 + 1.0;
                }
                for (int var36 = 0; var36 < 5; ++var36) {
                    double var37 = var2 + 0.5 - var32;
                    double var39 = var6 + 0.5 - var32;
                    if (var36 == 1 || var36 == 2) {
                        var37 += var32 * 2.0;
                    }
                    if (var36 == 2 || var36 == 3) {
                        var39 += var32 * 2.0;
                    }
                    double var41 = var2 + 0.5 - var34;
                    double var43 = var6 + 0.5 - var34;
                    if (var36 == 1 || var36 == 2) {
                        var41 += var34 * 2.0;
                    }
                    if (var36 == 2 || var36 == 3) {
                        var43 += var34 * 2.0;
                    }
                    Tessellator_addVertex(var10, var41 + var22, var4 + (double)(var26 * 16),
                                          var43 + var24);
                    Tessellator_addVertex(var10, var37 + var27, var4 + (double)((var26 + 1) * 16),
                                          var39 + var29);
                }
                Tessellator_draw(var10);
            }
        }
    }
    R_disable(R_BLEND);
    R_enable(R_LIGHTING);
    R_enable(R_TEXTURE_2D);
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    func_27002_a((RenderLightningBolt *)base, (EntityLightningBolt *)var1, var2, var4, var6, var8,
                 var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderLightningBolt *RenderLightningBolt_create(void) {
    RenderLightningBolt *self = (RenderLightningBolt *)calloc(1, sizeof(RenderLightningBolt));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    return self;
}
