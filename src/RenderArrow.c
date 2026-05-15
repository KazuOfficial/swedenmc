#include "Renderer.h"

#include "RenderArrow.h"
#include "Render.h"
#include "Tessellator.h"
#include "MathHelper.h"
#include <stdlib.h>

void RenderArrow_renderArrow(RenderArrow *self, EntityArrow *var1, double var2, double var4,
                             double var6, float var8, float var9) {
    (void)var8;
    if (var1->base.prevRotationYaw != 0.0f || var1->base.prevRotationPitch != 0.0f) {
        Render_loadTexture(&self->base, "/item/arrows.png");
        R_pushMatrix();
        R_translatef((float)var2, (float)var4, (float)var6);
        R_rotatef(var1->base.prevRotationYaw +
                      (var1->base.rotationYaw - var1->base.prevRotationYaw) * var9 - 90.0f,
                  0.0f, 1.0f, 0.0f);
        R_rotatef(var1->base.prevRotationPitch +
                      (var1->base.rotationPitch - var1->base.prevRotationPitch) * var9,
                  0.0f, 0.0f, 1.0f);
        Tessellator *var10 = &Tessellator_instance;
        int var11 = 0;
        float var12 = 0.0f;
        float var13 = 0.5f;
        float var14 = (float)(0 + var11 * 10) / 32.0f;
        float var15 = (float)(5 + var11 * 10) / 32.0f;
        float var16 = 0.0f;
        float var17 = 0.15625f;
        float var18 = (float)(5 + var11 * 10) / 32.0f;
        float var19 = (float)(10 + var11 * 10) / 32.0f;
        float var20 = 0.05625f;
        R_enable(R_RESCALE_NORMAL);
        float var21 = (float)var1->arrowShake - var9;
        if (var21 > 0.0f) {
            float var22 = -MathHelper_sin(var21 * 3.0f) * var21;
            R_rotatef(var22, 0.0f, 0.0f, 1.0f);
        }
        R_rotatef(45.0f, 1.0f, 0.0f, 0.0f);
        R_scalef(var20, var20, var20);
        R_translatef(-4.0f, 0.0f, 0.0f);
        R_normal3f(var20, 0.0f, 0.0f);
        Tessellator_startDrawingQuads(var10);
        Tessellator_addVertexWithUV(var10, -7.0, -2.0, -2.0, (double)var16, (double)var18);
        Tessellator_addVertexWithUV(var10, -7.0, -2.0, 2.0, (double)var17, (double)var18);
        Tessellator_addVertexWithUV(var10, -7.0, 2.0, 2.0, (double)var17, (double)var19);
        Tessellator_addVertexWithUV(var10, -7.0, 2.0, -2.0, (double)var16, (double)var19);
        Tessellator_draw(var10);
        R_normal3f(-var20, 0.0f, 0.0f);
        Tessellator_startDrawingQuads(var10);
        Tessellator_addVertexWithUV(var10, -7.0, 2.0, -2.0, (double)var16, (double)var18);
        Tessellator_addVertexWithUV(var10, -7.0, 2.0, 2.0, (double)var17, (double)var18);
        Tessellator_addVertexWithUV(var10, -7.0, -2.0, 2.0, (double)var17, (double)var19);
        Tessellator_addVertexWithUV(var10, -7.0, -2.0, -2.0, (double)var16, (double)var19);
        Tessellator_draw(var10);

        for (int var23 = 0; var23 < 4; ++var23) {
            R_rotatef(90.0f, 1.0f, 0.0f, 0.0f);
            R_normal3f(0.0f, 0.0f, var20);
            Tessellator_startDrawingQuads(var10);
            Tessellator_addVertexWithUV(var10, -8.0, -2.0, 0.0, (double)var12, (double)var14);
            Tessellator_addVertexWithUV(var10, 8.0, -2.0, 0.0, (double)var13, (double)var14);
            Tessellator_addVertexWithUV(var10, 8.0, 2.0, 0.0, (double)var13, (double)var15);
            Tessellator_addVertexWithUV(var10, -8.0, 2.0, 0.0, (double)var12, (double)var15);
            Tessellator_draw(var10);
        }

        R_disable(R_RESCALE_NORMAL);
        R_popMatrix();
    }
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    RenderArrow_renderArrow((RenderArrow *)base, (EntityArrow *)var1, var2, var4, var6, var8, var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderArrow *RenderArrow_create(void) {
    RenderArrow *self = (RenderArrow *)calloc(1, sizeof(RenderArrow));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    return self;
}
