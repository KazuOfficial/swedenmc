#include "JavaMath.h"
#include "EntityFlameFX.h"
#include "World.h"
#include "MathHelper.h"
#include <stdlib.h>

static EntityFXVtable EntityFlameFX_vtable;
static int EntityFlameFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityFlameFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityFlameFX_vtable);
    EntityFlameFX_vtable.renderParticle =
        (void (*)(EntityFX *, Tessellator *, float, float, float, float, float,
                  float))EntityFlameFX_renderParticle;
    EntityFlameFX_vtable.base.onUpdate = EntityFlameFX_onUpdate;
    EntityFlameFX_vtable.base.getEntityBrightness = EntityFlameFX_getEntityBrightness;
    EntityFlameFX_vtable_init = 1;
}

void EntityFlameFX_construct(EntityFlameFX *self, World *var1, double var2, double var4,
                             double var6, double var8, double var10, double var12) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, var8, var10, var12);
    self->base.base.vtable = (const EntityVtable *)&EntityFlameFX_vtable;

    self->base.base.motionX = self->base.base.motionX * (double)0.01F + var8;
    self->base.base.motionY = self->base.base.motionY * (double)0.01F + var10;
    self->base.base.motionZ = self->base.base.motionZ * (double)0.01F + var12;

    double var10000 = var2 + (double)((JavaRandom_nextFloat(&self->base.base.rand) -
                                       JavaRandom_nextFloat(&self->base.base.rand)) *
                                      0.05F);
    var10000 = var4 + (double)((JavaRandom_nextFloat(&self->base.base.rand) -
                                JavaRandom_nextFloat(&self->base.base.rand)) *
                               0.05F);
    var10000 = var6 + (double)((JavaRandom_nextFloat(&self->base.base.rand) -
                                JavaRandom_nextFloat(&self->base.base.rand)) *
                               0.05F);
    (void)var10000;

    self->field_672_a = self->base.particleScale;
    self->base.particleRed = self->base.particleGreen = self->base.particleBlue = 1.0F;
    self->base.particleMaxAge = (int)(8.0 / (Math_random() * 0.8 + 0.2)) + 4;
    self->base.base.noClip = 1;
    self->base.particleTextureIndex = 48;
}

EntityFlameFX *EntityFlameFX_create(World *var1, double var2, double var4, double var6, double var8,
                                    double var10, double var12) {
    EntityFlameFX *self = (EntityFlameFX *)calloc(1, sizeof(EntityFlameFX));
    EntityFlameFX_construct(self, var1, var2, var4, var6, var8, var10, var12);
    return self;
}

void EntityFlameFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                  float var4, float var5, float var6, float var7) {
    EntityFlameFX *self = (EntityFlameFX *)selfFX;
    float var8 = ((float)self->base.particleAge + var2) / (float)self->base.particleMaxAge;
    self->base.particleScale = self->field_672_a * (1.0F - var8 * var8 * 0.5F);
    EntityFX_renderParticle(&self->base, var1, var2, var3, var4, var5, var6, var7);
}

float EntityFlameFX_getEntityBrightness(Entity *selfE, float var1) {
    EntityFlameFX *self = (EntityFlameFX *)selfE;
    float var2 = ((float)self->base.particleAge + var1) / (float)self->base.particleMaxAge;
    if (var2 < 0.0F) {
        var2 = 0.0F;
    }

    if (var2 > 1.0F) {
        var2 = 1.0F;
    }

    float var3 = Entity_getEntityBrightness(selfE, var1);
    return var3 * var2 + (1.0F - var2);
}

void EntityFlameFX_onUpdate(Entity *selfE) {
    EntityFlameFX *self = (EntityFlameFX *)selfE;
    self->base.base.prevPosX = self->base.base.posX;
    self->base.base.prevPosY = self->base.base.posY;
    self->base.base.prevPosZ = self->base.base.posZ;
    if (self->base.particleAge++ >= self->base.particleMaxAge) {
        Entity_setEntityDead(&self->base.base);
    }

    self->base.base.vtable->moveEntity(&self->base.base, self->base.base.motionX,
                                       self->base.base.motionY, self->base.base.motionZ);
    self->base.base.motionX *= (double)0.96F;
    self->base.base.motionY *= (double)0.96F;
    self->base.base.motionZ *= (double)0.96F;
    if (self->base.base.onGround) {
        self->base.base.motionX *= (double)0.7F;
        self->base.base.motionZ *= (double)0.7F;
    }
}
