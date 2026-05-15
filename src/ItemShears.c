#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include <stdlib.h>

static int ItemShears_onBlockDestroyed(Item *self, ItemStack *stack, int blockID, int x, int y,
                                       int z, EntityLiving *player) {
    if (blockID == Block_leaves->blockID || blockID == Block_web->blockID)
        ItemStack_damageItem(stack, 1, (struct Entity *)player);
    return Item_defaultVtable.onBlockDestroyed(self, stack, blockID, x, y, z, player);
}

static int ItemShears_canHarvestBlock(Item *self, struct Block *block) {
    (void)self;
    return block->blockID == Block_web->blockID;
}

static float ItemShears_getStrVsBlock(Item *self, ItemStack *stack, struct Block *block) {
    if (block->blockID == Block_web->blockID || block->blockID == Block_leaves->blockID)
        return 15.0f;
    if (block->blockID == Block_cloth->blockID)
        return 5.0f;
    return Item_defaultVtable.getStrVsBlock(self, stack, block);
}

Item *ItemShears_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onBlockDestroyed = ItemShears_onBlockDestroyed;
        vtable.canHarvestBlock = ItemShears_canHarvestBlock;
        vtable.getStrVsBlock = ItemShears_getStrVsBlock;
        vi = 1;
    }
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    self->maxStackSize = 1;
    Item_setMaxDamage(self, 238);
    return self;
}
