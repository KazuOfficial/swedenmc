#ifndef BLOCK_DOOR_H
#define BLOCK_DOOR_H

#include "Block.h"
#include "IBlockAccess.h"

typedef struct BlockDoor {
    Block base;
} BlockDoor;

void BlockDoor_construct(BlockDoor *self, int id, Material *mat);
BlockDoor *BlockDoor_create(int id, Material *mat);

void BlockDoor_initVtable(BlockVtable *vt);
extern BlockVtable BlockDoor_defaultVtable;

int BlockDoor_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta);
int BlockDoor_isOpaqueCube(Block *self);
int BlockDoor_renderAsNormalBlock(Block *self);
int BlockDoor_getRenderType(Block *self);
AxisAlignedBB *BlockDoor_getSelectedBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                        int z);
AxisAlignedBB *BlockDoor_getCollisionBoundingBoxFromPool(Block *self, World *world, int x, int y,
                                                         int z);
void BlockDoor_setBlockBoundsBasedOnState(Block *self, IBlockAccess *world, int x, int y, int z);
void BlockDoor_onBlockClicked(Block *self, World *world, int x, int y, int z,
                              struct EntityPlayer *player);
int BlockDoor_blockActivated(Block *self, World *world, int x, int y, int z,
                             struct EntityPlayer *player);
void BlockDoor_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                     int neighborID);
int BlockDoor_idDropped(Block *self, int meta, JavaRandom *rand);
MovingObjectPosition *BlockDoor_collisionRayTrace(Block *self, World *world, int x, int y, int z,
                                                  Vec3D *start, Vec3D *end);
int BlockDoor_canPlaceBlockAt(Block *self, World *world, int x, int y, int z);
int BlockDoor_getMobilityFlag(Block *self);

void BlockDoor_setDoorRotation(Block *self, int var1);
void BlockDoor_onPoweredBlockChange(Block *self, World *world, int x, int y, int z, int powered);
int BlockDoor_getState(int meta);
int BlockDoor_isOpen(int meta);

#endif
