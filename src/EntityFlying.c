#include "EntityFlying.h"
#include "World.h"
#include "Block.h"
#include "MathHelper.h"
#include <stdlib.h>

EntityLivingVtable EntityFlying_defaultVtable;
static int EntityFlying_vtable_init = 0;

void EntityFlying_initVtable(EntityLivingVtable *vt) {
    EntityLiving_initVtable(vt);
    vt->base.fall = EntityFlying_fall;
    vt->moveEntityWithHeading = EntityFlying_moveEntityWithHeading;
    vt->isOnLadder = EntityFlying_isOnLadder;
}

static void ensure_vtable(void) {
    if (EntityFlying_vtable_init)
        return;
    EntityFlying_initVtable(&EntityFlying_defaultVtable);
    EntityFlying_vtable_init = 1;
}

void EntityFlying_construct(EntityFlying *self, World *var1) {
    ensure_vtable();
    EntityLiving_construct(&self->base, var1);
    self->base.base.vtable = (const EntityVtable *)&EntityFlying_defaultVtable;
}

EntityFlying *EntityFlying_create(World *var1) {
    EntityFlying *self = (EntityFlying *)calloc(1, sizeof(EntityFlying));
    EntityFlying_construct(self, var1);
    return self;
}

void EntityFlying_fall(Entity *self, float var1) {
    (void)self;
    (void)var1;
}

void EntityFlying_moveEntityWithHeading(EntityLiving *selfL, float var1, float var2) {
    Entity *self = &selfL->base;
    if (Entity_isInWater(self)) {
        Entity_moveFlying(self, var1, var2, 0.02F);
        self->vtable->moveEntity(self, self->motionX, self->motionY, self->motionZ);
        self->motionX *= (double)0.8F;
        self->motionY *= (double)0.8F;
        self->motionZ *= (double)0.8F;
    } else if (Entity_handleLavaMovement(self)) {
        Entity_moveFlying(self, var1, var2, 0.02F);
        self->vtable->moveEntity(self, self->motionX, self->motionY, self->motionZ);
        self->motionX *= 0.5;
        self->motionY *= 0.5;
        self->motionZ *= 0.5;
    } else {
        float var3 = 0.91F;
        if (self->onGround) {
            var3 = 546.0F * 0.1F * 0.1F * 0.1F;
            int var4 = self->worldObj->vtable->getBlockId(
                self->worldObj, MathHelper_floor_double(self->posX),
                MathHelper_floor_double(self->boundingBox.minY) - 1,
                MathHelper_floor_double(self->posZ));
            if (var4 > 0 && Block_blocksList[var4]) {
                var3 = Block_blocksList[var4]->slipperiness * 0.91F;
            }
        }

        float var8 = 0.16277136F / (var3 * var3 * var3);
        Entity_moveFlying(self, var1, var2, self->onGround ? 0.1F * var8 : 0.02F);
        var3 = 0.91F;
        if (self->onGround) {
            var3 = 546.0F * 0.1F * 0.1F * 0.1F;
            int var5 = self->worldObj->vtable->getBlockId(
                self->worldObj, MathHelper_floor_double(self->posX),
                MathHelper_floor_double(self->boundingBox.minY) - 1,
                MathHelper_floor_double(self->posZ));
            if (var5 > 0 && Block_blocksList[var5]) {
                var3 = Block_blocksList[var5]->slipperiness * 0.91F;
            }
        }

        self->vtable->moveEntity(self, self->motionX, self->motionY, self->motionZ);
        self->motionX *= (double)var3;
        self->motionY *= (double)var3;
        self->motionZ *= (double)var3;
    }

    selfL->field_705_Q = selfL->field_704_R;
    double var10 = self->posX - self->prevPosX;
    double var9 = self->posZ - self->prevPosZ;
    float var7 = MathHelper_sqrt_double(var10 * var10 + var9 * var9) * 4.0F;
    if (var7 > 1.0F) {
        var7 = 1.0F;
    }

    selfL->field_704_R += (var7 - selfL->field_704_R) * 0.4F;
    selfL->field_703_S += selfL->field_704_R;
}

int EntityFlying_isOnLadder(EntityLiving *self) {
    (void)self;
    return 0;
}
