#include "RecipesIngots.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include <stdint.h>

void RecipesIngots_addRecipes(RecipesIngots *self, CraftingManager *var1) {
    (void)self;

    Block *blocks[4] = {Block_blockGold, Block_blockSteel, Block_blockDiamond, Block_blockLapis};
    ItemStack *ingots[4] = {ItemStack_new(Item_ingotGold->shiftedIndex, 9, 0),
                            ItemStack_new(Item_ingotIron->shiftedIndex, 9, 0),
                            ItemStack_new(Item_diamond->shiftedIndex, 9, 0),
                            ItemStack_new(Item_dyePowder->shiftedIndex, 9, 4)};

    for (int var2 = 0; var2 < 4; ++var2) {
        Block *var3 = blocks[var2];
        ItemStack *var4 = ingots[var2];

        {
            ItemStack *r = ItemStack_new(var3->blockID, 1, 0);
            void *a[] = {"###", "###", "###", (void *)(uintptr_t)'#', (void *)var4};
            CraftingManager_addRecipe(var1, r, a, 5);
        }

        {
            void *a[] = {"#", (void *)(uintptr_t)'#', (void *)var3};
            CraftingManager_addRecipe(var1, var4, a, 3);
        }
    }
}
