#include "Block.h"
#include "Material.h"
#include <stdlib.h>

static int BlockSandStone_getBlockTextureFromSide(Block *self, int side) {
    if (side == 1)
        return self->blockIndexInTexture - 16;
    if (side == 0)
        return self->blockIndexInTexture + 16;
    return self->blockIndexInTexture;
}

Block *BlockSandStone_create(int id) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTextureFromSide = BlockSandStone_getBlockTextureFromSide;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_constructWithTexture(self, id, 192, Material_rock);
    return self;
}
