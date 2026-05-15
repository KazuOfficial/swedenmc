#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "World.h"
#include <stdlib.h>

typedef struct {
    Item base;
    int cropBlockID;
} ItemSeeds;

static int ItemSeeds_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                               int var4, int var5, int var6, int var7) {
    (void)var2;
    ItemSeeds *s = (ItemSeeds *)self;
    if (var7 != 1) {
        return 0;
    } else {
        int var8 = World_getBlockId(var3, var4, var5, var6);
        if (var8 == Block_tilledField->blockID && World_isAirBlock(var3, var4, var5 + 1, var6)) {
            World_setBlockWithNotify(var3, var4, var5 + 1, var6, s->cropBlockID);
            --var1->stackSize;
            return 1;
        } else {
            return 0;
        }
    }
}

Item *ItemSeeds_create(int id, int cropBlockID) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemSeeds_onItemUse;
        vi = 1;
    }

    ItemSeeds *self = (ItemSeeds *)calloc(1, sizeof(ItemSeeds));
    Item_construct(&self->base, id);
    self->base.vtable = &vtable;
    self->cropBlockID = cropBlockID;
    return &self->base;
}
