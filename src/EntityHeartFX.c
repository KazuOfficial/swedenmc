#include "EntityHeartFX.h"
#include <stdlib.h>

static EntityFXVtable EntityHeartFX_vtable;
static int EntityHeartFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityHeartFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityHeartFX_vtable);
    EntityHeartFX_vtable.base.onUpdate = EntityHeartFX_onUpdate;
    EntityHeartFX_vtable.renderParticle = EntityHeartFX_renderParticle;
    EntityHeartFX_vtable_init = 1;
}

void EntityHeartFX_construct2(EntityHeartFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12, float var14) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, 0.0, 0.0, 0.0);
    self->base.base.vtable = (const EntityVtable *)&EntityHeartFX_vtable;
    (void)var8;
    (void)var10;
    (void)var12;
    self->base.base.motionX *= (double)0.01F;
    self->base.base.motionY *= (double)0.01F;
    self->base.base.motionZ *= (double)0.01F;
    self->base.base.motionY += 0.1;
    self->base.particleScale *= 12.0F / 16.0F;
    self->base.particleScale *= var14;
    self->field_25022_a = self->base.particleScale;
    self->base.particleMaxAge = 16;
    self->base.base.noClip = 0;
    self->base.particleTextureIndex = 80;
}

void EntityHeartFX_construct1(EntityHeartFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12) {
    EntityHeartFX_construct2(self, var1, var2, var4, var6, var8, var10, var12, 2.0F);
}

EntityHeartFX *EntityHeartFX_create(World *var1, double var2, double var4, double var6, double var8,
                                    double var10, double var12) {
    EntityHeartFX *self = (EntityHeartFX *)calloc(1, sizeof(EntityHeartFX));
    EntityHeartFX_construct1(self, var1, var2, var4, var6, var8, var10, var12);
    return self;
}

void EntityHeartFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                  float var4, float var5, float var6, float var7) {
    EntityHeartFX *self = (EntityHeartFX *)selfFX;
    float var8 = ((float)selfFX->particleAge + var2) / (float)selfFX->particleMaxAge * 32.0F;
    if (var8 < 0.0F) {
        var8 = 0.0F;
    }
    if (var8 > 1.0F) {
        var8 = 1.0F;
    }
    selfFX->particleScale = self->field_25022_a * var8;
    EntityFX_renderParticle(selfFX, var1, var2, var3, var4, var5, var6, var7);
}

void EntityHeartFX_onUpdate(Entity *selfE) {
    EntityHeartFX *self = (EntityHeartFX *)selfE;
    EntityFX *selfFX = &self->base;
    selfE->prevPosX = selfE->posX;
    selfE->prevPosY = selfE->posY;
    selfE->prevPosZ = selfE->posZ;
    if (selfFX->particleAge++ >= selfFX->particleMaxAge) {
        Entity_setEntityDead(selfE);
    }
    selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);
    if (selfE->posY == selfE->prevPosY) {
        selfE->motionX *= 1.1;
        selfE->motionZ *= 1.1;
    }
    selfE->motionX *= (double)0.86F;
    selfE->motionY *= (double)0.86F;
    selfE->motionZ *= (double)0.86F;
    if (selfE->onGround) {
        selfE->motionX *= (double)0.7F;
        selfE->motionZ *= (double)0.7F;
    }
}
