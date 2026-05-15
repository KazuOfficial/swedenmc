#include "Renderer.h"

#include "RenderSpider.h"
#include "RenderLiving.h"
#include "EntitySpider.h"
#include "ModelSpider.h"
#include <stdlib.h>

static float setSpiderDeathMaxRotation(RenderSpider *self, EntitySpider *var1) {
    (void)self;
    (void)var1;
    return 180.0f;
}

static int setSpiderEyeBrightness(RenderSpider *self, EntitySpider *var1, int var2, float var3) {
    (void)var3;
    if (var2 != 0) {
        return 0;
    } else if (var2 != 0) {
        return 0;
    } else {
        Render_loadTexture(&self->base.base, "/mob/spider_eyes.png");
        float var4 =
            (1.0f - ((Entity *)var1)->vtable->getEntityBrightness((Entity *)var1, 1.0f)) * 0.5f;
        R_enable(R_BLEND);
        R_disable(R_ALPHA_TEST);
        R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
        R_color4f(1.0f, 1.0f, 1.0f, var4);
        return 1;
    }
}

static float vt_getDeathMaxRotation(RenderLiving *base, EntityLiving *var1) {
    return setSpiderDeathMaxRotation((RenderSpider *)base, (EntitySpider *)var1);
}

static int vt_shouldRenderPass(RenderLiving *base, EntityLiving *var1, int var2, float var3) {
    return setSpiderEyeBrightness((RenderSpider *)base, (EntitySpider *)var1, var2, var3);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderSpider *RenderSpider_create(void) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.getDeathMaxRotation = vt_getDeathMaxRotation;
        s_vtable.shouldRenderPass = vt_shouldRenderPass;
        s_vtable_init = 1;
    }
    ModelSpider *ms1 = (ModelSpider *)calloc(1, sizeof(ModelSpider));
    ModelSpider_init(ms1);
    ModelSpider *ms2 = (ModelSpider *)calloc(1, sizeof(ModelSpider));
    ModelSpider_init(ms2);
    RenderSpider *self = (RenderSpider *)calloc(1, sizeof(RenderSpider));
    RenderLiving_init(&self->base, (ModelBase *)ms1, 1.0f);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    RenderLiving_setRenderPassModel(&self->base, (ModelBase *)ms2);
    return self;
}
