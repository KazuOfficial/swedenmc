#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockObsidian_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return Block_obsidian->blockID;
}

static int BlockObsidian_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    (void)rand;
    return 1;
}

Block *BlockObsidian_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.idDropped = BlockObsidian_idDropped;
        vtable.quantityDropped = BlockObsidian_quantityDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_rock);
    return self;
}
