#ifndef BLOCK_CHEST_H
#define BLOCK_CHEST_H

#include "Block.h"
#include "JavaRandom.h"

typedef struct BlockChest {
    Block base;
    JavaRandom random;
} BlockChest;

void BlockChest_construct(BlockChest *self, int id);
BlockChest *BlockChest_create(int id);

void BlockChest_initVtable(BlockVtable *vt);
extern BlockVtable BlockChest_defaultVtable;

int BlockChest_getBlockTexture(Block *self, IBlockAccess *world, int x, int y, int z, int side);
int BlockChest_getBlockTextureFromSide(Block *self, int side);
int BlockChest_canPlaceBlockAt(Block *self, World *world, int x, int y, int z);
void BlockChest_onBlockRemoval(Block *self, World *world, int x, int y, int z);
int BlockChest_blockActivated(Block *self, World *world, int x, int y, int z, EntityPlayer *player);

struct TileEntity *BlockChest_getBlockEntity(Block *self);

#endif
