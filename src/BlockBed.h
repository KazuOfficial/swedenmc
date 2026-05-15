#ifndef BLOCK_BED_H
#define BLOCK_BED_H

#include "Block.h"
#include "ChunkCoordinates.h"

typedef struct BlockBed {
    Block base;
} BlockBed;

void BlockBed_construct(BlockBed *self, int id);
BlockBed *BlockBed_create(int id);

void BlockBed_initVtable(BlockVtable *vt);
extern BlockVtable BlockBed_defaultVtable;

int BlockBed_blockActivated(Block *self, World *world, int x, int y, int z, EntityPlayer *player);
int BlockBed_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta);
int BlockBed_getRenderType(Block *self);
int BlockBed_renderAsNormalBlock(Block *self);
int BlockBed_isOpaqueCube(Block *self);
void BlockBed_setBlockBoundsBasedOnState(Block *self, IBlockAccess *world, int x, int y, int z);
void BlockBed_onNeighborBlockChange(Block *self, World *world, int x, int y, int z, int neighborID);
int BlockBed_idDropped(Block *self, int meta, JavaRandom *rand);
void BlockBed_dropBlockAsItemWithChance(Block *self, World *world, int x, int y, int z, int meta,
                                        float chance);
int BlockBed_getMobilityFlag(Block *self);

int BlockBed_getDirectionFromMetadata(int meta);
int BlockBed_isBlockFootOfBed(int meta);
int BlockBed_isBedOccupied(int meta);
void BlockBed_setBedOccupied(World *world, int x, int y, int z, int occupied);

ChunkCoordinates *BlockBed_getNearestEmptyChunkCoordinates(World *world, int x, int y, int z,
                                                           int skip);

extern const int BlockBed_headBlockToFootBlockMap[4][2];

#endif
