#include "RecipesCrafting.h"
#include "Block.h"
#include "ItemStack.h"
#include <stdint.h>

void RecipesCrafting_addRecipes(RecipesCrafting *self, CraftingManager *var1) {
    (void)self;
    {
        ItemStack *r = ItemStack_new(Block_chest->blockID, 1, 0);
        void *a[] = {"###", "# #", "###", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(var1, r, a, 5);
    }
    {
        ItemStack *r = ItemStack_new(Block_stoneOvenIdle->blockID, 1, 0);
        void *a[] = {"###", "# #", "###", (void *)(uintptr_t)'#', (void *)Block_cobblestone};
        CraftingManager_addRecipe(var1, r, a, 5);
    }
    {
        ItemStack *r = ItemStack_new(Block_workbench->blockID, 1, 0);
        void *a[] = {"##", "##", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(var1, r, a, 4);
    }
    {
        ItemStack *r = ItemStack_new(Block_sandStone->blockID, 1, 0);
        void *a[] = {"##", "##", (void *)(uintptr_t)'#', (void *)Block_sand};
        CraftingManager_addRecipe(var1, r, a, 4);
    }
}
