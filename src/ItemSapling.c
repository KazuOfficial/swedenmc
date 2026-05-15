#include "ItemSapling.h"
#include "ItemBlock.h"
#include "Block.h"
#include <stdlib.h>

static int ItemSapling_getPlacedBlockMetadata(Item *self, int damage) {
    (void)self;
    return damage;
}

static int ItemSapling_getIconFromDamage(Item *self, int damage) {
    (void)self;
    return Block_sapling->vtable->getBlockTextureFromSideAndMetadata(Block_sapling, 0, damage);
}

Item *ItemSapling_create(int blockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemBlock_initVtable(&vtable);
        vtable.getIconFromDamage = ItemSapling_getIconFromDamage;
        vtable.getPlacedBlockMetadata = ItemSapling_getPlacedBlockMetadata;
        vi = 1;
    }
    ItemBlock *self = (ItemBlock *)ItemBlock_create(blockID);
    self->base.vtable = &vtable;
    Item_setMaxDamage(&self->base, 0);
    Item_setHasSubtypes(&self->base, 1);
    return &self->base;
}
