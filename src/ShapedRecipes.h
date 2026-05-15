#ifndef SHAPED_RECIPES_H
#define SHAPED_RECIPES_H

#include "IRecipe.h"
#include "ItemStack.h"

typedef struct ShapedRecipes {
    IRecipe base;
    int recipeWidth;
    int recipeHeight;
    ItemStack **recipeItems;
    int recipeItemsLength;
    ItemStack recipeOutput;
    int recipeOutputItemID;
} ShapedRecipes;

ShapedRecipes *ShapedRecipes_create(int recipeWidth, int recipeHeight, ItemStack **recipeItems,
                                    int recipeItemsLength, ItemStack *recipeOutput);

int ShapedRecipes_isInstance(IRecipe *r);

#endif
