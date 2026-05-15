#include "Renderer.h"

#include "RenderPainting.h"
#include "Render.h"
#include "Tessellator.h"
#include "MathHelper.h"
#include "EnumArt.h"
#include <stdlib.h>

#ifndef R_RESCALE_NORMAL
#define R_RESCALE_NORMAL 0x803A
#endif

static void func_160_a(RenderPainting *self, EntityPainting *var1, float var2, float var3) {
    int var4 = MathHelper_floor_double(var1->base.posX);
    int var5 = MathHelper_floor_double(var1->base.posY + (double)(var3 / 16.0f));
    int var6 = MathHelper_floor_double(var1->base.posZ);
    if (var1->direction == 0) {
        var4 = MathHelper_floor_double(var1->base.posX + (double)(var2 / 16.0f));
    }
    if (var1->direction == 1) {
        var6 = MathHelper_floor_double(var1->base.posZ - (double)(var2 / 16.0f));
    }
    if (var1->direction == 2) {
        var4 = MathHelper_floor_double(var1->base.posX - (double)(var2 / 16.0f));
    }
    if (var1->direction == 3) {
        var6 = MathHelper_floor_double(var1->base.posZ + (double)(var2 / 16.0f));
    }
    float var7 = (float)self->base.renderManager->worldObj->vtable->getLightBrightness(
        self->base.renderManager->worldObj, var4, var5, var6);
    R_color3f(var7, var7, var7);
}

static void func_159_a(RenderPainting *self, EntityPainting *var1, int var2, int var3, int var4,
                       int var5) {
    float var6 = (float)(-var2) / 2.0f;
    float var7 = (float)(-var3) / 2.0f;
    float var8 = -0.5f;
    float var9 = 0.5f;
    for (int var10 = 0; var10 < var2 / 16; ++var10) {
        for (int var11 = 0; var11 < var3 / 16; ++var11) {
            float var12 = var6 + (float)((var10 + 1) * 16);
            float var13 = var6 + (float)(var10 * 16);
            float var14 = var7 + (float)((var11 + 1) * 16);
            float var15 = var7 + (float)(var11 * 16);
            func_160_a(self, var1, (var12 + var13) / 2.0f, (var14 + var15) / 2.0f);
            float var16 = (float)(var4 + var2 - var10 * 16) / 256.0f;
            float var17 = (float)(var4 + var2 - (var10 + 1) * 16) / 256.0f;
            float var18 = (float)(var5 + var3 - var11 * 16) / 256.0f;
            float var19 = (float)(var5 + var3 - (var11 + 1) * 16) / 256.0f;
            float var20 = 12.0f / 16.0f;
            float var21 = 13.0f / 16.0f;
            float var22 = 0.0f;
            float var23 = 1.0f / 16.0f;
            float var24 = 12.0f / 16.0f;
            float var25 = 13.0f / 16.0f;
            float var26 = 0.001953125f;
            float var27 = 0.001953125f;
            float var28 = 385.0f / 512.0f;
            float var29 = 385.0f / 512.0f;
            float var30 = 0.0f;
            float var31 = 1.0f / 16.0f;
            Tessellator *var32 = &Tessellator_instance;
            Tessellator_startDrawingQuads(var32);
            Tessellator_setNormal(var32, 0.0f, 0.0f, -1.0f);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var15, (double)var8,
                                        (double)var17, (double)var18);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var15, (double)var8,
                                        (double)var16, (double)var18);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var14, (double)var8,
                                        (double)var16, (double)var19);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var14, (double)var8,
                                        (double)var17, (double)var19);
            Tessellator_setNormal(var32, 0.0f, 0.0f, 1.0f);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var14, (double)var9,
                                        (double)var20, (double)var22);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var14, (double)var9,
                                        (double)var21, (double)var22);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var15, (double)var9,
                                        (double)var21, (double)var23);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var15, (double)var9,
                                        (double)var20, (double)var23);
            Tessellator_setNormal(var32, 0.0f, -1.0f, 0.0f);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var14, (double)var8,
                                        (double)var24, (double)var26);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var14, (double)var8,
                                        (double)var25, (double)var26);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var14, (double)var9,
                                        (double)var25, (double)var27);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var14, (double)var9,
                                        (double)var24, (double)var27);
            Tessellator_setNormal(var32, 0.0f, 1.0f, 0.0f);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var15, (double)var9,
                                        (double)var24, (double)var26);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var15, (double)var9,
                                        (double)var25, (double)var26);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var15, (double)var8,
                                        (double)var25, (double)var27);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var15, (double)var8,
                                        (double)var24, (double)var27);
            Tessellator_setNormal(var32, -1.0f, 0.0f, 0.0f);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var14, (double)var9,
                                        (double)var29, (double)var30);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var15, (double)var9,
                                        (double)var29, (double)var31);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var15, (double)var8,
                                        (double)var28, (double)var31);
            Tessellator_addVertexWithUV(var32, (double)var12, (double)var14, (double)var8,
                                        (double)var28, (double)var30);
            Tessellator_setNormal(var32, 1.0f, 0.0f, 0.0f);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var14, (double)var8,
                                        (double)var29, (double)var30);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var15, (double)var8,
                                        (double)var29, (double)var31);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var15, (double)var9,
                                        (double)var28, (double)var31);
            Tessellator_addVertexWithUV(var32, (double)var13, (double)var14, (double)var9,
                                        (double)var28, (double)var30);
            Tessellator_draw(var32);
        }
    }
}

static void func_158_a(RenderPainting *self, EntityPainting *var1, double var2, double var4,
                       double var6, float var8, float var9) {
    (void)var9;
    JavaRandom_setSeed(&self->rand, 187L);
    R_pushMatrix();
    R_translatef((float)var2, (float)var4, (float)var6);
    R_rotatef(var8, 0.0f, 1.0f, 0.0f);
    R_enable(R_RESCALE_NORMAL);
    Render_loadTexture(&self->base, "/art/kz.png");
    EnumArt *var10 = var1->art;
    float var11 = 1.0f / 16.0f;
    R_scalef(var11, var11, var11);
    func_159_a(self, var1, var10->sizeX, var10->sizeY, var10->offsetX, var10->offsetY);
    R_disable(R_RESCALE_NORMAL);
    R_popMatrix();
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    func_158_a((RenderPainting *)base, (EntityPainting *)var1, var2, var4, var6, var8, var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderPainting *RenderPainting_create(void) {
    RenderPainting *self = (RenderPainting *)calloc(1, sizeof(RenderPainting));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    JavaRandom_init(&self->rand, 0);
    return self;
}
