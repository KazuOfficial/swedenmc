#include "ShapelessRecipes.h"
#include "InventoryCrafting.h"
#include <stdlib.h>
#include <string.h>

static int slr_matches(IRecipe *self, InventoryCrafting *var1);
static ItemStack *slr_getCraftingResult(IRecipe *self, InventoryCrafting *var1);
static int slr_getRecipeSize(IRecipe *self);
static ItemStack *slr_getRecipeOutput(IRecipe *self);
static void slr_destroy(IRecipe *self);

static void slr_destroy(IRecipe *iself) {
    ShapelessRecipes *self = (ShapelessRecipes *)iself;
    free(self->recipeItems);
    free(self);
}

static const IRecipeVtable ShapelessRecipes_vtable = {
    slr_matches, slr_getCraftingResult, slr_getRecipeSize, slr_getRecipeOutput, slr_destroy,
};

ShapelessRecipes *ShapelessRecipes_create(ItemStack *var1, ItemStack *var2Items, int var2Count) {
    ShapelessRecipes *self = (ShapelessRecipes *)calloc(1, sizeof(ShapelessRecipes));
    self->base.vtable = &ShapelessRecipes_vtable;
    self->recipeOutput = *var1;
    self->recipeItems = var2Items;
    self->recipeItemsCount = var2Count;
    return self;
}

static ItemStack *slr_getRecipeOutput(IRecipe *iself) {
    ShapelessRecipes *self = (ShapelessRecipes *)iself;
    return &self->recipeOutput;
}

static int slr_matches(IRecipe *iself, InventoryCrafting *var1) {
    ShapelessRecipes *self = (ShapelessRecipes *)iself;
    int n = self->recipeItemsCount;

    int used[64];
    if (n > 64)
        n = 64;
    memset(used, 0, n * sizeof(int));

    for (int var3 = 0; var3 < 3; ++var3) {
        for (int var4 = 0; var4 < 3; ++var4) {
            ItemStack *var5 = InventoryCrafting_func_21103_b(var1, var4, var3);
            if (var5 != NULL) {
                int var6 = 0;

                for (int j = 0; j < n; j++) {
                    if (used[j])
                        continue;
                    ItemStack *var8 = &self->recipeItems[j];
                    if (var5->itemID == var8->itemID &&
                        (ItemStack_getItemDamage(var8) == -1 ||
                         ItemStack_getItemDamage(var5) == ItemStack_getItemDamage(var8))) {
                        var6 = 1;
                        used[j] = 1;
                        break;
                    }
                }
                if (!var6)
                    return 0;
            }
        }
    }

    for (int j = 0; j < n; j++) {
        if (!used[j])
            return 0;
    }
    return 1;
}

static ItemStack *slr_getCraftingResult(IRecipe *iself, InventoryCrafting *var1) {
    ShapelessRecipes *self = (ShapelessRecipes *)iself;
    (void)var1;
    ItemStack copy = ItemStack_copy(&self->recipeOutput);
    ItemStack *result = (ItemStack *)calloc(1, sizeof(ItemStack));
    *result = copy;
    return result;
}

static int slr_getRecipeSize(IRecipe *iself) {
    ShapelessRecipes *self = (ShapelessRecipes *)iself;
    return self->recipeItemsCount;
}

int ShapelessRecipes_isInstance(IRecipe *r) {
    return r != NULL && r->vtable == &ShapelessRecipes_vtable;
}
