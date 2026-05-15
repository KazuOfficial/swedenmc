#ifndef WORLD_GEN_LAKES_H
#define WORLD_GEN_LAKES_H

#include "WorldGenerator.h"

typedef struct WorldGenLakes {
    WorldGenerator base;
    int field_40_a;
} WorldGenLakes;

WorldGenLakes *WorldGenLakes_create(int blockId);
void WorldGenLakes_free(WorldGenLakes *self);

#endif
