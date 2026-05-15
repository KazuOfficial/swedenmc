#include "EntityAnimal.h"
#include "World.h"
#include "Block.h"
#include "MathHelper.h"
#include "NBTTagCompound.h"
#include <stdlib.h>

static float def_getBlockPathWeight(EntityCreature *base, int var1, int var2, int var3) {
    Entity *e = &base->base.base;
    if (World_getBlockId(e->worldObj, var1, var2 - 1, var3) == Block_grass->blockID) {
        return 10.0f;
    }
    return (float)e->worldObj->vtable->getLightBrightness(e->worldObj, var1, var2, var3) - 0.5f;
}

static void def_writeEntityToNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.writeEntityToNBT(e, var1);
}

static void def_readEntityFromNBT(Entity *e, NBTTagCompound *var1) {
    extern EntityVtable Entity_defaultVtable;
    EntityLiving_defaultVtable.base.readEntityFromNBT(e, var1);
}

static int def_getCanSpawnHere(EntityLiving *base) {
    Entity *e = &base->base;
    int var1 = MathHelper_floor_double(e->posX);
    int var2 = MathHelper_floor_double(e->boundingBox.minY);
    int var3 = MathHelper_floor_double(e->posZ);
    return World_getBlockId(e->worldObj, var1, var2 - 1, var3) == Block_grass->blockID &&
           e->worldObj->vtable->getFullBlockLightValue(e->worldObj, var1, var2, var3) > 8 &&
           EntityCreature_getCanSpawnHere((EntityCreature *)base);
}

static int def_getTalkInterval(EntityLiving *self) {
    (void)self;
    return 120;
}

void EntityAnimal_initVtable(EntityAnimalVtable *vt) {
    EntityCreature_initVtable(&vt->base);
    vt->base.base.base.writeEntityToNBT = def_writeEntityToNBT;
    vt->base.base.base.readEntityFromNBT = def_readEntityFromNBT;
    vt->base.base.getCanSpawnHere = def_getCanSpawnHere;
    vt->base.base.getTalkInterval = def_getTalkInterval;
    vt->base.getBlockPathWeight = def_getBlockPathWeight;
}

void EntityAnimal_construct(EntityAnimal *self, World *world) {
    EntityCreature_construct(&self->base, world);
    self->base.base.base.isAnimal = 1;
}
