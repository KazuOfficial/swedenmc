#include "BlockBreakable.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockGlass_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 0;
}

static int BlockGlass_getRenderBlockPass(Block *self) {
    (void)self;
    return 0;
}

Block *BlockGlass_create(int id, int tex, Material *mat, int seeThrough) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;

        vtable.isOpaqueCube = BlockBreakable_isOpaqueCube;
        vtable.shouldSideBeRendered = BlockBreakable_shouldSideBeRendered;
        vtable.quantityDropped = BlockGlass_quantityDropped;
        vtable.getRenderBlockPass = BlockGlass_getRenderBlockPass;
        vtable_init = 1;
    }
    BlockBreakable *self = (BlockBreakable *)calloc(1, sizeof(BlockBreakable));
    self->base.vtable = &vtable;
    BlockBreakable_construct(self, id, tex, mat, seeThrough);
    return &self->base;
}
