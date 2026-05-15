#include "Renderer.h"

#include "RenderFallingSand.h"
#include "Render.h"
#include "Block.h"
#include "MathHelper.h"
#include "RenderBlocks.h"
#include <stdlib.h>

static void doRenderFallingSand(RenderFallingSand *self, EntityFallingSand *var1, double var2,
                                double var4, double var6, float var8, float var9) {
    (void)var8;
    (void)var9;
    R_pushMatrix();
    R_translatef((float)var2, (float)var4, (float)var6);
    Render_loadTexture(&self->base, "/terrain.png");
    Block *var10 =
        (var1->blockID > 0 && var1->blockID < 256) ? Block_blocksList[var1->blockID] : NULL;
    if (!var10) {
        R_popMatrix();
        return;
    }
    World *var11 = EntityFallingSand_getWorld(var1);
    R_disable(R_LIGHTING);
    RenderBlocks_renderBlockFallingSand(self->field_197_d, var10, var11,
                                        MathHelper_floor_double(((Entity *)var1)->posX),
                                        MathHelper_floor_double(((Entity *)var1)->posY),
                                        MathHelper_floor_double(((Entity *)var1)->posZ));
    R_enable(R_LIGHTING);
    R_popMatrix();
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    doRenderFallingSand((RenderFallingSand *)base, (EntityFallingSand *)var1, var2, var4, var6,
                        var8, var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderFallingSand *RenderFallingSand_create(void) {
    RenderFallingSand *self = (RenderFallingSand *)calloc(1, sizeof(RenderFallingSand));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    self->base.shadowSize = 0.5f;
    self->field_197_d = (RenderBlocks *)calloc(1, sizeof(RenderBlocks));
    RenderBlocks_init(self->field_197_d, NULL);
    return self;
}
