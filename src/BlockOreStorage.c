#include "Block.h"
#include "Material.h"
#include <stdlib.h>

static int BlockOreStorage_getBlockTextureFromSide(Block *self, int side) {
    (void)side;
    return self->blockIndexInTexture;
}

Block *BlockOreStorage_create(int id, int tex) {
    static BlockVtable vtable;
    static int vtable_init = 0;
    if (!vtable_init) {
        vtable = Block_defaultVtable;
        vtable.getBlockTextureFromSide = BlockOreStorage_getBlockTextureFromSide;
        vtable_init = 1;
    }
    Block *self = (Block *)calloc(1, sizeof(Block));
    self->vtable = &vtable;
    Block_construct(self, id, Material_iron);
    self->blockIndexInTexture = tex;
    return self;
}
