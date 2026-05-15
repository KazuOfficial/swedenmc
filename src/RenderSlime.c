#include "Renderer.h"

#include "RenderSlime.h"
#include "RenderLiving.h"
#include "EntitySlime.h"
#include <stdlib.h>

static int renderSlimePassModel(RenderSlime *self, EntitySlime *var1, int var2, float var3) {
    (void)var1;
    (void)var3;
    if (var2 == 0) {
        RenderLiving_setRenderPassModel(&self->base, self->scaleAmount);
        R_enable(R_NORMALIZE);
        R_enable(R_BLEND);
        R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
        return 1;
    } else {
        if (var2 == 1) {
            R_disable(R_BLEND);
            R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
        return 0;
    }
}

static void scaleSlime(RenderSlime *self, EntitySlime *var1, float var2) {
    (void)self;
    int var3 = EntitySlime_getSlimeSize(var1);
    float var4 = (var1->field_767_b + (var1->field_768_a - var1->field_767_b) * var2) /
                 ((float)var3 * 0.5f + 1.0f);
    float var5 = 1.0f / (var4 + 1.0f);
    float var6 = (float)var3;
    R_scalef(var5 * var6, 1.0f / var5 * var6, var5 * var6);
}

static void vt_preRenderCallback(RenderLiving *base, EntityLiving *var1, float var2) {
    scaleSlime((RenderSlime *)base, (EntitySlime *)var1, var2);
}

static int vt_shouldRenderPass(RenderLiving *base, EntityLiving *var1, int var2, float var3) {
    return renderSlimePassModel((RenderSlime *)base, (EntitySlime *)var1, var2, var3);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderSlime *RenderSlime_create(ModelBase *var1, ModelBase *var2, float var3) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.preRenderCallback = vt_preRenderCallback;
        s_vtable.shouldRenderPass = vt_shouldRenderPass;
        s_vtable_init = 1;
    }
    RenderSlime *self = (RenderSlime *)calloc(1, sizeof(RenderSlime));
    RenderLiving_init(&self->base, var1, var3);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    self->scaleAmount = var2;
    return self;
}
