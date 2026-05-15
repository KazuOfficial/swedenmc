#include "Item.h"
#include "ItemStack.h"
#include "Block.h"
#include "BlockRail.h"
#include "World.h"
#include "EntityPlayer.h"
#include "EntityMinecart.h"
#include <stdlib.h>

typedef struct {
    Item base;
    int minecartType;
} ItemMinecart;

static int ItemMinecart_onItemUse(Item *self, ItemStack *var1, EntityPlayer *var2, World *var3,
                                  int var4, int var5, int var6, int var7) {
    (void)var2;
    (void)var7;
    ItemMinecart *mc = (ItemMinecart *)self;
    int var8 = World_getBlockId(var3, var4, var5, var6);
    if (BlockRail_isRailBlock(var8)) {
        if (!var3->multiplayerWorld) {
            EntityMinecart *cart = EntityMinecart_create(
                var3, (double)((float)var4 + 0.5f), (double)((float)var5 + 0.5f),
                (double)((float)var6 + 0.5f), mc->minecartType);
            World_entityJoinedWorld(var3, (Entity *)cart);
        }
        --var1->stackSize;
        return 1;
    } else {
        return 0;
    }
}

Item *ItemMinecart_create(int id, int minecartType) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        vtable = Item_defaultVtable;
        vtable.onItemUse = ItemMinecart_onItemUse;
        vi = 1;
    }

    ItemMinecart *self = (ItemMinecart *)calloc(1, sizeof(ItemMinecart));
    Item_construct(&self->base, id);
    self->base.vtable = &vtable;
    self->base.maxStackSize = 1;
    self->minecartType = minecartType;
    return &self->base;
}
