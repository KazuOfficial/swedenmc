#include "Renderer.h"

#include "RenderSquid.h"
#include "RenderLiving.h"
#include "EntitySquid.h"
#include <stdlib.h>

static void func_21008_a(RenderSquid *self, EntitySquid *var1, double var2, double var4,
                         double var6, float var8, float var9) {
    RenderLiving_doRenderLiving(&self->base, (EntityLiving *)var1, var2, var4, var6, var8, var9);
}

static void func_21007_a(RenderSquid *self, EntitySquid *var1, float var2, float var3, float var4) {
    (void)self;
    (void)var2;
    float var5 = var1->field_21088_b + (var1->field_21089_a - var1->field_21088_b) * var4;
    float var6 = var1->field_21086_f + (var1->field_21087_c - var1->field_21086_f) * var4;
    R_translatef(0.0f, 0.5f, 0.0f);
    R_rotatef(180.0f - var3, 0.0f, 1.0f, 0.0f);
    R_rotatef(var5, 1.0f, 0.0f, 0.0f);
    R_rotatef(var6, 0.0f, 1.0f, 0.0f);
    R_translatef(0.0f, -1.2f, 0.0f);
}

static void func_21005_a(RenderSquid *self, EntitySquid *var1, float var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

static float func_21006_b(RenderSquid *self, EntitySquid *var1, float var2) {
    (void)self;
    float var3 = var1->field_21082_j + (var1->field_21083_i - var1->field_21082_j) * var2;
    return var3;
}

static void vt_preRenderCallback(RenderLiving *base, EntityLiving *var1, float var2) {
    func_21005_a((RenderSquid *)base, (EntitySquid *)var1, var2);
}

static float vt_func_170_d(RenderLiving *base, EntityLiving *var1, float var2) {
    return func_21006_b((RenderSquid *)base, (EntitySquid *)var1, var2);
}

static void vt_rotateCorpse(RenderLiving *base, EntityLiving *var1, float var2, float var3,
                            float var4) {
    func_21007_a((RenderSquid *)base, (EntitySquid *)var1, var2, var3, var4);
}

static void vt_doRenderLiving(RenderLiving *base, EntityLiving *var1, double var2, double var4,
                              double var6, float var8, float var9) {
    func_21008_a((RenderSquid *)base, (EntitySquid *)var1, var2, var4, var6, var8, var9);
}

static void vt_doRender(struct Render *base, struct Entity *var1, double var2, double var4,
                        double var6, float var8, float var9) {
    func_21008_a((RenderSquid *)base, (EntitySquid *)var1, var2, var4, var6, var8, var9);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderSquid *RenderSquid_create(ModelBase *var1, float var2) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.base.doRender = vt_doRender;
        s_vtable.doRenderLiving = vt_doRenderLiving;
        s_vtable.rotateCorpse = vt_rotateCorpse;
        s_vtable.func_170_d = vt_func_170_d;
        s_vtable.preRenderCallback = vt_preRenderCallback;
        s_vtable_init = 1;
    }
    RenderSquid *self = (RenderSquid *)calloc(1, sizeof(RenderSquid));
    RenderLiving_init(&self->base, var1, var2);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    return self;
}
