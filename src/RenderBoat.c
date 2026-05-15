#include "Renderer.h"

#include "RenderBoat.h"
#include "Render.h"
#include "MathHelper.h"
#include "ModelBoat.h"
#include <stdlib.h>

void RenderBoat_func_157_a(RenderBoat *self, EntityBoat *var1, double var2, double var4,
                           double var6, float var8, float var9) {
    R_pushMatrix();
    R_translatef((float)var2, (float)var4, (float)var6);
    R_rotatef(180.0f - var8, 0.0f, 1.0f, 0.0f);
    float var10 = (float)var1->boatTimeSinceHit - var9;
    float var11 = (float)var1->boatCurrentDamage - var9;
    if (var11 < 0.0f) {
        var11 = 0.0f;
    }
    if (var10 > 0.0f) {
        R_rotatef(MathHelper_sin(var10) * var10 * var11 / 10.0f * (float)var1->boatRockDirection,
                  1.0f, 0.0f, 0.0f);
    }
    Render_loadTexture(&self->base, "/terrain.png");
    float var12 = 12.0f / 16.0f;
    R_scalef(var12, var12, var12);
    R_scalef(1.0f / var12, 1.0f / var12, 1.0f / var12);
    Render_loadTexture(&self->base, "/item/boat.png");
    R_scalef(-1.0f, -1.0f, 1.0f);
    self->modelBoat->vtable->render(self->modelBoat, 0.0f, 0.0f, -0.1f, 0.0f, 0.0f, 1.0f / 16.0f);
    R_popMatrix();
}

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    RenderBoat_func_157_a((RenderBoat *)base, (EntityBoat *)var1, var2, var4, var6, var8, var9);
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderBoat *RenderBoat_create(void) {
    RenderBoat *self = (RenderBoat *)calloc(1, sizeof(RenderBoat));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    self->base.shadowSize = 0.5f;
    ModelBoat *mb = (ModelBoat *)calloc(1, sizeof(ModelBoat));
    ModelBoat_init(mb);
    self->modelBoat = (ModelBase *)mb;
    return self;
}
