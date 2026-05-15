#include "Renderer.h"

#include "RenderTNTPrimed.h"
#include "Render.h"
#include "Block.h"
#include "Entity.h"
#include <stdlib.h>

static void func_153_a(RenderTNTPrimed *self, EntityTNTPrimed *var1, double var2, double var4,
                       double var6, float var8, float var9) {
    (void)var8;
    R_pushMatrix();
    R_translatef((float)var2, (float)var4, (float)var6);
    float var10;
    if ((float)var1->fuse - var9 + 1.0f < 10.0f) {
        var10 = 1.0f - ((float)var1->fuse - var9 + 1.0f) / 10.0f;
        if (var10 < 0.0f) {
            var10 = 0.0f;
        }
        if (var10 > 1.0f) {
            var10 = 1.0f;
        }
        var10 *= var10;
        var10 *= var10;
        float var11 = 1.0f + var10 * 0.3f;
        R_scalef(var11, var11, var11);
    }
    var10 = (1.0f - ((float)var1->fuse - var9 + 1.0f) / 100.0f) * 0.8f;
    Render_loadTexture(&self->base, "/terrain.png");
    RenderBlocks_renderBlockOnInventory(
        &self->blockRenderer, Block_tnt, 0,
        ((Entity *)var1)->vtable->getEntityBrightness((Entity *)var1, var9));
    if (var1->fuse / 5 % 2 == 0) {
        R_disable(R_TEXTURE_2D);
        R_disable(R_LIGHTING);
        R_enable(R_BLEND);
        R_blendFunc(R_SRC_ALPHA, R_DST_ALPHA);
        R_color4f(1.0f, 1.0f, 1.0f, var10);
        RenderBlocks_renderBlockOnInventory(&self->blockRenderer, Block_tnt, 0, 1.0f);
        R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
        R_disable(R_BLEND);
        R_enable(R_LIGHTING);
        R_enable(R_TEXTURE_2D);
    }
    R_popMatrix();
}

static void vt_doRender(struct Render *base, struct Entity *var1, double var2, double var4,
                        double var6, float var8, float var9) {
    func_153_a((RenderTNTPrimed *)base, (EntityTNTPrimed *)var1, var2, var4, var6, var8, var9);
}

static Render_vtable s_vtable;
static int s_vtable_init = 0;

RenderTNTPrimed *RenderTNTPrimed_create(void) {
    if (!s_vtable_init) {
        s_vtable.doRender = vt_doRender;
        s_vtable_init = 1;
    }
    RenderTNTPrimed *self = (RenderTNTPrimed *)calloc(1, sizeof(RenderTNTPrimed));
    Render_init(&self->base);
    self->base.vtable = &s_vtable;
    self->base.shadowSize = 0.5f;
    RenderBlocks_init(&self->blockRenderer, NULL);
    return self;
}
