#include "BlockDeadBush.h"
#include "Block.h"
#include <stdlib.h>

BlockVtable BlockDeadBush_defaultVtable;

static int BlockDeadBush_canThisPlantGrowOnThisBlockID(Block *self, int var1) {
    (void)self;
    return var1 == Block_sand->blockID;
}

void BlockDeadBush_initVtable(BlockVtable *vt) {
    BlockFlower_initVtable(vt);
    vt->canThisPlantGrowOnThisBlockID = BlockDeadBush_canThisPlantGrowOnThisBlockID;
    vt->getBlockTextureFromSideAndMetadata = BlockDeadBush_getBlockTextureFromSideAndMetadata;
    vt->idDropped = BlockDeadBush_idDropped;
}

void BlockDeadBush_construct(BlockDeadBush *self, int id, int tex) {
    static int vtable_init = 0;
    if (!vtable_init) {
        BlockDeadBush_initVtable(&BlockDeadBush_defaultVtable);
        vtable_init = 1;
    }
    self->base.vtable = &BlockDeadBush_defaultVtable;
    Block_construct(&self->base, id, Material_plants);
    self->base.blockIndexInTexture = tex;
    Block_setTickOnLoad(&self->base, 1);
    float var3 = 0.4F;
    Block_setBlockBounds(&self->base, 0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, 0.8F,
                         0.5F + var3);
}

BlockDeadBush *BlockDeadBush_create(int id, int tex) {
    BlockDeadBush *self = (BlockDeadBush *)calloc(1, sizeof(BlockDeadBush));
    BlockDeadBush_construct(self, id, tex);
    return self;
}

int BlockDeadBush_getBlockTextureFromSideAndMetadata(Block *self, int var1, int var2) {
    (void)var1;
    (void)var2;
    return self->blockIndexInTexture;
}

int BlockDeadBush_idDropped(Block *self, int var1, JavaRandom *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return -1;
}
