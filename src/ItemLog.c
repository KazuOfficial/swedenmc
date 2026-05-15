#include "ItemLog.h"
#include "ItemBlock.h"
#include "Block.h"
#include <stdlib.h>

static int ItemLog_getIconFromDamage(Item *self, int damage) {
    (void)self;
    return Block_wood->vtable->getBlockTextureFromSideAndMetadata(Block_wood, 2, damage);
}

static int ItemLog_getPlacedBlockMetadata(Item *self, int damage) {
    (void)self;
    return damage;
}

Item *ItemLog_create(int blockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemBlock_initVtable(&vtable);
        vtable.getIconFromDamage = ItemLog_getIconFromDamage;
        vtable.getPlacedBlockMetadata = ItemLog_getPlacedBlockMetadata;
        vi = 1;
    }
    ItemBlock *self = (ItemBlock *)ItemBlock_create(blockID);
    self->base.vtable = &vtable;
    Item_setMaxDamage(&self->base, 0);
    Item_setHasSubtypes(&self->base, 1);
    return &self->base;
}
