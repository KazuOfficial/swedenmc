#ifndef WORLD_GEN_CLAY_H
#define WORLD_GEN_CLAY_H

#include "WorldGenerator.h"

typedef struct WorldGenClay {
    WorldGenerator base;
    int field_35_a;
} WorldGenClay;

WorldGenClay *WorldGenClay_create(int numberOfBlocks);
void WorldGenClay_free(WorldGenClay *self);

#endif
