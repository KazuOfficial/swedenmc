#include "RecipesWeapons.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include <stdint.h>

static const char *s_patterns[1][3] = {
    {"X", "X", "#"},
};

void RecipesWeapons_addRecipes(RecipesWeapons *self, CraftingManager *var1) {
    (void)self;

    void *materials[5] = {(void *)Block_planks, (void *)Block_cobblestone, (void *)Item_ingotIron,
                          (void *)Item_diamond, (void *)Item_ingotGold};
    Item *swords[5] = {Item_swordWood, Item_swordStone, Item_swordSteel, Item_swordDiamond,
                       Item_swordGold};

    for (int var2 = 0; var2 < 5; ++var2) {
        void *var3 = materials[var2];
        for (int var4 = 0; var4 < 1; ++var4) {
            Item *var5 = swords[var2];
            ItemStack *result = ItemStack_new(var5->shiftedIndex, 1, 0);

            void *args[] = {(void *)s_patterns[var4][0],
                            (void *)s_patterns[var4][1],
                            (void *)s_patterns[var4][2],
                            (void *)(uintptr_t)'#',
                            (void *)Item_stick,
                            (void *)(uintptr_t)'X',
                            var3};
            CraftingManager_addRecipe(var1, result, args, 7);
        }
    }

    {
        ItemStack *r = ItemStack_new(Item_bow->shiftedIndex, 1, 0);
        void *a[] = {" #X",
                     "# X",
                     " #X",
                     (void *)(uintptr_t)'X',
                     (void *)Item_silk,
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick};
        CraftingManager_addRecipe(var1, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Item_arrow->shiftedIndex, 4, 0);
        void *a[] = {"X",
                     "#",
                     "Y",
                     (void *)(uintptr_t)'Y',
                     (void *)Item_feather,
                     (void *)(uintptr_t)'X',
                     (void *)Item_flint,
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick};
        CraftingManager_addRecipe(var1, r, a, 9);
    }
}
