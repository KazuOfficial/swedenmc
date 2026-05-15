#include "BlockStone.h"
#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockStone_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;
    return Block_cobblestone->blockID;
}

Block *BlockStone_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.idDropped = BlockStone_idDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, Material_rock);
    return self;
}
