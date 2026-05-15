#ifndef BLOCK_DEAD_BUSH_H
#define BLOCK_DEAD_BUSH_H

#include "Block.h"
#include "BlockFlower.h"

typedef struct BlockDeadBush {
    Block base;
} BlockDeadBush;

void BlockDeadBush_construct(BlockDeadBush *self, int id, int tex);
BlockDeadBush *BlockDeadBush_create(int id, int tex);

void BlockDeadBush_initVtable(BlockVtable *vt);
extern BlockVtable BlockDeadBush_defaultVtable;

int BlockDeadBush_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta);
int BlockDeadBush_idDropped(Block *self, int meta, JavaRandom *rand);

#endif
