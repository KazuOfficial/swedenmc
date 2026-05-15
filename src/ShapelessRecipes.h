#ifndef SHAPELESS_RECIPES_H
#define SHAPELESS_RECIPES_H

#include "IRecipe.h"
#include "ItemStack.h"

typedef struct ShapelessRecipes {
    IRecipe base;
    ItemStack recipeOutput;
    ItemStack *recipeItems;
    int recipeItemsCount;
} ShapelessRecipes;

ShapelessRecipes *ShapelessRecipes_create(ItemStack *var1, ItemStack *var2Items, int var2Count);

int ShapelessRecipes_isInstance(IRecipe *r);

#endif
