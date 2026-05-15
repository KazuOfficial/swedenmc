#include "BlockDirt.h"
#include "Material.h"
#include <stdlib.h>

BlockVtable BlockDirt_defaultVtable;

void BlockDirt_initVtable(BlockVtable *vt) { *vt = Block_defaultVtable; }

void BlockDirt_construct(BlockDirt *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockDirt_initVtable(&BlockDirt_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockDirt_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_ground);
}

BlockDirt *BlockDirt_create(int id, int tex) {
    BlockDirt *self = (BlockDirt *)calloc(1, sizeof(BlockDirt));
    BlockDirt_construct(self, id, tex);
    return self;
}
