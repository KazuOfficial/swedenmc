#ifndef BLOCKFLOWER_H
#define BLOCKFLOWER_H

#include "Block.h"

void BlockFlower_initVtable(BlockVtable *vt);
Block *BlockFlower_create(int id, int tex);

void BlockFlower_updateTick(Block *self, World *world, int x, int y, int z, JavaRandom *rand);

#endif
