#include "RecipesArmor.h"
#include "Item.h"
#include "Block.h"
#include "ItemStack.h"
#include <stdint.h>

static const char *s_patterns[4][3] = {
    {"XXX", "X X", NULL},
    {"X X", "XXX", "XXX"},
    {"XXX", "X X", "X X"},
    {"X X", "X X", NULL},
};

void RecipesArmor_addRecipes(RecipesArmor *self, CraftingManager *var1) {
    (void)self;

    void *materials[5] = {(void *)Item_leather, (void *)Block_fire, (void *)Item_ingotIron,
                          (void *)Item_diamond, (void *)Item_ingotGold};

    Item *outputs[4][5] = {
        {Item_helmetLeather, Item_helmetChain, Item_helmetSteel, Item_helmetDiamond,
         Item_helmetGold},
        {Item_plateLeather, Item_plateChain, Item_plateSteel, Item_plateDiamond, Item_plateGold},
        {Item_legsLeather, Item_legsChain, Item_legsSteel, Item_legsDiamond, Item_legsGold},
        {Item_bootsLeather, Item_bootsChain, Item_bootsSteel, Item_bootsDiamond, Item_bootsGold},
    };

    for (int var2 = 0; var2 < 5; ++var2) {
        void *var3 = materials[var2];
        for (int var4 = 0; var4 < 4; ++var4) {
            Item *var5 = outputs[var4][var2];
            ItemStack *result = ItemStack_new(var5->shiftedIndex, 1, 0);

            if (s_patterns[var4][2] != NULL) {
                void *args[] = {(void *)s_patterns[var4][0], (void *)s_patterns[var4][1],
                                (void *)s_patterns[var4][2], (void *)(uintptr_t)'X', var3};
                CraftingManager_addRecipe(var1, result, args, 5);
            } else {
                void *args[] = {(void *)s_patterns[var4][0], (void *)s_patterns[var4][1],
                                (void *)(uintptr_t)'X', var3};
                CraftingManager_addRecipe(var1, result, args, 4);
            }
        }
    }
}
