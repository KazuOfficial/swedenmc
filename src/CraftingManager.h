#ifndef CRAFTING_MANAGER_H
#define CRAFTING_MANAGER_H

#include "IRecipe.h"
#include "ItemStack.h"

struct InventoryCrafting;

typedef struct IRecipeList {
    struct IRecipe **entries;
    int count;
    int capacity;
} IRecipeList;

typedef struct CraftingManager {

    IRecipeList recipes;
} CraftingManager;

CraftingManager *CraftingManager_getInstance(void);

void CraftingManager_addRecipe(CraftingManager *self, ItemStack *var1, void **var2, int var2_len);

void CraftingManager_addShapelessRecipe(CraftingManager *self, ItemStack *var1, void **var2,
                                        int var2_len);

ItemStack *CraftingManager_findMatchingRecipe(CraftingManager *self,
                                              struct InventoryCrafting *var1);

IRecipeList *CraftingManager_getRecipeList(CraftingManager *self);

void CraftingManager_destroy(CraftingManager *self);

#endif
