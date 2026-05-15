#include "Renderer.h"

#include "RenderEntity.h"
#include "Render.h"
#include "Entity.h"
#include "AxisAlignedBB.h"
#include <stdlib.h>

static void vt_doRender(Render *base, Entity *var1, double var2, double var4, double var6,
                        float var8, float var9) {
    (void)base;
    (void)var8;
    (void)var9;
    R_pushMatrix();
    Render_renderOffsetAABB(&var1->boundingBox, var2 - var1->lastTickPosX,
                            var4 - var1->lastTickPosY, var6 - var1->lastTickPosZ);
    R_popMatrix();
}

static const Render_vtable s_vtable = {.doRender = vt_doRender};

RenderEntity *RenderEntity_create(void) {
    RenderEntity *self = (RenderEntity *)calloc(1, sizeof(RenderEntity));
    self->base.vtable = &s_vtable;
    Render_init(&self->base);
    return self;
}
