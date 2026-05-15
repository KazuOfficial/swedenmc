#include "RecipesDyes.h"
#include "Item.h"
#include "Block.h"
#include "BlockCloth.h"
#include "ItemStack.h"
#include <stdint.h>

void RecipesDyes_addRecipes(RecipesDyes *self, CraftingManager *var1) {
    (void)self;

    for (int var2 = 0; var2 < 16; ++var2) {
        ItemStack *r = ItemStack_new(Block_cloth->blockID, 1, BlockCloth_func_21035_d(var2));
        ItemStack *ing0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, var2);
        ItemStack *ing1 = ItemStack_new(Item_itemsList[Block_cloth->blockID]->shiftedIndex, 1, 0);
        void *a[] = {(void *)ing0, (void *)ing1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 11);
        void *a[] = {(void *)Block_plantYellow};
        CraftingManager_addShapelessRecipe(var1, r, a, 1);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 1);
        void *a[] = {(void *)Block_plantRed};
        CraftingManager_addShapelessRecipe(var1, r, a, 1);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 3, 15);
        void *a[] = {(void *)Item_bone};
        CraftingManager_addShapelessRecipe(var1, r, a, 1);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 9);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 1);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 15);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 14);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 1);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 11);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 10);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 2);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 15);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 8);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 0);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 15);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 7);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 8);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 15);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 3, 7);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 0);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 15);
        ItemStack *a2 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 15);
        void *a[] = {(void *)a0, (void *)a1, (void *)a2};
        CraftingManager_addShapelessRecipe(var1, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 12);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 4);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 15);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 6);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 4);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 2);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 5);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 4);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 1);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 2, 13);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 5);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 9);
        void *a[] = {(void *)a0, (void *)a1};
        CraftingManager_addShapelessRecipe(var1, r, a, 2);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 3, 13);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 4);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 1);
        ItemStack *a2 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 9);
        void *a[] = {(void *)a0, (void *)a1, (void *)a2};
        CraftingManager_addShapelessRecipe(var1, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Item_dyePowder->shiftedIndex, 4, 13);
        ItemStack *a0 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 4);
        ItemStack *a1 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 1);
        ItemStack *a2 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 1);
        ItemStack *a3 = ItemStack_new(Item_dyePowder->shiftedIndex, 1, 15);
        void *a[] = {(void *)a0, (void *)a1, (void *)a2, (void *)a3};
        CraftingManager_addShapelessRecipe(var1, r, a, 4);
    }
}
