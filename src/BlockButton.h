#ifndef BLOCK_BUTTON_H
#define BLOCK_BUTTON_H

#include "Block.h"

typedef struct BlockButton {
    Block base;
} BlockButton;

void BlockButton_construct(BlockButton *self, int id, int tex);
BlockButton *BlockButton_create(int id, int tex);

void BlockButton_initVtable(BlockVtable *vt);
extern BlockVtable BlockButton_defaultVtable;

AxisAlignedBB *BlockButton_getCollisionBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                           int z);
int BlockButton_tickRate(Block *self);
int BlockButton_isOpaqueCube(Block *self);
int BlockButton_renderAsNormalBlock(Block *self);
int BlockButton_canPlaceBlockOnSide(Block *self, World *world, int x, int y, int z, int side);
int BlockButton_canPlaceBlockAt(Block *self, World *world, int x, int y, int z);
void BlockButton_onBlockPlaced(Block *self, World *world, int x, int y, int z, int side);
void BlockButton_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                       int neighborID);
void BlockButton_setBlockBoundsBasedOnState(Block *self, IBlockAccess *world, int x, int y, int z);
void BlockButton_onBlockClicked(Block *self, World *world, int x, int y, int z,
                                EntityPlayer *player);
int BlockButton_blockActivated(Block *self, World *world, int x, int y, int z,
                               EntityPlayer *player);
void BlockButton_onBlockRemoval(Block *self, World *world, int x, int y, int z);
int BlockButton_isPoweringTo(Block *self, IBlockAccess *world, int x, int y, int z, int side);
int BlockButton_isIndirectlyPoweringTo(Block *self, World *world, int x, int y, int z, int side);
int BlockButton_canProvidePower(Block *self);
void BlockButton_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
void BlockButton_setBlockBoundsForItemRender(Block *self);

#endif
