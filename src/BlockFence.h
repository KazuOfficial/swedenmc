#ifndef BLOCK_FENCE_H
#define BLOCK_FENCE_H

#include "Block.h"

typedef struct BlockFence {
    Block base;
} BlockFence;

void BlockFence_construct(BlockFence *self, int id, int tex);
BlockFence *BlockFence_create(int id, int tex);

void BlockFence_initVtable(BlockVtable *vt);
extern BlockVtable BlockFence_defaultVtable;

int BlockFence_canPlaceBlockAt(Block *self, World *world, int x, int y, int z);
AxisAlignedBB *BlockFence_getCollisionBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                          int z);
int BlockFence_isOpaqueCube(Block *self);
int BlockFence_renderAsNormalBlock(Block *self);
int BlockFence_getRenderType(Block *self);

#endif
