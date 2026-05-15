#include "Renderer.h"

#include "RenderSnowball.h"
#include "Render.h"
#include "Tessellator.h"
#include "RenderManager.h"
#include <stdlib.h>

#ifndef R_RESCALE_NORMAL
#define R_RESCALE_NORMAL 0x803A
#endif

static void vt_doRender(struct Render *base, struct Entity *var1, double var2, double var4,
                        double var6, float var8, float var9) {
    RenderSnowball *self = (RenderSnowball *)base;
    (void)var1;
    (void)var8;
    (void)var9;
    R_pushMatrix();
    R_translatef((float)var2, (float)var4, (float)var6);
    R_enable(R_RESCALE_NORMAL);
    R_scalef(0.5f, 0.5f, 0.5f);
    Render_loadTexture(&self->base, "/gui/items.png");
    Tessellator *var10 = &Tessellator_instance;
    float var11 = (float)(self->itemIconIndex % 16 * 16 + 0) / 256.0f;
    float var12 = (float)(self->itemIconIndex % 16 * 16 + 16) / 256.0f;
    float var13 = (float)(self->itemIconIndex / 16 * 16 + 0) / 256.0f;
    float var14 = (float)(self->itemIconIndex / 16 * 16 + 16) / 256.0f;
    float var15 = 1.0f;
    float var16 = 0.5f;
    float var17 = 0.25f;
    R_rotatef(180.0f - self->base.renderManager->playerViewY, 0.0f, 1.0f, 0.0f);
    R_rotatef(-self->base.renderManager->playerViewX, 1.0f, 0.0f, 0.0f);
    Tessellator_startDrawingQuads(var10);
    Tessellator_setNormal(var10, 0.0f, 1.0f, 0.0f);
    Tessellator_addVertexWithUV(var10, (double)(0.0f - var16), (double)(0.0f - var17), 0.0,
                                (double)var11, (double)var14);
    Tessellator_addVertexWithUV(var10, (double)(var15 - var16), (double)(0.0f - var17), 0.0,
                                (double)var12, (double)var14);
    Tessellator_addVertexWithUV(var10, (double)(var15 - var16), (double)(1.0f - var17), 0.0,
                                (double)var12, (double)var13);
    Tessellator_addVertexWithUV(var10, (double)(0.0f - var16), (double)(1.0f - var17), 0.0,
                                (double)var11, (double)var13);
    Tessellator_draw(var10);
    R_disable(R_RESCALE_NORMAL);
    R_popMatrix();
}

static Render_vtable s_vtable;
static int s_vtable_init = 0;

RenderSnowball *RenderSnowball_create(int var1) {
    if (!s_vtable_init) {
        s_vtable.doRender = vt_doRender;
        s_vtable_init = 1;
    }
    RenderSnowball *self = (RenderSnowball *)calloc(1, sizeof(RenderSnowball));
    Render_init(&self->base);
    self->base.vtable = &s_vtable;
    self->itemIconIndex = var1;
    return self;
}
