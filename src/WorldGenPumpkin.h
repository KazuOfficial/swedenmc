#ifndef WORLD_GEN_PUMPKIN_H
#define WORLD_GEN_PUMPKIN_H

#include "WorldGenerator.h"

typedef struct WorldGenPumpkin {
    WorldGenerator base;
} WorldGenPumpkin;

WorldGenPumpkin *WorldGenPumpkin_create(void);
void WorldGenPumpkin_free(WorldGenPumpkin *self);

#endif
