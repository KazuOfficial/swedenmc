#include "BlockClay.h"
#include "Item.h"
#include <stdlib.h>

BlockVtable BlockClay_defaultVtable;

void BlockClay_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->idDropped = BlockClay_idDropped;
    vt->quantityDropped = BlockClay_quantityDropped;
}

void BlockClay_construct(BlockClay *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockClay_initVtable(&BlockClay_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockClay_defaultVtable;
    Block_constructWithTexture(&self->base, id, tex, Material_clay);
}

BlockClay *BlockClay_create(int id, int tex) {
    BlockClay *self = (BlockClay *)calloc(1, sizeof(BlockClay));
    BlockClay_construct(self, id, tex);
    return self;
}

int BlockClay_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return Item_clay->shiftedIndex;
}

int BlockClay_quantityDropped(Block *self, JavaRandom *var1) {
    (void)self;
    (void)var1;
    return 4;
}
