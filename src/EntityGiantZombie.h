#ifndef ENTITY_GIANT_ZOMBIE_H
#define ENTITY_GIANT_ZOMBIE_H

#include "EntityMob.h"

typedef struct EntityGiantZombie {
    EntityMob base;
} EntityGiantZombie;

void EntityGiantZombie_construct(EntityGiantZombie *self, World *var1);
EntityGiantZombie *EntityGiantZombie_create(World *var1);

float EntityGiantZombie_getBlockPathWeight(EntityCreature *self, int var1, int var2, int var3);

#endif
