#ifndef BLOCK_BREAKABLE_H
#define BLOCK_BREAKABLE_H

#include "Block.h"
#include "Material.h"

typedef struct BlockBreakable {
    Block base;
    int localFlag;
} BlockBreakable;

void BlockBreakable_construct(BlockBreakable *self, int id, int tex, Material *mat, int localFlag);

int BlockBreakable_isOpaqueCube(Block *self);
int BlockBreakable_shouldSideBeRendered(Block *self, IBlockAccess *world, int x, int y, int z,
                                        int side);

#endif
