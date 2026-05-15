#ifndef WORLD_GEN_HELL_LAVA_H
#define WORLD_GEN_HELL_LAVA_H

#include "WorldGenerator.h"

typedef struct WorldGenHellLava {
    WorldGenerator base;
    int field_4158_a;
} WorldGenHellLava;

WorldGenHellLava *WorldGenHellLava_create(int blockId);

#endif
