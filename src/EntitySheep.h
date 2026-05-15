#ifndef ENTITY_SHEEP_H
#define ENTITY_SHEEP_H

#include "EntityAnimal.h"
#include "JavaRandom.h"

typedef struct EntitySheep {
    EntityAnimal base;

} EntitySheep;

EntitySheep *EntitySheep_create(World *world);

extern float EntitySheep_fleeceColorTable[16][3];

int EntitySheep_getFleeceColor(EntitySheep *self);

void EntitySheep_setFleeceColor(EntitySheep *self, int var1);

int EntitySheep_getSheared(EntitySheep *self);

void EntitySheep_setSheared(EntitySheep *self, int var1);

int EntitySheep_getRandomFleeceColor(JavaRandom *var0);

#endif
