#ifndef BLOCK_DISPENSER_H
#define BLOCK_DISPENSER_H

#include "Block.h"
#include "JavaRandom.h"

typedef struct BlockDispenser {
    Block base;
    JavaRandom random;
} BlockDispenser;

void BlockDispenser_construct(BlockDispenser *self, int id);
BlockDispenser *BlockDispenser_create(int id);

void BlockDispenser_initVtable(BlockVtable *vt);
extern BlockVtable BlockDispenser_defaultVtable;

int BlockDispenser_tickRate(Block *self);
int BlockDispenser_idDropped(Block *self, int meta, JavaRandom *rand);
void BlockDispenser_onBlockAdded(Block *self, World *world, int x, int y, int z);
int BlockDispenser_getBlockTexture(Block *self, IBlockAccess *world, int x, int y, int z, int side);
int BlockDispenser_getBlockTextureFromSide(Block *self, int side);
int BlockDispenser_blockActivated(Block *self, World *world, int x, int y, int z,
                                  struct EntityPlayer *player);
void BlockDispenser_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                          int neighborID);
void BlockDispenser_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
void BlockDispenser_onBlockPlacedBy(Block *self, World *world, int x, int y, int z,
                                    struct EntityLiving *entity);
void BlockDispenser_onBlockRemoval(Block *self, World *world, int x, int y, int z);

#endif
