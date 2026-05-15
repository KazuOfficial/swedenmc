#include "ShapedRecipes.h"
#include "InventoryCrafting.h"
#include <stdlib.h>
#include <string.h>

static int sr_matches(IRecipe *self, InventoryCrafting *var1);
static ItemStack *sr_getCraftingResult(IRecipe *self, InventoryCrafting *var1);
static int sr_getRecipeSize(IRecipe *self);
static ItemStack *sr_getRecipeOutput(IRecipe *self);
static void sr_destroy(IRecipe *self);

static void sr_destroy(IRecipe *iself) {
    ShapedRecipes *self = (ShapedRecipes *)iself;
    for (int i = 0; i < self->recipeItemsLength; i++)
        ItemStack_free(self->recipeItems[i]);
    free(self->recipeItems);
    free(self);
}

static const IRecipeVtable ShapedRecipes_vtable = {
    sr_matches, sr_getCraftingResult, sr_getRecipeSize, sr_getRecipeOutput, sr_destroy,
};

ShapedRecipes *ShapedRecipes_create(int var1, int var2, ItemStack **var3, int recipeItemsLength,
                                    ItemStack *var4) {
    ShapedRecipes *self = (ShapedRecipes *)calloc(1, sizeof(ShapedRecipes));
    self->base.vtable = &ShapedRecipes_vtable;
    self->recipeOutputItemID = var4->itemID;
    self->recipeWidth = var1;
    self->recipeHeight = var2;
    self->recipeItems = var3;
    self->recipeItemsLength = recipeItemsLength;
    self->recipeOutput = *var4;
    return self;
}

static ItemStack *sr_getRecipeOutput(IRecipe *iself) {
    ShapedRecipes *self = (ShapedRecipes *)iself;
    return &self->recipeOutput;
}

static int sr_func_21137_a(ShapedRecipes *self, InventoryCrafting *var1, int var2, int var3,
                           int var4) {
    for (int var5 = 0; var5 < 3; ++var5) {
        for (int var6 = 0; var6 < 3; ++var6) {
            int var7 = var5 - var2;
            int var8 = var6 - var3;
            ItemStack *var9 = NULL;
            if (var7 >= 0 && var8 >= 0 && var7 < self->recipeWidth && var8 < self->recipeHeight) {
                if (var4) {
                    var9 =
                        self->recipeItems[self->recipeWidth - var7 - 1 + var8 * self->recipeWidth];
                } else {
                    var9 = self->recipeItems[var7 + var8 * self->recipeWidth];
                }
            }

            ItemStack *var10 = InventoryCrafting_func_21103_b(var1, var5, var6);
            if (var10 != NULL || var9 != NULL) {
                if ((var10 == NULL && var9 != NULL) || (var10 != NULL && var9 == NULL)) {
                    return 0;
                }
                if (var9->itemID != var10->itemID) {
                    return 0;
                }
                if (ItemStack_getItemDamage(var9) != -1 &&
                    ItemStack_getItemDamage(var9) != ItemStack_getItemDamage(var10)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

static int sr_matches(IRecipe *iself, InventoryCrafting *var1) {
    ShapedRecipes *self = (ShapedRecipes *)iself;
    for (int var2 = 0; var2 <= 3 - self->recipeWidth; ++var2) {
        for (int var3 = 0; var3 <= 3 - self->recipeHeight; ++var3) {
            if (sr_func_21137_a(self, var1, var2, var3, 1))
                return 1;
            if (sr_func_21137_a(self, var1, var2, var3, 0))
                return 1;
        }
    }
    return 0;
}

static ItemStack *sr_getCraftingResult(IRecipe *iself, InventoryCrafting *var1) {
    ShapedRecipes *self = (ShapedRecipes *)iself;
    (void)var1;
    return ItemStack_new(self->recipeOutput.itemID, self->recipeOutput.stackSize,
                         ItemStack_getItemDamage(&self->recipeOutput));
}

static int sr_getRecipeSize(IRecipe *iself) {
    ShapedRecipes *self = (ShapedRecipes *)iself;
    return self->recipeWidth * self->recipeHeight;
}

int ShapedRecipes_isInstance(IRecipe *r) { return r != NULL && r->vtable == &ShapedRecipes_vtable; }
