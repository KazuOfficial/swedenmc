#ifndef BLOCK_CAKE_H
#define BLOCK_CAKE_H

#include "Block.h"

typedef struct BlockCake {
    Block base;
} BlockCake;

void BlockCake_construct(BlockCake *self, int id, int tex);
BlockCake *BlockCake_create(int id, int tex);

void BlockCake_initVtable(BlockVtable *vt);
extern BlockVtable BlockCake_defaultVtable;

void BlockCake_setBlockBoundsBasedOnState(Block *self, IBlockAccess *world, int x, int y, int z);
void BlockCake_setBlockBoundsForItemRender(Block *self);
AxisAlignedBB *BlockCake_getCollisionBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                         int z);
AxisAlignedBB *BlockCake_getSelectedBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                        int z);
int BlockCake_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta);
int BlockCake_getBlockTextureFromSide(Block *self, int side);
int BlockCake_renderAsNormalBlock(Block *self);
int BlockCake_isOpaqueCube(Block *self);
int BlockCake_blockActivated(Block *self, World *world, int x, int y, int z, EntityPlayer *player);
void BlockCake_onBlockClicked(Block *self, World *world, int x, int y, int z, EntityPlayer *player);
int BlockCake_canPlaceBlockAt(Block *self, World *world, int x, int y, int z);
void BlockCake_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                     int neighborID);
int BlockCake_canBlockStay(Block *self, World *world, int x, int y, int z);
int BlockCake_quantityDropped(Block *self, JavaRandom *rand);
int BlockCake_idDropped(Block *self, int meta, JavaRandom *rand);

#endif
