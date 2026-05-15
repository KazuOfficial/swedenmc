#include "JavaMath.h"
#include "EntitySmokeFX.h"
#include "MathHelper.h"
#include <stdlib.h>

static EntityFXVtable EntitySmokeFX_vtable;
static int EntitySmokeFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntitySmokeFX_vtable_init)
        return;
    EntityFX_initVtable(&EntitySmokeFX_vtable);
    EntitySmokeFX_vtable.base.onUpdate = EntitySmokeFX_onUpdate;
    EntitySmokeFX_vtable.renderParticle = EntitySmokeFX_renderParticle;
    EntitySmokeFX_vtable_init = 1;
}

void EntitySmokeFX_construct1(EntitySmokeFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12) {
    EntitySmokeFX_construct2(self, var1, var2, var4, var6, var8, var10, var12, 1.0F);
}

void EntitySmokeFX_construct2(EntitySmokeFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12, float var14) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, 0.0, 0.0, 0.0);
    self->base.base.vtable = (const EntityVtable *)&EntitySmokeFX_vtable;
    self->base.base.motionX *= (double)0.1F;
    self->base.base.motionY *= (double)0.1F;
    self->base.base.motionZ *= (double)0.1F;
    self->base.base.motionX += var8;
    self->base.base.motionY += var10;
    self->base.base.motionZ += var12;

    self->base.particleRed = self->base.particleGreen = self->base.particleBlue =
        (float)(Math_random() * (double)0.3F);
    self->base.particleScale *= 12.0F / 16.0F;
    self->base.particleScale *= var14;
    self->field_671_a = self->base.particleScale;

    self->base.particleMaxAge = (int)(8.0 / (Math_random() * 0.8 + 0.2));

    self->base.particleMaxAge = (int)((float)self->base.particleMaxAge * var14);
    self->base.base.noClip = 0;
}

EntitySmokeFX *EntitySmokeFX_create1(World *var1, double var2, double var4, double var6,
                                     double var8, double var10, double var12) {
    EntitySmokeFX *self = (EntitySmokeFX *)calloc(1, sizeof(EntitySmokeFX));
    EntitySmokeFX_construct1(self, var1, var2, var4, var6, var8, var10, var12);
    return self;
}

EntitySmokeFX *EntitySmokeFX_create2(World *var1, double var2, double var4, double var6,
                                     double var8, double var10, double var12, float var14) {
    EntitySmokeFX *self = (EntitySmokeFX *)calloc(1, sizeof(EntitySmokeFX));
    EntitySmokeFX_construct2(self, var1, var2, var4, var6, var8, var10, var12, var14);
    return self;
}

void EntitySmokeFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                  float var4, float var5, float var6, float var7) {
    EntitySmokeFX *self = (EntitySmokeFX *)selfFX;
    float var8 = ((float)selfFX->particleAge + var2) / (float)selfFX->particleMaxAge * 32.0F;
    if (var8 < 0.0F) {
        var8 = 0.0F;
    }
    if (var8 > 1.0F) {
        var8 = 1.0F;
    }
    selfFX->particleScale = self->field_671_a * var8;

    EntityFX_renderParticle(selfFX, var1, var2, var3, var4, var5, var6, var7);
}

void EntitySmokeFX_onUpdate(Entity *selfE) {
    EntityFX *selfFX = (EntityFX *)selfE;
    selfE->prevPosX = selfE->posX;
    selfE->prevPosY = selfE->posY;
    selfE->prevPosZ = selfE->posZ;
    if (selfFX->particleAge++ >= selfFX->particleMaxAge) {
        selfE->vtable->setEntityDead(selfE);
    }
    selfFX->particleTextureIndex = 7 - selfFX->particleAge * 8 / selfFX->particleMaxAge;
    selfE->motionY += 0.004;

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
