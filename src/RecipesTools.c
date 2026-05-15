#include "RecipesTools.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"
#include <stdint.h>

static const char *s_patterns[4][3] = {
    {"XXX", " # ", " # "},
    {"X", "#", "#"},
    {"XX", "X#", " #"},
    {"XX", " #", " #"},
};

void RecipesTools_addRecipes(RecipesTools *self, CraftingManager *var1) {
    (void)self;

    void *materials[5] = {(void *)Block_planks, (void *)Block_cobblestone, (void *)Item_ingotIron,
                          (void *)Item_diamond, (void *)Item_ingotGold};

    Item *outputs[4][5] = {
        {Item_pickaxeWood, Item_pickaxeStone, Item_pickaxeSteel, Item_pickaxeDiamond,
         Item_pickaxeGold},
        {Item_shovelWood, Item_shovelStone, Item_shovelSteel, Item_shovelDiamond, Item_shovelGold},
        {Item_axeWood, Item_axeStone, Item_axeSteel, Item_axeDiamond, Item_axeGold},
        {Item_hoeWood, Item_hoeStone, Item_hoeSteel, Item_hoeDiamond, Item_hoeGold},
    };

    for (int var2 = 0; var2 < 5; ++var2) {
        void *var3 = materials[var2];
        for (int var4 = 0; var4 < 4; ++var4) {
            Item *var5 = outputs[var4][var2];
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
        ItemStack *r = ItemStack_new(Item_shears->shiftedIndex, 1, 0);
        void *a[] = {" #", "# ", (void *)(uintptr_t)'#', (void *)Item_ingotIron};
        CraftingManager_addRecipe(var1, r, a, 4);
    }
}
