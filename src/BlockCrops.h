#ifndef BLOCK_CROPS_H
#define BLOCK_CROPS_H

#include "Block.h"
#include "BlockFlower.h"

typedef struct BlockCrops {
    Block base;
} BlockCrops;

void BlockCrops_construct(BlockCrops *self, int id, int tex);
BlockCrops *BlockCrops_create(int id, int tex);

void BlockCrops_initVtable(BlockVtable *vt);
extern BlockVtable BlockCrops_defaultVtable;

int BlockCrops_canThisPlantGrowOnThisBlockID(Block *self, int blockId);
void BlockCrops_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);
int BlockCrops_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta);
int BlockCrops_getRenderType(Block *self);
void BlockCrops_dropBlockAsItemWithChance(Block *self, World *world, int x, int y, int z, int meta,
                                          float chance);
int BlockCrops_idDropped(Block *self, int meta, JavaRandom *rand);
int BlockCrops_quantityDropped(Block *self, JavaRandom *rand);

void BlockCrops_fertilize(BlockCrops *self, World *world, int x, int y, int z);

#endif
