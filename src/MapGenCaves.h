#ifndef MAP_GEN_CAVES_H
#define MAP_GEN_CAVES_H

#include "MapGenBase.h"

typedef struct MapGenCaves {
    MapGenBase base;
} MapGenCaves;

MapGenCaves *MapGenCaves_create(void);
void MapGenCaves_free(MapGenCaves *self);

#endif
