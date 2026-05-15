#include "Renderer.h"

#include "RenderFireball.h"
#include "Render.h"
#include "Item.h"
#include "Tessellator.h"
#include "RenderManager.h"
#include <stdlib.h>

#ifndef R_RESCALE_NORMAL
#define R_RESCALE_NORMAL 0x803A
#endif

static void func_4012_a(RenderFireball *self, EntityFireball *var1, double var2, double var4,
                        double var6, float var8, float var9) {
    (void)var1;
    (void)var8;
    (void)var9;
    R_pushMatrix();
    R_translatef((float)var2, (float)var4, (float)var6);
    R_enable(R_RESCALE_NORMAL);
    float var10 = 2.0f;
    R_scalef(var10 / 1.0f, var10 / 1.0f, var10 / 1.0f);
    int var11 = Item_snowball->vtable->getIconFromDamage(Item_snowball, 0);
    Render_loadTexture(&self->base, "/gui/items.png");
    Tessellator *var12 = &Tessellator_instance;
    float var13 = (float)(var11 % 16 * 16 + 0) / 256.0f;
    float var14 = (float)(var11 % 16 * 16 + 16) / 256.0f;
    float var15 = (float)(var11 / 16 * 16 + 0) / 256.0f;
    float var16 = (float)(var11 / 16 * 16 + 16) / 256.0f;
    float var17 = 1.0f;
    float var18 = 0.5f;
    float var19 = 0.25f;
    R_rotatef(180.0f - self->base.renderManager->playerViewY, 0.0f, 1.0f, 0.0f);
    R_rotatef(-self->base.renderManager->playerViewX, 1.0f, 0.0f, 0.0f);
    Tessellator_startDrawingQuads(var12);
    Tessellator_setNormal(var12, 0.0f, 1.0f, 0.0f);
    Tessellator_addVertexWithUV(var12, (double)(0.0f - var18), (double)(0.0f - var19), 0.0,
                                (double)var13, (double)var16);
    Tessellator_addVertexWithUV(var12, (double)(var17 - var18), (double)(0.0f - var19), 0.0,
                                (double)var14, (double)var16);
    Tessellator_addVertexWithUV(var12, (double)(var17 - var18), (double)(1.0f - var19), 0.0,
                                (double)var14, (double)var15);
    Tessellator_addVertexWithUV(var12, (double)(0.0f - var18), (double)(1.0f - var19), 0.0,
                                (double)var13, (double)var15);
    Tessellator_draw(var12);
    R_disable(R_RESCALE_NORMAL);
    R_popMatrix();
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    func_4012_a((RenderFireball *)base, (EntityFireball *)var1, var2, var4, var6, var8, var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderFireball *RenderFireball_create(void) {
    RenderFireball *self = (RenderFireball *)calloc(1, sizeof(RenderFireball));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    return self;
}
