#include "EntityWaterMob.h"
#include "World.h"

static EntityCreatureVtable EntityWaterMob_vtable;
static int EntityWaterMob_vtable_init = 0;

void EntityWaterMob_initVtable(EntityCreatureVtable *vt) {
    EntityCreature_initVtable(vt);
    vt->base.canBreatheUnderwater = EntityWaterMob_canBreatheUnderwater;
    vt->base.base.writeEntityToNBT = EntityWaterMob_writeEntityToNBT;
    vt->base.base.readEntityFromNBT = EntityWaterMob_readEntityFromNBT;
    vt->base.getCanSpawnHere = EntityWaterMob_getCanSpawnHere;
    vt->base.getTalkInterval = EntityWaterMob_getTalkInterval;
}

static void ensure_vtable(void) {
    if (EntityWaterMob_vtable_init)
        return;
    EntityWaterMob_initVtable(&EntityWaterMob_vtable);
    EntityWaterMob_vtable_init = 1;
}

void EntityWaterMob_construct(EntityWaterMob *self, World *var1) {
    ensure_vtable();
    EntityCreature_construct(&self->base, var1);
    self->base.base.base.vtable = (const EntityVtable *)&EntityWaterMob_vtable;
    self->base.base.base.isWaterMob = 1;
}

int EntityWaterMob_canBreatheUnderwater(EntityLiving *self) {
    (void)self;
    return 1;
}

void EntityWaterMob_writeEntityToNBT(Entity *self, NBTTagCompound *var1) {

    EntityLiving_defaultVtable.base.writeEntityToNBT(self, var1);
}

void EntityWaterMob_readEntityFromNBT(Entity *self, NBTTagCompound *var1) {

    EntityLiving_defaultVtable.base.readEntityFromNBT(self, var1);
}

int EntityWaterMob_getCanSpawnHere(EntityLiving *self) {
    Entity *selfE = &self->base;
    return World_checkIfAABBIsClear(selfE->worldObj, &selfE->boundingBox);
}

int EntityWaterMob_getTalkInterval(EntityLiving *self) {
    (void)self;
    return 120;
}
