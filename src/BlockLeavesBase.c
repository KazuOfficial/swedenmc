#include "BlockLeavesBase.h"
#include "IBlockAccess.h"

int BlockLeavesBase_isOpaqueCube(Block *self) {
    (void)self;
    return 0;
}

int BlockLeavesBase_shouldSideBeRendered(Block *self, IBlockAccess *world, int x, int y, int z,
                                         int side) {
    BlockLeavesBase *b = (BlockLeavesBase *)self;
    int var6 = world->vtable->getBlockId(world, x, y, z);
    if (!b->graphicsLevel && var6 == self->blockID) {
        return 0;
    }
    return Block_defaultVtable.shouldSideBeRendered(self, world, x, y, z, side);
}

void BlockLeavesBase_construct(BlockLeavesBase *self, int id, int tex, Material *mat,
                               int graphicsLevel) {
    Block_constructWithTexture(&self->base, id, tex, mat);
    self->graphicsLevel = graphicsLevel;
}
