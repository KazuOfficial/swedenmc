#ifndef BLOCK_CLAY_H
#define BLOCK_CLAY_H

#include "Block.h"

typedef struct BlockClay {
    Block base;
} BlockClay;

void BlockClay_construct(BlockClay *self, int id, int tex);
BlockClay *BlockClay_create(int id, int tex);

void BlockClay_initVtable(BlockVtable *vt);
extern BlockVtable BlockClay_defaultVtable;

int BlockClay_idDropped(Block *self, int meta, JavaRandom *rand);
int BlockClay_quantityDropped(Block *self, JavaRandom *rand);

#endif
