#include "ItemSlab.h"
#include "ItemBlock.h"
#include "Block.h"
#include <stdlib.h>
#include <stdio.h>

static const char *BlockStep_field_22037_a[] = {"stone", "sand", "wood", "cobble"};

static int ItemSlab_getIconFromDamage(Item *self, int damage) {
    (void)self;
    return Block_stairSingle->vtable->getBlockTextureFromSideAndMetadata(Block_stairSingle, 2,
                                                                         damage);
}

static int ItemSlab_getPlacedBlockMetadata(Item *self, int damage) {
    (void)self;
    return damage;
}

static const char *ItemSlab_getItemNameIS(Item *self, ItemStack *stack) {
    static char buf[64];
    const char *base = ItemBlock_getItemName(self);
    snprintf(buf, sizeof(buf), "%s.%s", base, BlockStep_field_22037_a[stack->itemDamage]);
    return buf;
}

Item *ItemSlab_create(int blockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemBlock_initVtable(&vtable);
        vtable.getIconFromDamage = ItemSlab_getIconFromDamage;
        vtable.getPlacedBlockMetadata = ItemSlab_getPlacedBlockMetadata;
        vtable.getItemNameIS = ItemSlab_getItemNameIS;
        vi = 1;
    }
    ItemBlock *self = (ItemBlock *)ItemBlock_create(blockID);
    self->base.vtable = &vtable;
    Item_setMaxDamage(&self->base, 0);
    Item_setHasSubtypes(&self->base, 1);
    return &self->base;
}
