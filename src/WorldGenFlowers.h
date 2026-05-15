#ifndef WORLD_GEN_FLOWERS_H
#define WORLD_GEN_FLOWERS_H

#include "WorldGenerator.h"

typedef struct WorldGenFlowers {
    WorldGenerator base;
    int field_39_a;
} WorldGenFlowers;

WorldGenFlowers *WorldGenFlowers_create(int blockId);
void WorldGenFlowers_free(WorldGenFlowers *self);

#endif
