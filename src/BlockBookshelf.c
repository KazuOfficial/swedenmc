#include "BlockBookshelf.h"
#include <stdlib.h>

BlockVtable BlockBookshelf_defaultVtable;

static void init_vtable(void) {
    BlockBookshelf_defaultVtable = Block_defaultVtable;
    BlockBookshelf_defaultVtable.getBlockTextureFromSide = BlockBookshelf_getBlockTextureFromSide;
    BlockBookshelf_defaultVtable.quantityDropped = BlockBookshelf_quantityDropped;
}

void BlockBookshelf_construct(BlockBookshelf *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        init_vtable();
        vtable_init = 1;
    }
    self->base.vtable = &BlockBookshelf_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_wood);
}

BlockBookshelf *BlockBookshelf_create(int id, int tex) {
    BlockBookshelf *self = (BlockBookshelf *)calloc(1, sizeof(BlockBookshelf));
    BlockBookshelf_construct(self, id, tex);
    return self;
}

int BlockBookshelf_getBlockTextureFromSide(Block *self, int var1) {
    return var1 <= 1 ? 4 : self->blockIndexInTexture;
}

int BlockBookshelf_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 0;
}
