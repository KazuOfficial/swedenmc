#include "Renderer.h"

#include "RenderGiantZombie.h"
#include "RenderLiving.h"
#include <stdlib.h>

static void preRenderScale(RenderGiantZombie *self, EntityGiantZombie *var1, float var2) {
    (void)var1;
    (void)var2;
    R_scalef(self->scale, self->scale, self->scale);
}

static void vt_preRenderCallback(RenderLiving *base, EntityLiving *var1, float var2) {
    preRenderScale((RenderGiantZombie *)base, (EntityGiantZombie *)var1, var2);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderGiantZombie *RenderGiantZombie_create(ModelBase *var1, float var2, float var3) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.preRenderCallback = vt_preRenderCallback;
        s_vtable_init = 1;
    }
    RenderGiantZombie *self = (RenderGiantZombie *)calloc(1, sizeof(RenderGiantZombie));
    RenderLiving_init(&self->base, var1, var2 * var3);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    self->scale = var3;
    return self;
}
