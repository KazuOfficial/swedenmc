#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include <stdlib.h>

static int BlockGlowStone_idDropped(Block *self, int meta, JavaRandom *rand) {
    (void)self;
    (void)meta;
    (void)rand;

    return 348;
}

static int BlockGlowStone_quantityDropped(Block *self, JavaRandom *rand) {
    (void)self;
    return 2 + JavaRandom_nextInt(rand, 3);
}

Block *BlockGlowStone_create(int id, int tex, Material *mat) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.idDropped = BlockGlowStone_idDropped;
        vtable.quantityDropped = BlockGlowStone_quantityDropped;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, tex, mat);
    return self;
}
