#ifndef BLOCK_DIRT_H
#define BLOCK_DIRT_H

#include "Block.h"

typedef struct BlockDirt {
    Block base;
} BlockDirt;

void BlockDirt_construct(BlockDirt *self, int id, int tex);
BlockDirt *BlockDirt_create(int id, int tex);

void BlockDirt_initVtable(BlockVtable *vt);
extern BlockVtable BlockDirt_defaultVtable;

#endif
