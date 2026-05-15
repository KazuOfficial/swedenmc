#include "RenderWolf.h"
#include "RenderLiving.h"
#include "EntityWolf.h"
#include <stdlib.h>

static void renderWolf(RenderWolf *self, EntityWolf *var1, double var2, double var4, double var6,
                       float var8, float var9) {
    RenderLiving_doRenderLiving(&self->base, (EntityLiving *)var1, var2, var4, var6, var8, var9);
}

static float func_25004_a(RenderWolf *self, EntityWolf *var1, float var2) {
    (void)self;
    (void)var2;
    return EntityWolf_setTailRotation(var1);
}

static void func_25006_b(RenderWolf *self, EntityWolf *var1, float var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

static void vt_preRenderCallback(RenderLiving *base, EntityLiving *var1, float var2) {
    func_25006_b((RenderWolf *)base, (EntityWolf *)var1, var2);
}

static float vt_func_170_d(RenderLiving *base, EntityLiving *var1, float var2) {
    return func_25004_a((RenderWolf *)base, (EntityWolf *)var1, var2);
}

static void vt_doRenderLiving(RenderLiving *base, EntityLiving *var1, double var2, double var4,
                              double var6, float var8, float var9) {
    renderWolf((RenderWolf *)base, (EntityWolf *)var1, var2, var4, var6, var8, var9);
}

static void vt_doRender(struct Render *base, struct Entity *var1, double var2, double var4,
                        double var6, float var8, float var9) {
    renderWolf((RenderWolf *)base, (EntityWolf *)var1, var2, var4, var6, var8, var9);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderWolf *RenderWolf_create(ModelBase *var1, float var2) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.base.doRender = vt_doRender;
        s_vtable.doRenderLiving = vt_doRenderLiving;
        s_vtable.func_170_d = vt_func_170_d;
        s_vtable.preRenderCallback = vt_preRenderCallback;
        s_vtable_init = 1;
    }
    RenderWolf *self = (RenderWolf *)calloc(1, sizeof(RenderWolf));
    RenderLiving_init(&self->base, var1, var2);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    return self;
}
