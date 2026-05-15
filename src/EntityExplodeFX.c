#include "JavaMath.h"
#include "EntityExplodeFX.h"
#include "EntityFX.h"
#include "Entity.h"
#include "MathHelper.h"

static EntityFXVtable EntityExplodeFX_vtable;
static int EntityExplodeFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (!EntityExplodeFX_vtable_init) {
        EntityFX_initVtable(&EntityExplodeFX_vtable);
        EntityExplodeFX_vtable.base.onUpdate = (void (*)(Entity *))EntityExplodeFX_onUpdate;
        EntityExplodeFX_vtable.renderParticle =
            (void (*)(EntityFX *, Tessellator *, float, float, float, float, float,
                      float))EntityExplodeFX_renderParticle;
        EntityExplodeFX_vtable_init = 1;
    }
}

void EntityExplodeFX_construct(EntityExplodeFX *self, World *var1, double var2, double var4,
                               double var6, double var8, double var10, double var12) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, var8, var10, var12);
    self->base.base.vtable = (const EntityVtable *)&EntityExplodeFX_vtable;

    self->base.base.motionX = var8 + (double)((float)(Math_random() * 2.0 - 1.0) * 0.05F);
    self->base.base.motionY = var10 + (double)((float)(Math_random() * 2.0 - 1.0) * 0.05F);
    self->base.base.motionZ = var12 + (double)((float)(Math_random() * 2.0 - 1.0) * 0.05F);
    self->base.particleRed = self->base.particleGreen = self->base.particleBlue =
        JavaRandom_nextFloat(&self->base.base.rand) * 0.3F + 0.7F;
    self->base.particleScale = JavaRandom_nextFloat(&self->base.base.rand) *
                                   JavaRandom_nextFloat(&self->base.base.rand) * 6.0F +
                               1.0F;
    self->base.particleMaxAge =
        (int)(16.0 / ((double)JavaRandom_nextFloat(&self->base.base.rand) * 0.8 + 0.2)) + 2;
}

void EntityExplodeFX_renderParticle(EntityExplodeFX *self, Tessellator *var1, float var2,
                                    float var3, float var4, float var5, float var6, float var7) {

    EntityFX_renderParticle(&self->base, var1, var2, var3, var4, var5, var6, var7);
}

void EntityExplodeFX_onUpdate(EntityExplodeFX *self) {
    self->base.base.prevPosX = self->base.base.posX;
    self->base.base.prevPosY = self->base.base.posY;
    self->base.base.prevPosZ = self->base.base.posZ;
    if (self->base.particleAge++ >= self->base.particleMaxAge) {
        Entity_setEntityDead(&self->base.base);
    }

    self->base.particleTextureIndex = 7 - self->base.particleAge * 8 / self->base.particleMaxAge;
    self->base.base.motionY += 0.004;
    self->base.base.vtable->moveEntity(&self->base.base, self->base.base.motionX,
                                       self->base.base.motionY, self->base.base.motionZ);
    self->base.base.motionX *= (double)0.9F;
    self->base.base.motionY *= (double)0.9F;
    self->base.base.motionZ *= (double)0.9F;
    if (self->base.base.onGround) {
        self->base.base.motionX *= (double)0.7F;
        self->base.base.motionZ *= (double)0.7F;
    }
}
