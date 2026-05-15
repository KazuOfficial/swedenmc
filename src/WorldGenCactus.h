#ifndef WORLD_GEN_CACTUS_H
#define WORLD_GEN_CACTUS_H

#include "WorldGenerator.h"

typedef struct WorldGenCactus {
    WorldGenerator base;
} WorldGenCactus;

WorldGenCactus *WorldGenCactus_create(void);
void WorldGenCactus_free(WorldGenCactus *self);

#endif
