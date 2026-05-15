#ifndef BLOCKCONTAINER_H
#define BLOCKCONTAINER_H

#include "Block.h"

void BlockContainer_initVtable(BlockVtable *vt);
void BlockContainer_construct(Block *self, int id, Material *mat);
void BlockContainer_constructWithTexture(Block *self, int id, int tex, Material *mat);

void BlockContainer_onBlockRemoval(Block *self, World *world, int x, int y, int z);

#endif
