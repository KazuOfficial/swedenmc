#ifndef ENTITY_PIG_H
#define ENTITY_PIG_H

#include "EntityAnimal.h"

typedef struct EntityPig {
    EntityAnimal base;

} EntityPig;

EntityPig *EntityPig_create(World *world);

int EntityPig_getSaddled(EntityPig *self);

void EntityPig_setSaddled(EntityPig *self, int var1);

#endif
