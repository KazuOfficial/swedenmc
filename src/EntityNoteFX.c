#include "EntityNoteFX.h"
#include "MathHelper.h"
#include <math.h>
#include <stdlib.h>

static EntityFXVtable EntityNoteFX_vtable;
static int EntityNoteFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityNoteFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityNoteFX_vtable);
    EntityNoteFX_vtable.base.onUpdate = EntityNoteFX_onUpdate;
    EntityNoteFX_vtable.renderParticle = EntityNoteFX_renderParticle;
    EntityNoteFX_vtable_init = 1;
}

void EntityNoteFX_construct2(EntityNoteFX *self, World *var1, double var2, double var4, double var6,
                             double var8, double var10, double var12, float var14) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, 0.0, 0.0, 0.0);
    self->base.base.vtable = (const EntityVtable *)&EntityNoteFX_vtable;
    (void)var10;
    (void)var12;
    self->base.base.motionX *= (double)0.01F;
    self->base.base.motionY *= (double)0.01F;
    self->base.base.motionZ *= (double)0.01F;
    self->base.base.motionY += 0.2;
    self->base.particleRed =
        MathHelper_sin(((float)var8 + 0.0F) * (float)M_PI * 2.0F) * 0.65F + 0.35F;
    self->base.particleGreen =
        MathHelper_sin(((float)var8 + 1.0F / 3.0F) * (float)M_PI * 2.0F) * 0.65F + 0.35F;
    self->base.particleBlue =
        MathHelper_sin(((float)var8 + 2.0F / 3.0F) * (float)M_PI * 2.0F) * 0.65F + 0.35F;
    self->base.particleScale *= 12.0F / 16.0F;
    self->base.particleScale *= var14;
    self->field_21065_a = self->base.particleScale;
    self->base.particleMaxAge = 6;
    self->base.base.noClip = 0;
    self->base.particleTextureIndex = 64;
}

void EntityNoteFX_construct1(EntityNoteFX *self, World *var1, double var2, double var4, double var6,
                             double var8, double var10, double var12) {
    EntityNoteFX_construct2(self, var1, var2, var4, var6, var8, var10, var12, 2.0F);
}

EntityNoteFX *EntityNoteFX_create(World *var1, double var2, double var4, double var6, double var8,
                                  double var10, double var12) {
    EntityNoteFX *self = (EntityNoteFX *)calloc(1, sizeof(EntityNoteFX));
    EntityNoteFX_construct1(self, var1, var2, var4, var6, var8, var10, var12);
    return self;
}

void EntityNoteFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                 float var4, float var5, float var6, float var7) {
    EntityNoteFX *self = (EntityNoteFX *)selfFX;
    float var8 = ((float)selfFX->particleAge + var2) / (float)selfFX->particleMaxAge * 32.0F;
    if (var8 < 0.0F) {
        var8 = 0.0F;
    }
    if (var8 > 1.0F) {
        var8 = 1.0F;
    }
    selfFX->particleScale = self->field_21065_a * var8;
    EntityFX_renderParticle(selfFX, var1, var2, var3, var4, var5, var6, var7);
}

void EntityNoteFX_onUpdate(Entity *selfE) {
    EntityNoteFX *self = (EntityNoteFX *)selfE;
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
    selfE->motionX *= (double)0.66F;
    selfE->motionY *= (double)0.66F;
    selfE->motionZ *= (double)0.66F;
    if (selfE->onGround) {
        selfE->motionX *= (double)0.7F;
        selfE->motionZ *= (double)0.7F;
    }
}
