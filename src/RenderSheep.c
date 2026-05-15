#include "Renderer.h"

#include "RenderSheep.h"
#include "RenderLiving.h"
#include "EntitySheep.h"
#include "EntityLiving.h"
#include "Entity.h"
#include <stdlib.h>

static int setWoolColorAndRender(RenderSheep *self, EntitySheep *var1, int var2, float var3) {
    if (var2 == 0 && !EntitySheep_getSheared(var1)) {
        Render_loadTexture(&self->base.base, "/mob/sheep_fur.png");
        float var4 = ((Entity *)var1)->vtable->getEntityBrightness((Entity *)var1, var3);
        int var5 = EntitySheep_getFleeceColor(var1);
        R_color3f(var4 * EntitySheep_fleeceColorTable[var5][0],
                  var4 * EntitySheep_fleeceColorTable[var5][1],
                  var4 * EntitySheep_fleeceColorTable[var5][2]);
        return 1;
    } else {
        return 0;
    }
}

static int vt_shouldRenderPass(RenderLiving *base, EntityLiving *var1, int var2, float var3) {
    return setWoolColorAndRender((RenderSheep *)base, (EntitySheep *)var1, var2, var3);
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderSheep *RenderSheep_create(ModelBase *var1, ModelBase *var2, float var3) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.shouldRenderPass = vt_shouldRenderPass;
        s_vtable_init = 1;
    }
    RenderSheep *self = (RenderSheep *)calloc(1, sizeof(RenderSheep));
    RenderLiving_init(&self->base, var1, var3);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    RenderLiving_setRenderPassModel(&self->base, var2);
    return self;
}
