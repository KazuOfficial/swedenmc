#ifndef ITEM_FOOD_H
#define ITEM_FOOD_H

#include "Item.h"

typedef struct {
    Item base;

    int healAmount;

    int isWolfsFavoriteMeat;
} ItemFood;

Item *ItemFood_create(int id, int heal, int isWolfMeat);

void ItemFood_initVtable(ItemVtable *vtable);

struct ItemStack *ItemFood_onItemRightClick(Item *self, struct ItemStack *stack,
                                            struct World *world, struct EntityPlayer *player);

int ItemFood_getHealAmount(ItemFood *self);

int ItemFood_getIsWolfsFavoriteMeat(ItemFood *self);

#endif
