#include "EntityZombie.h"
#include "EntityMob.h"
#include "EntityLiving.h"
#include "World.h"
#include "Item.h"
#include "MathHelper.h"
#include <stdlib.h>

EntityMobVtable EntityZombie_defaultVtable;

static const char *zombie_getEntityString(Entity *self) {
    (void)self;
    return "Zombie";
}

void EntityZombie_initVtable(EntityMobVtable *vt) {
    EntityMob_initVtable(vt);
    vt->base.base.base.getEntityString = zombie_getEntityString;

    vt->base.base.onLivingUpdate = EntityZombie_onLivingUpdate;
    vt->base.base.getLivingSound = EntityZombie_getLivingSound;
    vt->base.base.getHurtSound = EntityZombie_getHurtSound;
    vt->base.base.getDeathSound = EntityZombie_getDeathSound;
    vt->base.base.getDropItemId = EntityZombie_getDropItemId;
}

void EntityZombie_construct(EntityZombie *self, World *var1) {
    static int vtable_init = 0;
    if (!vtable_init) {
        EntityZombie_initVtable(&EntityZombie_defaultVtable);
        vtable_init = 1;
    }
    EntityMob_construct(&self->base, var1);
    self->base.base.base.base.vtable = (const EntityVtable *)&EntityZombie_defaultVtable;

    self->base.base.base.texture = "/mob/zombie.png";
    self->base.base.base.moveSpeed = 0.5F;
    self->base.attackStrength = 5;
    self->base.base.base.base.isZombie = 1;
}

EntityZombie *EntityZombie_create(World *var1) {
    EntityZombie *self = (EntityZombie *)calloc(1, sizeof(EntityZombie));
    EntityZombie_construct(self, var1);
    return self;
}

void EntityZombie_onLivingUpdate(EntityLiving *self) {
    Entity *e = &self->base;

    if (World_isDaytime(e->worldObj)) {
        float var1 = e->vtable->getEntityBrightness(e, 1.0F);
        if (var1 > 0.5F &&
            World_canBlockSeeTheSky(e->worldObj, MathHelper_floor_double(e->posX),
                                    MathHelper_floor_double(e->posY),
                                    MathHelper_floor_double(e->posZ)) &&
            JavaRandom_nextFloat(&e->rand) * 30.0F < (var1 - 0.4F) * 2.0F) {
            e->fire = 300;
        }
    }

    EntityMob_onLivingUpdate(self);
}

const char *EntityZombie_getLivingSound(EntityLiving *self) {
    (void)self;
    return "mob.zombie";
}

const char *EntityZombie_getHurtSound(EntityLiving *self) {
    (void)self;
    return "mob.zombiehurt";
}

const char *EntityZombie_getDeathSound(EntityLiving *self) {
    (void)self;
    return "mob.zombiedeath";
}

int EntityZombie_getDropItemId(EntityLiving *self) {
    (void)self;
    return Item_feather->shiftedIndex;
}
