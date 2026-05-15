#include "RenderCow.h"
#include "RenderLiving.h"
#include <stdlib.h>

static void renderCow(RenderCow *self, EntityCow *var1, double var2, double var4, double var6,
                      float var8, float var9) {
    RenderLiving_doRenderLiving(&self->base, (EntityLiving *)var1, var2, var4, var6, var8, var9);
}

static void vt_doRenderLiving(RenderLiving *base, EntityLiving *var1, double var2, double var4,
                              double var6, float var8, float var9) {
    renderCow((RenderCow *)base, (EntityCow *)var1, var2, var4, var6, var8, var9);
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    renderCow((RenderCow *)base, (EntityCow *)var1, var2, var4, var6, var8, var9);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderCow *RenderCow_create(ModelBase *var1, float var2) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.base.doRender = vt_doRender;
        s_vtable.doRenderLiving = vt_doRenderLiving;
        s_vtable_init = 1;
    }
    RenderCow *self = (RenderCow *)calloc(1, sizeof(RenderCow));
    RenderLiving_init(&self->base, var1, var2);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    return self;
}
