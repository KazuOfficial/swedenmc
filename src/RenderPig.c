#include "RenderPig.h"
#include "RenderLiving.h"
#include <stdlib.h>

static int renderSaddledPig(RenderPig *self, EntityPig *var1, int var2, float var3) {
    (void)var3;
    Render_loadTexture(&self->base.base, "/mob/saddle.png");
    return var2 == 0 && EntityPig_getSaddled(var1);
}

static int vt_shouldRenderPass(RenderLiving *base, EntityLiving *var1, int var2, float var3) {
    return renderSaddledPig((RenderPig *)base, (EntityPig *)var1, var2, var3);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderPig *RenderPig_create(ModelBase *var1, ModelBase *var2, float var3) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.shouldRenderPass = vt_shouldRenderPass;
        s_vtable_init = 1;
    }
    RenderPig *self = (RenderPig *)calloc(1, sizeof(RenderPig));
    RenderLiving_init(&self->base, var1, var3);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    RenderLiving_setRenderPassModel(&self->base, var2);
    return self;
}
