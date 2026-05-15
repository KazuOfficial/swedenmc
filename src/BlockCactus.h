#ifndef BLOCK_CACTUS_H
#define BLOCK_CACTUS_H

#include "Block.h"

typedef struct BlockCactus {
    Block base;
} BlockCactus;

void BlockCactus_construct(BlockCactus *self, int id, int tex);
BlockCactus *BlockCactus_create(int id, int tex);

void BlockCactus_initVtable(BlockVtable *vt);
extern BlockVtable BlockCactus_defaultVtable;

void BlockCactus_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
AxisAlignedBB *BlockCactus_getCollisionBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                           int z);
AxisAlignedBB *BlockCactus_getSelectedBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                          int z);
int BlockCactus_getBlockTextureFromSide(Block *self, int side);
int BlockCactus_renderAsNormalBlock(Block *self);
int BlockCactus_isOpaqueCube(Block *self);
int BlockCactus_getRenderType(Block *self);
int BlockCactus_canPlaceBlockAt(Block *self, World *world, int x, int y, int z);
void BlockCactus_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                       int neighborID);
int BlockCactus_canBlockStay(Block *self, World *world, int x, int y, int z);
void BlockCactus_onEntityCollidedWithBlock(Block *self, World *world, int x, int y, int z,
                                           struct Entity *entity);

#endif
