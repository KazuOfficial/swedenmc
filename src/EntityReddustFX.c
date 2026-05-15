#include "JavaMath.h"
#include "EntityReddustFX.h"
#include "MathHelper.h"
#include <stdlib.h>

static EntityFXVtable EntityReddustFX_vtable;
static int EntityReddustFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityReddustFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityReddustFX_vtable);
    EntityReddustFX_vtable.base.onUpdate = EntityReddustFX_onUpdate;
    EntityReddustFX_vtable.renderParticle = EntityReddustFX_renderParticle;
    EntityReddustFX_vtable_init = 1;
}

void EntityReddustFX_construct1(EntityReddustFX *self, World *var1, double var2, double var4,
                                double var6, float var8, float var9, float var10) {
    EntityReddustFX_construct2(self, var1, var2, var4, var6, 1.0F, var8, var9, var10);
}

void EntityReddustFX_construct2(EntityReddustFX *self, World *var1, double var2, double var4,
                                double var6, float var8, float var9, float var10, float var11) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, 0.0, 0.0, 0.0);
    self->base.base.vtable = (const EntityVtable *)&EntityReddustFX_vtable;
    self->base.base.motionX *= (double)0.1F;
    self->base.base.motionY *= (double)0.1F;
    self->base.base.motionZ *= (double)0.1F;
    if (var9 == 0.0F) {
        var9 = 1.0F;
    }

    float var12 = (float)(Math_random()) * 0.4F + 0.6F;

    self->base.particleRed = ((float)(Math_random() * (double)0.2F) + 0.8F) * var9 * var12;

    self->base.particleGreen = ((float)(Math_random() * (double)0.2F) + 0.8F) * var10 * var12;

    self->base.particleBlue = ((float)(Math_random() * (double)0.2F) + 0.8F) * var11 * var12;
    self->base.particleScale *= 12.0F / 16.0F;
    self->base.particleScale *= var8;
    self->field_673_a = self->base.particleScale;

    self->base.particleMaxAge = (int)(8.0 / (Math_random() * 0.8 + 0.2));

    self->base.particleMaxAge = (int)((float)self->base.particleMaxAge * var8);
    self->base.base.noClip = 0;
}

EntityReddustFX *EntityReddustFX_create1(World *var1, double var2, double var4, double var6,
                                         float var8, float var9, float var10) {
    EntityReddustFX *self = (EntityReddustFX *)calloc(1, sizeof(EntityReddustFX));
    EntityReddustFX_construct1(self, var1, var2, var4, var6, var8, var9, var10);
    return self;
}

EntityReddustFX *EntityReddustFX_create2(World *var1, double var2, double var4, double var6,
                                         float var8, float var9, float var10, float var11) {
    EntityReddustFX *self = (EntityReddustFX *)calloc(1, sizeof(EntityReddustFX));
    EntityReddustFX_construct2(self, var1, var2, var4, var6, var8, var9, var10, var11);
    return self;
}

void EntityReddustFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                    float var4, float var5, float var6, float var7) {
    EntityReddustFX *self = (EntityReddustFX *)selfFX;
    float var8 = ((float)selfFX->particleAge + var2) / (float)selfFX->particleMaxAge * 32.0F;
    if (var8 < 0.0F) {
        var8 = 0.0F;
    }
    if (var8 > 1.0F) {
        var8 = 1.0F;
    }
    selfFX->particleScale = self->field_673_a * var8;

    EntityFX_renderParticle(selfFX, var1, var2, var3, var4, var5, var6, var7);
}

void EntityReddustFX_onUpdate(Entity *selfE) {
    EntityFX *selfFX = (EntityFX *)selfE;
    selfE->prevPosX = selfE->posX;
    selfE->prevPosY = selfE->posY;
    selfE->prevPosZ = selfE->posZ;
    if (selfFX->particleAge++ >= selfFX->particleMaxAge) {
        selfE->vtable->setEntityDead(selfE);
    }
    selfFX->particleTextureIndex = 7 - selfFX->particleAge * 8 / selfFX->particleMaxAge;

    selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);
    if (selfE->posY == selfE->prevPosY) {
        selfE->motionX *= 1.1;
        selfE->motionZ *= 1.1;
    }
    selfE->motionX *= (double)0.96F;
    selfE->motionY *= (double)0.96F;
    selfE->motionZ *= (double)0.96F;
    if (selfE->onGround) {
        selfE->motionX *= (double)0.7F;
        selfE->motionZ *= (double)0.7F;
    }
}
