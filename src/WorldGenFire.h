#ifndef WORLD_GEN_FIRE_H
#define WORLD_GEN_FIRE_H

#include "WorldGenerator.h"

typedef struct WorldGenFire {
    WorldGenerator base;
} WorldGenFire;

WorldGenFire *WorldGenFire_create(void);

#endif
