#ifndef IRECIPE_H
#define IRECIPE_H

#include "GameForward.h"

struct IRecipe;

typedef struct IRecipeVtable {

    int (*matches)(struct IRecipe *self, InventoryCrafting *var1);

    ItemStack *(*getCraftingResult)(struct IRecipe *self, InventoryCrafting *var1);

    int (*getRecipeSize)(struct IRecipe *self);

    ItemStack *(*getRecipeOutput)(struct IRecipe *self);

    void (*destroy)(struct IRecipe *self);
} IRecipeVtable;

typedef struct IRecipe {
    const IRecipeVtable *vtable;
} IRecipe;

#endif
