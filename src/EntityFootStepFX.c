#include "Renderer.h"
#include "EntityFootStepFX.h"
#include "World.h"
#include "MathHelper.h"
#include <stdlib.h>

static EntityFXVtable EntityFootStepFX_vtable;
static int EntityFootStepFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityFootStepFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityFootStepFX_vtable);
    EntityFootStepFX_vtable.renderParticle =
        (void (*)(EntityFX *, Tessellator *, float, float, float, float, float,
                  float))EntityFootStepFX_renderParticle;
    EntityFootStepFX_vtable.getFXLayer = (int (*)(EntityFX *))EntityFootStepFX_getFXLayer;
    EntityFootStepFX_vtable.base.onUpdate = EntityFootStepFX_onUpdate;
    EntityFootStepFX_vtable_init = 1;
}

void EntityFootStepFX_construct(EntityFootStepFX *self, RenderEngine *var1, World *var2,
                                double var3, double var5, double var7) {
    ensure_vtable();
    EntityFX_construct(&self->base, var2, var3, var5, var7, 0.0, 0.0, 0.0);
    self->base.base.vtable = (const EntityVtable *)&EntityFootStepFX_vtable;
    self->field_27019_p = var1;
    self->base.base.motionX = self->base.base.motionY = self->base.base.motionZ = 0.0;
    self->field_27020_o = 200;
}

EntityFootStepFX *EntityFootStepFX_create(RenderEngine *var1, World *var2, double var3, double var5,
                                          double var7) {
    EntityFootStepFX *self = (EntityFootStepFX *)calloc(1, sizeof(EntityFootStepFX));
    EntityFootStepFX_construct(self, var1, var2, var3, var5, var7);
    return self;
}

void EntityFootStepFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                     float var4, float var5, float var6, float var7) {
    EntityFootStepFX *self = (EntityFootStepFX *)selfFX;
    (void)var3;
    (void)var4;
    (void)var5;
    (void)var6;
    (void)var7;

    float var8 = ((float)self->field_27018_a + var2) / (float)self->field_27020_o;
    var8 *= var8;
    float var9 = 2.0F - var8 * 2.0F;
    if (var9 > 1.0F) {
        var9 = 1.0F;
    }

    var9 *= 0.2F;
    R_disable(R_LIGHTING);
    float var10 = 2.0F / 16.0F;
    float var11 = (float)(self->base.base.posX - EntityFX_interpPosX);
    float var12 = (float)(self->base.base.posY - EntityFX_interpPosY);
    float var13 = (float)(self->base.base.posZ - EntityFX_interpPosZ);
    float var14 = self->base.base.worldObj->vtable->getLightBrightness(
        self->base.base.worldObj, MathHelper_floor_double(self->base.base.posX),
        MathHelper_floor_double(self->base.base.posY),
        MathHelper_floor_double(self->base.base.posZ));
    RenderEngine_bindTexture(self->field_27019_p,
                             RenderEngine_getTexture(self->field_27019_p, "/misc/footprint.png"));
    R_enable(R_BLEND);
    R_blendFunc(R_SRC_ALPHA, R_ONE_MINUS_SRC_ALPHA);
    Tessellator_startDrawingQuads(var1);
    Tessellator_setColorRGBA_F(var1, var14, var14, var14, var9);
    Tessellator_addVertexWithUV(var1, (double)(var11 - var10), (double)var12,
                                (double)(var13 + var10), 0.0, 1.0);
    Tessellator_addVertexWithUV(var1, (double)(var11 + var10), (double)var12,
                                (double)(var13 + var10), 1.0, 1.0);
    Tessellator_addVertexWithUV(var1, (double)(var11 + var10), (double)var12,
                                (double)(var13 - var10), 1.0, 0.0);
    Tessellator_addVertexWithUV(var1, (double)(var11 - var10), (double)var12,
                                (double)(var13 - var10), 0.0, 0.0);
    Tessellator_draw(var1);
    R_disable(R_BLEND);
    R_enable(R_LIGHTING);
}

void EntityFootStepFX_onUpdate(Entity *selfE) {
    EntityFootStepFX *self = (EntityFootStepFX *)selfE;
    ++self->field_27018_a;
    if (self->field_27018_a == self->field_27020_o) {
        Entity_setEntityDead(&self->base.base);
    }
}

int EntityFootStepFX_getFXLayer(EntityFX *self) {
    (void)self;
    return 3;
}
