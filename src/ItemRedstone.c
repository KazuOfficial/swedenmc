#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "World.h"
#include <stdlib.h>

static int ItemRedstone_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                                  int var4, int var5, int var6, int var7) {
    (void)self;
    (void)var2;
    if (World_getBlockId(var3, var4, var5, var6) != Block_snow->blockID) {
        if (var7 == 0) {
            --var5;
        }
        if (var7 == 1) {
            ++var5;
        }
        if (var7 == 2) {
            --var6;
        }
        if (var7 == 3) {
            ++var6;
        }
        if (var7 == 4) {
            --var4;
        }
        if (var7 == 5) {
            ++var4;
        }
        if (!World_isAirBlock(var3, var4, var5, var6)) {
            return 0;
        }
    }
    if (Block_redstoneWire->vtable->canPlaceBlockAt(Block_redstoneWire, var3, var4, var5, var6)) {
        --var1->stackSize;
        World_setBlockWithNotify(var3, var4, var5, var6, Block_redstoneWire->blockID);
    }
    return 1;
}

Item *ItemRedstone_create(int id) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemRedstone_onItemUse;
        vi = 1;
    }

    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    self->vtable = &vtable;
    return self;
}
