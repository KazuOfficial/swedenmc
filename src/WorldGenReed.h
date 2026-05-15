#ifndef WORLD_GEN_REED_H
#define WORLD_GEN_REED_H

#include "WorldGenerator.h"

typedef struct WorldGenReed {
    WorldGenerator base;
} WorldGenReed;

WorldGenReed *WorldGenReed_create(void);
void WorldGenReed_free(WorldGenReed *self);

#endif
