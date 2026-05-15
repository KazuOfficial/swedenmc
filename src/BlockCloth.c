#include "BlockCloth.h"
#include <stdlib.h>

BlockVtable BlockCloth_defaultVtable;

void BlockCloth_initVtable(BlockVtable *vt) {
    *vt = Block_defaultVtable;
    vt->getBlockTextureFromSideAndMetadata = BlockCloth_getBlockTextureFromSideAndMetadata;
    vt->damageDropped = BlockCloth_damageDropped;
}

void BlockCloth_construct(BlockCloth *self) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockCloth_initVtable(&BlockCloth_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockCloth_defaultVtable;
    Block_constructWithTexture(&self->base, 35, 64, Material_cloth);
}

BlockCloth *BlockCloth_create(void) {
    BlockCloth *self = (BlockCloth *)calloc(1, sizeof(BlockCloth));
    BlockCloth_construct(self);
    return self;
}

int BlockCloth_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    (void)var1;
    if (var2 == 0) {
        return self->blockIndexInTexture;
    } else {
        var2 = ~(var2 & 15);
        return 113 + ((var2 & 8) >> 3) + (var2 & 7) * 16;
    }
}

int BlockCloth_damageDropped(Block *self, int var1) {
    (void)self;
    return var1;
}

int BlockCloth_func_21034_c(int var0) { return ~var0 & 15; }

int BlockCloth_func_21035_d(int var0) { return ~var0 & 15; }
