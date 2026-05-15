#include "Renderer.h"

#include "RenderGhast.h"
#include "RenderLiving.h"
#include <stdlib.h>

static void func_4014_a(RenderGhast *self, EntityGhast *var1, float var2) {
    (void)self;
    float var4 = ((float)var1->prevAttackCounter +
                  (float)(var1->attackCounter - var1->prevAttackCounter) * var2) /
                 20.0f;
    if (var4 < 0.0f) {
        var4 = 0.0f;
    }
    var4 = 1.0f / (var4 * var4 * var4 * var4 * var4 * 2.0f + 1.0f);
    float var5 = (8.0f + var4) / 2.0f;
    float var6 = (8.0f + 1.0f / var4) / 2.0f;
    R_scalef(var6, var5, var6);
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
}

static void vt_preRenderCallback(RenderLiving *base, EntityLiving *var1, float var2) {
    func_4014_a((RenderGhast *)base, (EntityGhast *)var1, var2);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderGhast *RenderGhast_create(void) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.preRenderCallback = vt_preRenderCallback;
        s_vtable_init = 1;
    }
    ModelGhast *mg = (ModelGhast *)calloc(1, sizeof(ModelGhast));
    ModelGhast_init(mg);
    RenderGhast *self = (RenderGhast *)calloc(1, sizeof(RenderGhast));
    RenderLiving_init(&self->base, (ModelBase *)mg, 0.5f);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    return self;
}
