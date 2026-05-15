#include "JavaMath.h"
#include "EntityTNTPrimed.h"
#include "World.h"
#include "MathHelper.h"
#include "NBTTagCompound.h"
#include <math.h>
#include <stdlib.h>

static EntityVtable EntityTNTPrimed_vtable;
static int EntityTNTPrimed_vtable_init = 0;

static const char *tntprimed_getEntityString(Entity *self) {
    (void)self;
    return "PrimedTnt";
}

static void ensure_vtable(void) {
    if (EntityTNTPrimed_vtable_init)
        return;
    Entity_initVtable(&EntityTNTPrimed_vtable);
    EntityTNTPrimed_vtable.getEntityString = tntprimed_getEntityString;
    EntityTNTPrimed_vtable.entityInit = EntityTNTPrimed_entityInit;
    EntityTNTPrimed_vtable.canTriggerWalking = EntityTNTPrimed_canTriggerWalking;
    EntityTNTPrimed_vtable.canBeCollidedWith = EntityTNTPrimed_canBeCollidedWith;
    EntityTNTPrimed_vtable.onUpdate = EntityTNTPrimed_onUpdate;
    EntityTNTPrimed_vtable.writeEntityToNBT = EntityTNTPrimed_writeEntityToNBT;
    EntityTNTPrimed_vtable.readEntityFromNBT = EntityTNTPrimed_readEntityFromNBT;
    EntityTNTPrimed_vtable.getShadowSize = EntityTNTPrimed_getShadowSize;
    EntityTNTPrimed_vtable_init = 1;
}

void EntityTNTPrimed_construct(EntityTNTPrimed *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityTNTPrimed_vtable;

    self->fuse = 0;
    self->base.preventEntitySpawning = 1;
    Entity_setSize(&self->base, 0.98F, 0.98F);
    self->base.yOffset = self->base.height / 2.0F;
    self->base.isTNTPrimed = 1;
}

void EntityTNTPrimed_construct2(EntityTNTPrimed *self, World *var1, double var2, double var4,
                                double var6) {
    EntityTNTPrimed_construct(self, var1);
    self->base.vtable->setPosition(&self->base, var2, var4, var6);

    float var8 = (float)((Math_random()) * (double)((float)M_PI) * 2.0);
    self->base.motionX = (double)(-MathHelper_sin(var8 * (float)M_PI / 180.0F) * 0.02F);
    self->base.motionY = (double)0.2F;
    self->base.motionZ = (double)(-MathHelper_cos(var8 * (float)M_PI / 180.0F) * 0.02F);
    self->fuse = 80;
    self->base.prevPosX = var2;
    self->base.prevPosY = var4;
    self->base.prevPosZ = var6;
}

EntityTNTPrimed *EntityTNTPrimed_create(World *var1) {
    EntityTNTPrimed *self = (EntityTNTPrimed *)calloc(1, sizeof(EntityTNTPrimed));
    EntityTNTPrimed_construct(self, var1);
    return self;
}

EntityTNTPrimed *EntityTNTPrimed_create2(World *var1, double var2, double var4, double var6) {
    EntityTNTPrimed *self = (EntityTNTPrimed *)calloc(1, sizeof(EntityTNTPrimed));
    EntityTNTPrimed_construct2(self, var1, var2, var4, var6);
    return self;
}

void EntityTNTPrimed_entityInit(Entity *self) { (void)self; }

int EntityTNTPrimed_canTriggerWalking(Entity *self) {
    (void)self;
    return 0;
}

int EntityTNTPrimed_canBeCollidedWith(Entity *self) { return !self->isDead; }

static void explode(EntityTNTPrimed *self) {
    float var1 = 4.0F;
    World_createExplosion(self->base.worldObj, NULL, self->base.posX, self->base.posY,
                          self->base.posZ, var1);
}

void EntityTNTPrimed_onUpdate(Entity *self) {
    EntityTNTPrimed *tnt = (EntityTNTPrimed *)self;

    self->prevPosX = self->posX;
    self->prevPosY = self->posY;
    self->prevPosZ = self->posZ;
    self->motionY -= (double)0.04F;
    self->vtable->moveEntity(self, self->motionX, self->motionY, self->motionZ);
    self->motionX *= (double)0.98F;
    self->motionY *= (double)0.98F;
    self->motionZ *= (double)0.98F;
    if (self->onGround) {
        self->motionX *= (double)0.7F;
        self->motionZ *= (double)0.7F;
        self->motionY *= -0.5;
    }

    if (tnt->fuse-- <= 0) {
        if (!self->worldObj->multiplayerWorld) {
            self->vtable->setEntityDead(self);
            explode(tnt);
        } else {
            self->vtable->setEntityDead(self);
        }
    } else {
        World_spawnParticle(self->worldObj, "smoke", self->posX, self->posY + 0.5, self->posZ, 0.0,
                            0.0, 0.0);
    }
}

void EntityTNTPrimed_writeEntityToNBT(Entity *self, NBTTagCompound *var1) {
    EntityTNTPrimed *tnt = (EntityTNTPrimed *)self;
    NBTTagCompound_setByte(var1, "Fuse", (int8_t)tnt->fuse);
}

void EntityTNTPrimed_readEntityFromNBT(Entity *self, NBTTagCompound *var1) {
    EntityTNTPrimed *tnt = (EntityTNTPrimed *)self;
    tnt->fuse = NBTTagCompound_getByte(var1, "Fuse");
}

float EntityTNTPrimed_getShadowSize(Entity *self) {
    (void)self;
    return 0.0F;
}
