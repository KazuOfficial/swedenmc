#ifndef ENTITY_SORTER_H
#define ENTITY_SORTER_H

#include "Entity.h"

typedef struct WorldRenderer WorldRenderer;

typedef struct EntitySorter {

    double field_30008_a;

    double field_30007_b;

    double field_30009_c;
} EntitySorter;

void EntitySorter_construct(EntitySorter *self, Entity *var1);

int EntitySorter_sortByDistanceToEntity(EntitySorter *self, WorldRenderer *var1,
                                        WorldRenderer *var2);

int EntitySorter_compare(EntitySorter *self, void *var1, void *var2);

#endif
