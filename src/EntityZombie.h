#ifndef ENTITY_ZOMBIE_H
#define ENTITY_ZOMBIE_H

#include "EntityMob.h"

typedef struct EntityZombie {
    EntityMob base;
} EntityZombie;

void EntityZombie_construct(EntityZombie *self, World *var1);
void EntityZombie_initVtable(EntityMobVtable *vt);

extern EntityMobVtable EntityZombie_defaultVtable;

EntityZombie *EntityZombie_create(World *var1);

void EntityZombie_onLivingUpdate(EntityLiving *self);
const char *EntityZombie_getLivingSound(EntityLiving *self);
const char *EntityZombie_getHurtSound(EntityLiving *self);
const char *EntityZombie_getDeathSound(EntityLiving *self);
int EntityZombie_getDropItemId(EntityLiving *self);

#endif
