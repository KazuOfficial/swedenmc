#ifndef WORLD_GEN_DEAD_BUSH_H
#define WORLD_GEN_DEAD_BUSH_H

#include "WorldGenerator.h"

typedef struct WorldGenDeadBush {
    WorldGenerator base;
    int field_28058_a;
} WorldGenDeadBush;

WorldGenDeadBush *WorldGenDeadBush_create(int blockId);
void WorldGenDeadBush_free(WorldGenDeadBush *self);

#endif
