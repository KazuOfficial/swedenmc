#ifndef WORLD_GEN_MINABLE_H
#define WORLD_GEN_MINABLE_H

#include "WorldGenerator.h"

typedef struct WorldGenMinable {
    WorldGenerator base;
    int minableBlockId;
    int numberOfBlocks;
} WorldGenMinable;

WorldGenMinable *WorldGenMinable_create(int blockID, int count);
void WorldGenMinable_free(WorldGenMinable *self);

#endif
