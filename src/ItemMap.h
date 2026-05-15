#ifndef ITEM_MAP_H
#define ITEM_MAP_H

#include "ItemMapBase.h"
#include "MapData.h"
#include "World.h"

Item *ItemMap_create(int id);

MapData *ItemMap_func_28012_a(Item *self, struct ItemStack *var1, World *var2);

MapData *ItemMap_func_28013_a(short var0, World *var1);

#endif
