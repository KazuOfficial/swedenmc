#include "ItemPiston.h"
#include "ItemBlock.h"
#include <stdlib.h>

static int ItemPiston_getPlacedBlockMetadata(Item *self, int damage) {
    (void)self;
    (void)damage;
    return 7;
}

Item *ItemPiston_create(int blockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemBlock_initVtable(&vtable);
        vtable.getPlacedBlockMetadata = ItemPiston_getPlacedBlockMetadata;
        vi = 1;
    }
    ItemBlock *self = (ItemBlock *)ItemBlock_create(blockID);
    self->base.vtable = &vtable;
    return &self->base;
}
