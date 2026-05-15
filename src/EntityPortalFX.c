#include "JavaMath.h"
#include "EntityPortalFX.h"
#include "Entity.h"
#include "MathHelper.h"
#include <stdlib.h>

static EntityFXVtable EntityPortalFX_vtable;
static int EntityPortalFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityPortalFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityPortalFX_vtable);
    EntityPortalFX_vtable.base.onUpdate = EntityPortalFX_onUpdate;
    EntityPortalFX_vtable.base.getEntityBrightness = EntityPortalFX_getEntityBrightness;
    EntityPortalFX_vtable.renderParticle = EntityPortalFX_renderParticle;
    EntityPortalFX_vtable_init = 1;
}

void EntityPortalFX_construct(EntityPortalFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, var8, var10, var12);
    self->base.base.vtable = (const EntityVtable *)&EntityPortalFX_vtable;
    self->base.base.motionX = var8;
    self->base.base.motionY = var10;
    self->base.base.motionZ = var12;
    self->field_4086_p = self->base.base.posX = var2;
    self->field_4085_q = self->base.base.posY = var4;
    self->field_4084_r = self->base.base.posZ = var6;
    float var14 = JavaRandom_nextFloat(&self->base.base.rand) * 0.6F + 0.4F;
    self->field_4083_a = self->base.particleScale =
        JavaRandom_nextFloat(&self->base.base.rand) * 0.2F + 0.5F;
    self->base.particleRed = self->base.particleGreen = self->base.particleBlue = 1.0F * var14;
    self->base.particleGreen *= 0.3F;
    self->base.particleRed *= 0.9F;

    self->base.particleMaxAge = (int)(Math_random() * 10.0) + 40;
    self->base.base.noClip = 1;

    self->base.particleTextureIndex = (int)(Math_random() * 8.0);
}

EntityPortalFX *EntityPortalFX_create(World *var1, double var2, double var4, double var6,
                                      double var8, double var10, double var12) {
    EntityPortalFX *self = (EntityPortalFX *)calloc(1, sizeof(EntityPortalFX));
    EntityPortalFX_construct(self, var1, var2, var4, var6, var8, var10, var12);
    return self;
}

void EntityPortalFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                   float var4, float var5, float var6, float var7) {
    EntityPortalFX *self = (EntityPortalFX *)selfFX;
    float var8 = ((float)selfFX->particleAge + var2) / (float)selfFX->particleMaxAge;
    var8 = 1.0F - var8;
    var8 *= var8;
    var8 = 1.0F - var8;
    selfFX->particleScale = self->field_4083_a * var8;

    EntityFX_renderParticle(selfFX, var1, var2, var3, var4, var5, var6, var7);
}

float EntityPortalFX_getEntityBrightness(Entity *selfE, float var1) {
    EntityFX *selfFX = (EntityFX *)selfE;

    float var2 = Entity_getEntityBrightness(selfE, var1);
    float var3 = (float)selfFX->particleAge / (float)selfFX->particleMaxAge;
    var3 *= var3;
    var3 *= var3;
    return var2 * (1.0F - var3) + var3;
}

void EntityPortalFX_onUpdate(Entity *selfE) {
    EntityFX *selfFX = (EntityFX *)selfE;
    EntityPortalFX *self = (EntityPortalFX *)selfE;
    selfE->prevPosX = selfE->posX;
    selfE->prevPosY = selfE->posY;
    selfE->prevPosZ = selfE->posZ;
    float var1 = (float)selfFX->particleAge / (float)selfFX->particleMaxAge;
    float var2 = var1;
    var1 = -var1 + var1 * var1 * 2.0F;
    var1 = 1.0F - var1;
    selfE->posX = self->field_4086_p + selfE->motionX * (double)var1;
    selfE->posY = self->field_4085_q + selfE->motionY * (double)var1 + (double)(1.0F - var2);
    selfE->posZ = self->field_4084_r + selfE->motionZ * (double)var1;
    if (selfFX->particleAge++ >= selfFX->particleMaxAge) {
        selfE->vtable->setEntityDead(selfE);
    }
}
