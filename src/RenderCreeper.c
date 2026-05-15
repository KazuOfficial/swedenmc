#include "Renderer.h"

#include "RenderCreeper.h"
#include "RenderLiving.h"
#include "MathHelper.h"
#include <stdlib.h>

static void updateCreeperScale(RenderCreeper *self, EntityCreeper *var1, float var2) {
    (void)self;
    float var4 = EntityCreeper_setCreeperFlashTime(var1, var2);
    float var5 = 1.0f + MathHelper_sin(var4 * 100.0f) * var4 * 0.01f;
    if (var4 < 0.0f) {
        var4 = 0.0f;
    }
    if (var4 > 1.0f) {
        var4 = 1.0f;
    }
    var4 *= var4;
    var4 *= var4;
    float var6 = (1.0f + var4 * 0.4f) * var5;
    float var7 = (1.0f + var4 * 0.1f) / var5;
    R_scalef(var6, var7, var6);
}

static int updateCreeperColorMultiplier(RenderCreeper *self, EntityCreeper *var1, float var2,
                                        float var3) {
    (void)self;
    (void)var2;
    float var5 = EntityCreeper_setCreeperFlashTime(var1, var3);
    if ((int)(var5 * 10.0f) % 2 == 0) {
        return 0;
    } else {
        int var6 = (int)(var5 * 0.2f * 255.0f);
        if (var6 < 0) {
            var6 = 0;
        }
        if (var6 > 255) {
            var6 = 255;
        }
        int var7 = 255;
        int var8 = 255;
        int var9 = 255;
        return (int)(((unsigned int)var6 << 24) | ((unsigned int)var7 << 16) |
                     ((unsigned int)var8 << 8) | (unsigned int)var9);
    }
}

static int func_27006_a(RenderCreeper *self, EntityCreeper *var1, int var2, float var3) {
    if (EntityCreeper_getPowered(var1)) {
        if (var2 == 1) {
            float var4 = (float)var1->base.base.base.base.ticksExisted + var3;
            Render_loadTexture(&self->base.base, "/armor/power.png");
            R_matrixMode(R_TEXTURE);
            R_loadIdentity();
            float var5 = var4 * 0.01f;
            float var6 = var4 * 0.01f;
            R_translatef(var5, var6, 0.0f);
            RenderLiving_setRenderPassModel(&self->base, self->field_27008_a);
            R_matrixMode(R_MODELVIEW);
            R_enable(R_BLEND);
            float var7 = 0.5f;
            R_color4f(var7, var7, var7, 1.0f);
            R_disable(R_LIGHTING);
            R_blendFunc(R_ONE, R_ONE);
            return 1;
        }
        if (var2 == 2) {
            R_matrixMode(R_TEXTURE);
            R_loadIdentity();
            R_matrixMode(R_MODELVIEW);
            R_enable(R_LIGHTING);
            R_disable(R_BLEND);
        }
    }
    return 0;
}

static int func_27007_b(RenderCreeper *self, EntityCreeper *var1, int var2, float var3) {
    (void)self;
    (void)var1;
    (void)var2;
    (void)var3;
    return 0;
}

static void vt_preRenderCallback(RenderLiving *base, EntityLiving *var1, float var2) {
    updateCreeperScale((RenderCreeper *)base, (EntityCreeper *)var1, var2);
}
static int vt_getColorMultiplier(RenderLiving *base, EntityLiving *var1, float var2, float var3) {
    return updateCreeperColorMultiplier((RenderCreeper *)base, (EntityCreeper *)var1, var2, var3);
}
static int vt_shouldRenderPass(RenderLiving *base, EntityLiving *var1, int var2, float var3) {
    return func_27006_a((RenderCreeper *)base, (EntityCreeper *)var1, var2, var3);
}
static int vt_func_27005_b(RenderLiving *base, EntityLiving *var1, int var2, float var3) {
    return func_27007_b((RenderCreeper *)base, (EntityCreeper *)var1, var2, var3);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderCreeper *RenderCreeper_create(void) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.preRenderCallback = vt_preRenderCallback;
        s_vtable.getColorMultiplier = vt_getColorMultiplier;
        s_vtable.shouldRenderPass = vt_shouldRenderPass;
        s_vtable.func_27005_b = vt_func_27005_b;
        s_vtable_init = 1;
    }
    ModelCreeper *mc = (ModelCreeper *)calloc(1, sizeof(ModelCreeper));
    ModelCreeper_init(mc);
    RenderCreeper *self = (RenderCreeper *)calloc(1, sizeof(RenderCreeper));
    RenderLiving_init(&self->base, (ModelBase *)mc, 0.5f);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;

    ModelCreeper *mc2 = (ModelCreeper *)calloc(1, sizeof(ModelCreeper));
    ModelCreeper_initF(mc2, 2.0f);
    self->field_27008_a = (ModelBase *)mc2;
    return self;
}
