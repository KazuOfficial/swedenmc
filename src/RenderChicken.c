#include "RenderChicken.h"
#include "RenderLiving.h"
#include "MathHelper.h"
#include <stdlib.h>

static void renderChicken(RenderChicken *self, EntityChicken *var1, double var2, double var4,
                          double var6, float var8, float var9) {
    RenderLiving_doRenderLiving(&self->base, (EntityLiving *)var1, var2, var4, var6, var8, var9);
}

static float getWingRotation(RenderChicken *self, EntityChicken *var1, float var2) {
    (void)self;
    float var3 = var1->field_756_e + (var1->field_752_b - var1->field_756_e) * var2;
    float var4 = var1->field_757_d + (var1->destPos - var1->field_757_d) * var2;
    return (MathHelper_sin(var3) + 1.0f) * var4;
}

static float vt_func_170_d(RenderLiving *base, EntityLiving *var1, float var2) {
    return getWingRotation((RenderChicken *)base, (EntityChicken *)var1, var2);
}

static void vt_doRenderLiving(RenderLiving *base, EntityLiving *var1, double var2, double var4,
                              double var6, float var8, float var9) {
    renderChicken((RenderChicken *)base, (EntityChicken *)var1, var2, var4, var6, var8, var9);
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    renderChicken((RenderChicken *)base, (EntityChicken *)var1, var2, var4, var6, var8, var9);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderChicken *RenderChicken_create(ModelBase *var1, float var2) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.base.doRender = vt_doRender;
        s_vtable.doRenderLiving = vt_doRenderLiving;
        s_vtable.func_170_d = vt_func_170_d;
        s_vtable_init = 1;
    }
    RenderChicken *self = (RenderChicken *)calloc(1, sizeof(RenderChicken));
    RenderLiving_init(&self->base, var1, var2);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    return self;
}
