#ifndef MAP_GEN_CAVES_HELL_H
#define MAP_GEN_CAVES_HELL_H

#include "MapGenCaves.h"

typedef struct MapGenCavesHell {
    MapGenBase base;
} MapGenCavesHell;

void MapGenCavesHell_init(MapGenCavesHell *self);
MapGenCavesHell *MapGenCavesHell_create(void);
void MapGenCavesHell_free(MapGenCavesHell *self);

#endif
