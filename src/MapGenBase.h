#ifndef MAP_GEN_BASE_H
#define MAP_GEN_BASE_H

#include "World.h"
#include "JavaRandom.h"
#include <stdint.h>

typedef struct MapGenBase MapGenBase;

typedef struct MapGenBaseVtable {

    void (*func_868_a)(MapGenBase *self, World *world, int srcCX, int srcCZ, int tgtCX, int tgtCZ,
                       uint8_t *chunkBlocks);
} MapGenBaseVtable;

struct MapGenBase {
    const MapGenBaseVtable *vtable;
    int range;
    JavaRandom rand;
};

void MapGenBase_generate(MapGenBase *self, World *world, int chunkX, int chunkZ,
                         uint8_t *chunkBlocks);

void MapGenBase_func_868_a_default(MapGenBase *self, World *world, int srcCX, int srcCZ, int tgtCX,
                                   int tgtCZ, uint8_t *chunkBlocks);

#endif
