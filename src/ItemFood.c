#include "ItemFood.h"
#include "ItemStack.h"
#include "EntityLiving.h"
#include <stdlib.h>

ItemStack *ItemFood_onItemRightClick(Item *self, ItemStack *stack, World *world,
                                     EntityPlayer *player) {
    (void)world;
    ItemFood *f = (ItemFood *)self;
    --stack->stackSize;
    EntityLiving_heal((EntityLiving *)player, f->healAmount);
    return stack;
}

void ItemFood_initVtable(ItemVtable *vtable) {
    *vtable = Item_defaultVtable;
    vtable->onItemRightClick = ItemFood_onItemRightClick;
}

Item *ItemFood_create(int id, int heal, int isWolfMeat) {
    static ItemVtable vtable;
    static int vi = 0;
    if (!vi) {
        ItemFood_initVtable(&vtable);
        vi = 1;
    }

    ItemFood *self = (ItemFood *)calloc(1, sizeof(ItemFood));
    Item_construct(&self->base, id);
    self->base.vtable = &vtable;
    self->base.maxStackSize = 1;
    self->healAmount = heal;
    self->isWolfsFavoriteMeat = isWolfMeat;
    self->base.isFood = 1;
    return &self->base;
}

Item *ItemCookie_create(int id, int heal, int isWolfMeat, int maxStack) {
    Item *self = ItemFood_create(id, heal, isWolfMeat);
    self->maxStackSize = maxStack;
    return self;
}

int ItemFood_getHealAmount(ItemFood *self) { return self->healAmount; }

int ItemFood_getIsWolfsFavoriteMeat(ItemFood *self) { return self->isWolfsFavoriteMeat; }
