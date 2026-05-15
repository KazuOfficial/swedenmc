#include "Renderer.h"
#include "EntityPickupFX.h"
#include "MathHelper.h"
#include "World.h"
#include "RenderManager.h"
#include <stdlib.h>

static EntityFXVtable EntityPickupFX_vtable;
static int EntityPickupFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityPickupFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityPickupFX_vtable);
    EntityPickupFX_vtable.base.onUpdate = EntityPickupFX_onUpdate;
    EntityPickupFX_vtable.renderParticle = EntityPickupFX_renderParticle;
    EntityPickupFX_vtable.getFXLayer = EntityPickupFX_getFXLayer;
    EntityPickupFX_vtable_init = 1;
}

void EntityPickupFX_construct(EntityPickupFX *self, World *var1, Entity *var2, Entity *var3,
                              float var4) {
    ensure_vtable();

    EntityFX_construct(&self->base, var1, var2->posX, var2->posY, var2->posZ, var2->motionX,
                       var2->motionY, var2->motionZ);
    self->base.base.vtable = (const EntityVtable *)&EntityPickupFX_vtable;
    self->field_675_a = var2;
    self->field_679_o = var3;
    self->field_678_p = 0;
    self->field_677_q = 3;
    self->field_676_r = var4;
}

EntityPickupFX *EntityPickupFX_create(World *var1, Entity *var2, Entity *var3, float var4) {
    EntityPickupFX *self = (EntityPickupFX *)calloc(1, sizeof(EntityPickupFX));
    EntityPickupFX_construct(self, var1, var2, var3, var4);
    return self;
}

void EntityPickupFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                   float var4, float var5, float var6, float var7) {
    (void)var1;
    (void)var3;
    (void)var4;
    (void)var5;
    (void)var6;
    (void)var7;
    EntityPickupFX *self = (EntityPickupFX *)selfFX;
    Entity *selfE = &selfFX->base;
    float var8 = ((float)self->field_678_p + var2) / (float)self->field_677_q;
    var8 *= var8;
    double var9 = self->field_675_a->posX;
    double var11 = self->field_675_a->posY;
    double var13 = self->field_675_a->posZ;
    double var15 = self->field_679_o->lastTickPosX +
                   (self->field_679_o->posX - self->field_679_o->lastTickPosX) * (double)var2;
    double var17 = self->field_679_o->lastTickPosY +
                   (self->field_679_o->posY - self->field_679_o->lastTickPosY) * (double)var2 +
                   (double)self->field_676_r;
    double var19 = self->field_679_o->lastTickPosZ +
                   (self->field_679_o->posZ - self->field_679_o->lastTickPosZ) * (double)var2;
    double var21 = var9 + (var15 - var9) * (double)var8;
    double var23 = var11 + (var17 - var11) * (double)var8;
    double var25 = var13 + (var19 - var13) * (double)var8;
    int var27 = MathHelper_floor_double(var21);
    int var28 = MathHelper_floor_double(var23 + (double)(selfE->yOffset / 2.0F));
    int var29 = MathHelper_floor_double(var25);
    float var30 =
        (float)selfE->worldObj->vtable->getLightBrightness(selfE->worldObj, var27, var28, var29);
    var21 -= EntityFX_interpPosX;
    var23 -= EntityFX_interpPosY;
    var25 -= EntityFX_interpPosZ;

    R_color4f(var30, var30, var30, 1.0F);

    RenderManager_renderEntityWithPosYaw(
        RenderManager_instance, self->field_675_a, (double)((float)var21), (double)((float)var23),
        (double)((float)var25), self->field_675_a->rotationYaw, var2);
}

void EntityPickupFX_onUpdate(Entity *selfE) {
    EntityPickupFX *self = (EntityPickupFX *)selfE;
    ++self->field_678_p;
    if (self->field_678_p == self->field_677_q) {
        selfE->vtable->setEntityDead(selfE);
    }
}

int EntityPickupFX_getFXLayer(EntityFX *self) {
    (void)self;
    return 3;
}
