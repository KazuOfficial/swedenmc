#include "ItemLeaves.h"
#include "ItemBlock.h"
#include "Block.h"
#include "ColorizerFoliage.h"
#include <stdlib.h>

static int ItemLeaves_getIconFromDamage(Item *self, int damage) {
    (void)self;
    return Block_leaves->vtable->getBlockTextureFromSideAndMetadata(Block_leaves, 0, damage);
}

static int ItemLeaves_getPlacedBlockMetadata(Item *self, int damage) {
    (void)self;
    return damage | 8;
}

static int ItemLeaves_getColorFromDamage(Item *self, int damage) {
    (void)self;
    if ((damage & 1) == 1)
        return ColorizerFoliage_getFoliageColorPine();
    if ((damage & 2) == 2)
        return ColorizerFoliage_getFoliageColorBirch();
    return ColorizerFoliage_func_31073_c();
}

Item *ItemLeaves_create(int blockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemBlock_initVtable(&vtable);
        vtable.getIconFromDamage = ItemLeaves_getIconFromDamage;
        vtable.getPlacedBlockMetadata = ItemLeaves_getPlacedBlockMetadata;
        vtable.getColorFromDamage = ItemLeaves_getColorFromDamage;
        vi = 1;
    }
    ItemBlock *self = (ItemBlock *)ItemBlock_create(blockID);
    self->base.vtable = &vtable;
    Item_setMaxDamage(&self->base, 0);
    Item_setHasSubtypes(&self->base, 1);
    return &self->base;
}
