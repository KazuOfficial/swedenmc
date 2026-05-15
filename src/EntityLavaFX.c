#include "JavaMath.h"
#include "EntityLavaFX.h"
#include "MathHelper.h"
#include "World.h"
#include <stdlib.h>

static EntityFXVtable EntityLavaFX_vtable;
static int EntityLavaFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityLavaFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityLavaFX_vtable);
    EntityLavaFX_vtable.base.getEntityBrightness = EntityLavaFX_getEntityBrightness;
    EntityLavaFX_vtable.base.onUpdate = EntityLavaFX_onUpdate;
    EntityLavaFX_vtable.renderParticle = EntityLavaFX_renderParticle;
    EntityLavaFX_vtable_init = 1;
}

void EntityLavaFX_construct(EntityLavaFX *self, World *var1, double var2, double var4,
                            double var6) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, 0.0, 0.0, 0.0);
    self->base.base.vtable = (const EntityVtable *)&EntityLavaFX_vtable;
    self->base.base.motionX *= (double)0.8F;
    self->base.base.motionY *= (double)0.8F;
    self->base.base.motionZ *= (double)0.8F;
    self->base.base.motionY = (double)(JavaRandom_nextFloat(&self->base.base.rand) * 0.4F + 0.05F);
    self->base.particleRed = self->base.particleGreen = self->base.particleBlue = 1.0F;
    self->base.particleScale *= JavaRandom_nextFloat(&self->base.base.rand) * 2.0F + 0.2F;
    self->field_674_a = self->base.particleScale;
    self->base.particleMaxAge = (int)(16.0 / (Math_random() * 0.8 + 0.2));
    self->base.base.noClip = 0;
    self->base.particleTextureIndex = 49;
}

EntityLavaFX *EntityLavaFX_create(World *var1, double var2, double var4, double var6) {
    EntityLavaFX *self = (EntityLavaFX *)calloc(1, sizeof(EntityLavaFX));
    EntityLavaFX_construct(self, var1, var2, var4, var6);
    return self;
}

float EntityLavaFX_getEntityBrightness(Entity *self, float var1) {
    (void)self;
    (void)var1;
    return 1.0F;
}

void EntityLavaFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                 float var4, float var5, float var6, float var7) {
    EntityLavaFX *self = (EntityLavaFX *)selfFX;
    float var8 = ((float)selfFX->particleAge + var2) / (float)selfFX->particleMaxAge;
    selfFX->particleScale = self->field_674_a * (1.0F - var8 * var8);
    EntityFX_renderParticle(selfFX, var1, var2, var3, var4, var5, var6, var7);
}

void EntityLavaFX_onUpdate(Entity *selfE) {
    EntityLavaFX *self = (EntityLavaFX *)selfE;
    EntityFX *selfFX = &self->base;
    selfE->prevPosX = selfE->posX;
    selfE->prevPosY = selfE->posY;
    selfE->prevPosZ = selfE->posZ;
    if (selfFX->particleAge++ >= selfFX->particleMaxAge) {
        Entity_setEntityDead(selfE);
    }
    float var1 = (float)selfFX->particleAge / (float)selfFX->particleMaxAge;
    if (JavaRandom_nextFloat(&selfE->rand) > var1) {
        World_spawnParticle(selfE->worldObj, "smoke", selfE->posX, selfE->posY, selfE->posZ,
                            selfE->motionX, selfE->motionY, selfE->motionZ);
    }
    selfE->motionY -= 0.03;
    selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);
    selfE->motionX *= (double)0.999F;
    selfE->motionY *= (double)0.999F;
    selfE->motionZ *= (double)0.999F;
    if (selfE->onGround) {
        selfE->motionX *= (double)0.7F;
        selfE->motionZ *= (double)0.7F;
    }
}
