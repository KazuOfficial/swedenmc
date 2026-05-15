#ifndef BLOCK_LEAVES_BASE_H
#define BLOCK_LEAVES_BASE_H

#include "Block.h"
#include "Material.h"

typedef struct BlockLeavesBase {
    Block base;
    int graphicsLevel;
} BlockLeavesBase;

void BlockLeavesBase_construct(BlockLeavesBase *self, int id, int tex, Material *mat,
                               int graphicsLevel);

int BlockLeavesBase_isOpaqueCube(Block *self);
int BlockLeavesBase_shouldSideBeRendered(Block *self, IBlockAccess *world, int x, int y, int z,
                                         int side);

#endif
