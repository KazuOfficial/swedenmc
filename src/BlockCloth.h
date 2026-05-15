#ifndef BLOCK_CLOTH_H
#define BLOCK_CLOTH_H

#include "Block.h"

typedef struct BlockCloth {
    Block base;
} BlockCloth;

void BlockCloth_construct(BlockCloth *self);
BlockCloth *BlockCloth_create(void);

void BlockCloth_initVtable(BlockVtable *vt);
extern BlockVtable BlockCloth_defaultVtable;

int BlockCloth_getBlockTextureFromSideAndMetadata(Block *self, int side, int meta);
int BlockCloth_damageDropped(Block *self, int meta);

int BlockCloth_func_21034_c(int var0);

int BlockCloth_func_21035_d(int var0);

#endif
