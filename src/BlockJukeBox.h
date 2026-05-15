#ifndef BLOCK_JUKEBOX_H
#define BLOCK_JUKEBOX_H

#include "Block.h"
#include "World.h"

Block *BlockJukeBox_create(int id, int tex);

void BlockJukeBox_ejectRecord(Block *self, World *var1, int var2, int var3, int var4, int var5);

#endif
