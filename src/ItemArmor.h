#ifndef ITEM_ARMOR_H
#define ITEM_ARMOR_H

#include "Item.h"

typedef struct {
    Item base;
    int armorLevel;
    int armorType;
    int damageReduceAmount;
    int renderIndex;
} ItemArmor;

Item *ItemArmor_create(int id, int armorLevel, int renderIndex, int armorType);

int ItemArmor_isInstance(Item *item);

#endif
