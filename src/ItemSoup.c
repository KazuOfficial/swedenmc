#include "ItemSoup.h"
#include "ItemFood.h"
#include "ItemStack.h"
#include "Item.h"

static ItemStack *ItemSoup_onItemRightClick(Item *self, ItemStack *stack, World *world,
                                            EntityPlayer *player) {
    ItemFood_onItemRightClick(self, stack, world, player);
    return ItemStack_new(Item_bowlEmpty->shiftedIndex, 1, 0);
}

Item *ItemSoup_create(int id, int heal) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemFood_initVtable(&vtable);
        vtable.onItemRightClick = ItemSoup_onItemRightClick;
        vi = 1;
    }
    Item *self = ItemFood_create(id, heal, 0);
    self->vtable = &vtable;
    return self;
}
