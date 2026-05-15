#ifndef ENTITY_ANIMAL_H
#define ENTITY_ANIMAL_H

#include "EntityCreature.h"

struct EntityAnimal;
typedef struct EntityAnimal EntityAnimal;

typedef struct EntityAnimalVtable {
    EntityCreatureVtable base;
} EntityAnimalVtable;

struct EntityAnimal {
    EntityCreature base;
};

void EntityAnimal_construct(EntityAnimal *self, World *world);
void EntityAnimal_initVtable(EntityAnimalVtable *vt);

#endif
