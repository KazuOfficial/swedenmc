#ifndef RECIPE_SORTER_H
#define RECIPE_SORTER_H

#include "IRecipe.h"
#include "CraftingManager.h"

typedef struct RecipeSorter {
    CraftingManager *craftingManager;
} RecipeSorter;

RecipeSorter *RecipeSorter_create(CraftingManager *var1);

int RecipeSorter_compareRecipes(RecipeSorter *self, IRecipe *var1, IRecipe *var2);

int RecipeSorter_compare(RecipeSorter *self, void *var1, void *var2);

#endif
