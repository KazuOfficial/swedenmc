#include "JavaMath.h"
#include "EntityRainFX.h"
#include "World.h"
#include "MathHelper.h"
#include "Material.h"
#include "BlockFluid.h"
#include <stdlib.h>

static EntityFXVtable EntityRainFX_vtable;
static int EntityRainFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (EntityRainFX_vtable_init)
        return;
    EntityFX_initVtable(&EntityRainFX_vtable);
    EntityRainFX_vtable.base.onUpdate = EntityRainFX_onUpdate;
    EntityRainFX_vtable.renderParticle = EntityRainFX_renderParticle;
    EntityRainFX_vtable_init = 1;
}

void EntityRainFX_construct(EntityRainFX *self, World *var1, double var2, double var4,
                            double var6) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, 0.0, 0.0, 0.0);
    self->base.base.vtable = (const EntityVtable *)&EntityRainFX_vtable;
    self->base.base.motionX *= (double)0.3F;

    self->base.base.motionY = (double)((float)(Math_random()) * 0.2F + 0.1F);
    self->base.base.motionZ *= (double)0.3F;
    self->base.particleRed = 1.0F;
    self->base.particleGreen = 1.0F;
    self->base.particleBlue = 1.0F;
    self->base.particleTextureIndex = 19 + JavaRandom_nextInt(&self->base.base.rand, 4);
    Entity_setSize(&self->base.base, 0.01F, 0.01F);
    self->base.particleGravity = 0.06F;

    self->base.particleMaxAge = (int)(8.0 / (Math_random() * 0.8 + 0.2));
}

EntityRainFX *EntityRainFX_create(World *var1, double var2, double var4, double var6) {
    EntityRainFX *self = (EntityRainFX *)calloc(1, sizeof(EntityRainFX));
    EntityRainFX_construct(self, var1, var2, var4, var6);
    return self;
}

void EntityRainFX_renderParticle(EntityFX *selfFX, Tessellator *var1, float var2, float var3,
                                 float var4, float var5, float var6, float var7) {

    EntityFX_renderParticle(selfFX, var1, var2, var3, var4, var5, var6, var7);
}

void EntityRainFX_onUpdate(Entity *selfE) {
    EntityFX *selfFX = (EntityFX *)selfE;
    selfE->prevPosX = selfE->posX;
    selfE->prevPosY = selfE->posY;
    selfE->prevPosZ = selfE->posZ;
    selfE->motionY -= (double)selfFX->particleGravity;

    selfE->vtable->moveEntity(selfE, selfE->motionX, selfE->motionY, selfE->motionZ);
    selfE->motionX *= (double)0.98F;
    selfE->motionY *= (double)0.98F;
    selfE->motionZ *= (double)0.98F;
    if (selfFX->particleMaxAge-- <= 0) {
        selfE->vtable->setEntityDead(selfE);
    }
    if (selfE->onGround) {

        if (Math_random() < 0.5) {
            selfE->vtable->setEntityDead(selfE);
        }
        selfE->motionX *= (double)0.7F;
        selfE->motionZ *= (double)0.7F;
    }

    Material *var1 = World_getBlockMaterial(selfE->worldObj, MathHelper_floor_double(selfE->posX),
                                            MathHelper_floor_double(selfE->posY),
                                            MathHelper_floor_double(selfE->posZ));
    if (Material_getIsLiquid(var1) || Material_isSolid(var1)) {

        double var2 = (double)((float)(MathHelper_floor_double(selfE->posY) + 1) -
                               BlockFluid_getPercentAir(World_getBlockMetadata(
                                   selfE->worldObj, MathHelper_floor_double(selfE->posX),
                                   MathHelper_floor_double(selfE->posY),
                                   MathHelper_floor_double(selfE->posZ))));
        if (selfE->posY < var2) {
            selfE->vtable->setEntityDead(selfE);
        }
    }
}
