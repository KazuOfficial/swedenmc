#ifndef BLOCK_RAIL_H
#define BLOCK_RAIL_H

#include "Block.h"
#include "World.h"

typedef struct BlockRail {
    Block base;
    int isPowered;
} BlockRail;

void BlockRail_initVtable(BlockVtable *vt);
extern BlockVtable BlockRail_defaultVtable;

void BlockRail_construct(BlockRail *self, int id, int tex, int booster);

int BlockRail_isRailBlockAt(World *world, int x, int y, int z);

int BlockRail_isRailBlock(int blockId);

int BlockRail_getIsPowered(Block *self);

Block *BlockRail_create(int id, int tex, int booster);

#endif
