#include "JavaMath.h"
#include "EntityBubbleFX.h"
#include "EntityFX.h"
#include "Entity.h"
#include "World.h"
#include "Material.h"
#include "MathHelper.h"

static EntityFXVtable EntityBubbleFX_vtable;
static int EntityBubbleFX_vtable_init = 0;

static void ensure_vtable(void) {
    if (!EntityBubbleFX_vtable_init) {
        EntityFX_initVtable(&EntityBubbleFX_vtable);
        EntityBubbleFX_vtable.base.onUpdate = (void (*)(Entity *))EntityBubbleFX_onUpdate;
        EntityBubbleFX_vtable_init = 1;
    }
}

void EntityBubbleFX_construct(EntityBubbleFX *self, World *var1, double var2, double var4,
                              double var6, double var8, double var10, double var12) {
    ensure_vtable();
    EntityFX_construct(&self->base, var1, var2, var4, var6, var8, var10, var12);
    self->base.base.vtable = (const EntityVtable *)&EntityBubbleFX_vtable;

    self->base.particleRed = 1.0F;
    self->base.particleGreen = 1.0F;
    self->base.particleBlue = 1.0F;
    self->base.particleTextureIndex = 32;
    Entity_setSize(&self->base.base, 0.02F, 0.02F);
    self->base.particleScale *= JavaRandom_nextFloat(&self->base.base.rand) * 0.6F + 0.2F;
    self->base.base.motionX =
        var8 * (double)0.2F + (double)((float)(Math_random() * 2.0 - 1.0) * 0.02F);
    self->base.base.motionY =
        var10 * (double)0.2F + (double)((float)(Math_random() * 2.0 - 1.0) * 0.02F);
    self->base.base.motionZ =
        var12 * (double)0.2F + (double)((float)(Math_random() * 2.0 - 1.0) * 0.02F);
    self->base.particleMaxAge = (int)(8.0 / (Math_random() * 0.8 + 0.2));
}

void EntityBubbleFX_onUpdate(EntityBubbleFX *self) {
    self->base.base.prevPosX = self->base.base.posX;
    self->base.base.prevPosY = self->base.base.posY;
    self->base.base.prevPosZ = self->base.base.posZ;
    self->base.base.motionY += 0.002;
    self->base.base.vtable->moveEntity(&self->base.base, self->base.base.motionX,
                                       self->base.base.motionY, self->base.base.motionZ);
    self->base.base.motionX *= (double)0.85F;
    self->base.base.motionY *= (double)0.85F;
    self->base.base.motionZ *= (double)0.85F;
    if (World_getBlockMaterial(self->base.base.worldObj,
                               MathHelper_floor_double(self->base.base.posX),
                               MathHelper_floor_double(self->base.base.posY),
                               MathHelper_floor_double(self->base.base.posZ)) != Material_water) {
        Entity_setEntityDead(&self->base.base);
    }

    if (self->base.particleMaxAge-- <= 0) {
        Entity_setEntityDead(&self->base.base);
    }
}
