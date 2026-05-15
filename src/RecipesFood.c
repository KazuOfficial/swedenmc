#include "RecipesFood.h"
#include "Item.h"
#include "Block.h"
#include "ItemStack.h"
#include <stdint.h>

void RecipesFood_addRecipes(RecipesFood *self, CraftingManager *var1) {
    (void)self;

    {
        ItemStack *r = ItemStack_new(Item_bowlSoup->shiftedIndex, 1, 0);
        void *a[] = {"Y",
                     "X",
                     "#",
                     (void *)(uintptr_t)'X',
                     (void *)Block_mushroomBrown,
                     (void *)(uintptr_t)'Y',
                     (void *)Block_mushroomRed,
                     (void *)(uintptr_t)'#',
                     (void *)Item_bowlEmpty};
        CraftingManager_addRecipe(var1, r, a, 9);
    }

    {
        ItemStack *r = ItemStack_new(Item_bowlSoup->shiftedIndex, 1, 0);
        void *a[] = {"Y",
                     "X",
                     "#",
                     (void *)(uintptr_t)'X',
                     (void *)Block_mushroomRed,
                     (void *)(uintptr_t)'Y',
                     (void *)Block_mushroomBrown,
                     (void *)(uintptr_t)'#',
                     (void *)Item_bowlEmpty};
        CraftingManager_addRecipe(var1, r, a, 9);
    }

    {
        ItemStack *r = ItemStack_new(Item_cookie->shiftedIndex, 8, 0);
        ItemStack *x = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 3);
        void *a[] = {"#X#", (void *)(uintptr_t)'X', (void *)x, (void *)(uintptr_t)'#',
                     (void *)Item_wheat};
        CraftingManager_addRecipe(var1, r, a, 5);
        ItemStack_free(x);
    }
}
