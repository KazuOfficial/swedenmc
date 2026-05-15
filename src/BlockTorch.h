#ifndef BLOCKTORCH_H
#define BLOCKTORCH_H

#include "Block.h"

void BlockTorch_initVtable(BlockVtable *vt);
Block *BlockTorch_create(int id, int tex);

void BlockTorch_onBlockAdded(Block *self, World *world, int x, int y, int z);
void BlockTorch_onNeighborBlockChange(Block *self, World *world, int x, int y, int z,
                                      int neighborID);

#endif
