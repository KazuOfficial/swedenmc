#ifndef ITEM_BLOCK_H
#define ITEM_BLOCK_H

#include "Item.h"

typedef struct {
    Item base;
    int blockID;
} ItemBlock;

void ItemBlock_initVtable(ItemVtable *vtable);

Item *ItemBlock_create(int blockID);

const char *ItemBlock_getItemName(Item *self);

Item *ItemCloth_create(int blockID);

#endif
