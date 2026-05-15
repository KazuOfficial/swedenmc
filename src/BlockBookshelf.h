#ifndef BLOCK_BOOKSHELF_H
#define BLOCK_BOOKSHELF_H

#include "Block.h"

typedef struct BlockBookshelf {
    Block base;
} BlockBookshelf;

void BlockBookshelf_construct(BlockBookshelf *self, int id, int tex);
BlockBookshelf *BlockBookshelf_create(int id, int tex);

int BlockBookshelf_getBlockTextureFromSide(Block *self, int side);
int BlockBookshelf_quantityDropped(Block *self, JavaRandom *rand);

extern BlockVtable BlockBookshelf_defaultVtable;

#endif
