#include "Block.h"
#include "BlockContainer.h"
#include "Material.h"
#include "World.h"
#include <stdlib.h>

static void BlockContainer_onBlockAdded(Block *self, World *world, int x, int y, int z) {
    (void)self;
    (void)world;
    (void)x;
    (void)y;
    (void)z;
}

void BlockContainer_onBlockRemoval(Block *self, World *world, int x, int y, int z) {
    (void)self;
    World_removeBlockTileEntity(world, x, y, z);
}

void BlockContainer_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->onBlockAdded = BlockContainer_onBlockAdded;
    vt->onBlockRemoval = BlockContainer_onBlockRemoval;
}

void BlockContainer_construct(Block *self, int id, Material *mat) {
    Block_construct(self, id, mat);
    Block_isBlockContainer[id] = 1;
}

void BlockContainer_constructWithTexture(Block *self, int id, int tex, Material *mat) {
    Block_constructWithTexture(self, id, tex, mat);
    Block_isBlockContainer[id] = 1;
}
