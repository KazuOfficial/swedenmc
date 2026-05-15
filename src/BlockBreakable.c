#include "BlockBreakable.h"
#include "IBlockAccess.h"
#include <stdlib.h>

int BlockBreakable_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

int BlockBreakable_shouldSideBeRendered(Block *self, IBlockAccess *world, int x, int y, int z,
                                        int side) {
    BlockBreakable *b = (BlockBreakable *)self;
    int var6 = world->vtable->getBlockId(world, x, y, z);
    if (!b->localFlag && var6 == self->blockID) {
        return 0;
    }
    return Block_defaultVtable.shouldSideBeRendered(self, world, x, y, z, side);
}

void BlockBreakable_construct(BlockBreakable *self, int id, int tex, Material *mat, int localFlag) {
    Block_constructWithTexture(&self->base, id, tex, mat);
    self->localFlag = localFlag;
}
