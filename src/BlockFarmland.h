#ifndef BLOCK_FARMLAND_H
#define BLOCK_FARMLAND_H

#include "Block.h"

typedef struct BlockFarmland {
    Block base;
} BlockFarmland;

void BlockFarmland_construct(BlockFarmland *self, int id);
BlockFarmland *BlockFarmland_create(int id);

void BlockFarmland_initVtable(BlockVtable *vt);
extern BlockVtable BlockFarmland_defaultVtable;

AxisAlignedBB *BlockFarmland_getCollisionBoundingBoxFromPool(Block *self, World *world, int x,
                                                             int y, int z);
int BlockFarmland_isOpaqueCube(Block *self);
int BlockFarmland_renderAsNormalBlock(Block *self);
int BlockFarmland_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta);
void BlockFarmland_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
void BlockFarmland_onEntityWalking(Block *self, World *world, int x, int y, int z,
                                   struct Entity *entity);
void BlockFarmland_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                         int neighborID);
int BlockFarmland_idDropped(Block *self, int meta, JavaRandom *rand);

#endif
