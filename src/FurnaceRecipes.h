#ifndef FURNACE_RECIPES_H
#define FURNACE_RECIPES_H

#include "ItemStack.h"

typedef struct SmeltingEntry {
    int key;
    ItemStack value;
} SmeltingEntry;

typedef struct FurnaceRecipes {
    SmeltingEntry *smeltingList;
    int smeltingCount;
    int smeltingCap;
} FurnaceRecipes;

FurnaceRecipes *FurnaceRecipes_smelting(void);

void FurnaceRecipes_addSmelting(FurnaceRecipes *self, int var1, ItemStack var2);

ItemStack *FurnaceRecipes_getSmeltingResult(FurnaceRecipes *self, int var1);

SmeltingEntry *FurnaceRecipes_getSmeltingList(FurnaceRecipes *self, int *outCount);

#endif
