#ifndef ENTITY_COW_H
#define ENTITY_COW_H

#include "EntityAnimal.h"

typedef struct EntityCow {
    EntityAnimal base;
} EntityCow;

EntityCow *EntityCow_create(World *world);

#endif
