#ifndef BLOCK_FIRE_H
#define BLOCK_FIRE_H

#include "Block.h"
#include "IBlockAccess.h"

#define BLOCK_FIRE_TABLE_SIZE 256

typedef struct BlockFire {
    Block base;
    int chanceToEncourageFire[BLOCK_FIRE_TABLE_SIZE];
    int abilityToCatchFire[BLOCK_FIRE_TABLE_SIZE];
} BlockFire;

void BlockFire_construct(BlockFire *self, int id, int tex);
BlockFire *BlockFire_create(int id, int tex);

void BlockFire_initVtable(BlockVtable *vt);
extern BlockVtable BlockFire_defaultVtable;

void BlockFire_initializeBlock(Block *self);
AxisAlignedBB *BlockFire_getCollisionBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                         int z);
int BlockFire_isOpaqueCube(Block *self);
int BlockFire_renderAsNormalBlock(Block *self);
int BlockFire_getRenderType(Block *self);
int BlockFire_quantityDropped(Block *self, JavaRandom *rand);
int BlockFire_tickRate(Block *self);
void BlockFire_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
int BlockFire_isCollidable(Block *self);
int BlockFire_canPlaceBlockAt(Block *self, World *world, int x, int y, int z);
void BlockFire_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                     int neighborID);
void BlockFire_onBlockAdded(Block *self, World *world, int x, int y, int z);
void BlockFire_randomDisplayTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);

int BlockFire_canBlockCatchFire(Block *self, IBlockAccess *world, int x, int y, int z);
int BlockFire_getChanceToEncourageFire(Block *self, World *world, int x, int y, int z, int current);

#endif
