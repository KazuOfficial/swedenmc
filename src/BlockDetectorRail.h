#ifndef BLOCK_DETECTOR_RAIL_H
#define BLOCK_DETECTOR_RAIL_H

#include "BlockRail.h"

typedef struct BlockDetectorRail {
    BlockRail base;
} BlockDetectorRail;

void BlockDetectorRail_construct(BlockDetectorRail *self, int id, int tex);
BlockDetectorRail *BlockDetectorRail_create(int id, int tex);

void BlockDetectorRail_initVtable(BlockVtable *vt);
extern BlockVtable BlockDetectorRail_defaultVtable;

int BlockDetectorRail_tickRate(Block *self);
int BlockDetectorRail_canProvidePower(Block *self);
void BlockDetectorRail_onEntityCollidedWithBlock(Block *self, World *world, int x, int y, int z,
                                                 struct Entity *entity);
void BlockDetectorRail_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
int BlockDetectorRail_isPoweringTo(Block *self, IBlockAccess *world, int x, int y, int z, int side);
int BlockDetectorRail_isIndirectlyPoweringTo(Block *self, World *world, int x, int y, int z,
                                             int side);

#endif
