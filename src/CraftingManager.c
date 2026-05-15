#include "CraftingManager.h"
#include "IRecipe.h"
#include "ShapedRecipes.h"
#include "ShapelessRecipes.h"
#include "RecipeSorter.h"
#include "RecipesTools.h"
#include "RecipesWeapons.h"
#include "RecipesIngots.h"
#include "RecipesFood.h"
#include "RecipesCrafting.h"
#include "RecipesArmor.h"
#include "RecipesDyes.h"
#include "JavaSort.h"
#include "Item.h"
#include "Block.h"
#include "InventoryCrafting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void CraftingManager_construct(CraftingManager *self);
void CraftingManager_addRecipe(CraftingManager *self, ItemStack *var1, void **var2, int var2_len);
void CraftingManager_addShapelessRecipe(CraftingManager *self, ItemStack *var1, void **var2,
                                        int var2_len);

static CraftingManager s_instance;
static int s_init = 0;

CraftingManager *CraftingManager_getInstance(void) {
    if (!s_init) {
        s_init = 1;
        CraftingManager_construct(&s_instance);
    }
    return &s_instance;
}

static void IRecipeList_add(IRecipeList *list, struct IRecipe *entry) {
    if (list->count == list->capacity) {
        int newcap = list->capacity == 0 ? 8 : list->capacity * 2;
        list->entries = realloc(list->entries, newcap * sizeof(*list->entries));
        list->capacity = newcap;
    }
    list->entries[list->count++] = entry;
}

static int arg_is_block(void *p) {
    if ((uintptr_t)p <= 127)
        return 0;
    Block *b = (Block *)p;
    int id = b->blockID;
    if (id < 1 || id >= BLOCK_COUNT)
        return 0;
    return Block_blocksList[id] == b;
}

static int arg_is_item(void *p) {
    if ((uintptr_t)p <= 127)
        return 0;
    Item *it = (Item *)p;
    int idx = it->shiftedIndex;
    if (idx < 0 || idx >= ITEM_LIST_SIZE)
        return 0;
    return Item_itemsList[idx] == it;
}

static int arg_is_string(void *p) {
    if ((uintptr_t)p <= 127)
        return 0;
    unsigned char first = *(unsigned char *)p;
    return first >= 0x20 && first <= 0x7E;
}

void CraftingManager_addRecipe(CraftingManager *self, ItemStack *var1, void **var2, int var2_len) {

    char var3[128];
    int var3_len = 0;
    int var4 = 0;
    int var5 = 0;
    int var6 = 0;

    while (var4 < var2_len && arg_is_string(var2[var4])) {
        const char *var7 = (const char *)var2[var4++];
        ++var6;
        var5 = (int)strlen(var7);

        memcpy(var3 + var3_len, var7, var5);
        var3_len += var5;
    }
    var3[var3_len] = '\0';

    ItemStack *var12[128];
    memset(var12, 0, sizeof(var12));

    for (; var4 < var2_len; var4 += 2) {
        int var13 = (int)(uintptr_t)var2[var4];
        if (var13 < 0 || var13 >= 128)
            continue;
        ItemStack *var15 = NULL;
        if (var4 + 1 < var2_len) {
            void *obj = var2[var4 + 1];
            if (arg_is_item(obj)) {

                Item *it = (Item *)obj;
                var15 = ItemStack_new(it->shiftedIndex, 1, 0);
            } else if (arg_is_block(obj)) {

                Block *b = (Block *)obj;
                var15 = ItemStack_new(b->blockID, 1, -1);
            } else {

                var15 = ItemStack_copy_ptr((ItemStack *)obj);
            }
        }
        var12[var13] = var15;
    }

    int n = var5 * var6;
    ItemStack **var14 = (ItemStack **)calloc(n, sizeof(ItemStack *));
    for (int var16 = 0; var16 < n; ++var16) {
        unsigned char var10 = (unsigned char)var3[var16];
        if (var10 < 128 && var12[var10] != NULL) {
            var14[var16] = ItemStack_copy_ptr(var12[var10]);
        } else {
            var14[var16] = NULL;
        }
    }

    IRecipeList_add(&self->recipes, (IRecipe *)ShapedRecipes_create(var5, var6, var14, n, var1));

    for (int c = 0; c < 128; c++)
        ItemStack_free(var12[c]);
    free(var1);
}

void CraftingManager_addShapelessRecipe(CraftingManager *self, ItemStack *var1, void **var2,
                                        int var2_len) {
    ItemStack *var3_items = (ItemStack *)malloc(var2_len * sizeof(ItemStack));
    int var3_count = 0;
    void **var4 = var2;
    int var5 = var2_len;
    for (int var6 = 0; var6 < var5; ++var6) {
        void *var7 = var4[var6];
        if (!arg_is_item(var7) && !arg_is_block(var7)) {

            ItemStack copy = ItemStack_copy((ItemStack *)var7);
            var3_items[var3_count++] = copy;
        } else if (arg_is_item(var7)) {

            Item *it = (Item *)var7;
            ItemStack is = ItemStack_fromItemID(it->shiftedIndex, 1, 0);
            var3_items[var3_count++] = is;
        } else {

            Block *b = (Block *)var7;
            ItemStack is = ItemStack_fromBlockID(b->blockID, 1, 0);
            var3_items[var3_count++] = is;
        }
    }
    IRecipeList_add(&self->recipes,
                    (IRecipe *)ShapelessRecipes_create(var1, var3_items, var3_count));
    free(var1);

    for (int var6 = 0; var6 < var5; ++var6) {
        void *var7 = var4[var6];
        if (!arg_is_item(var7) && !arg_is_block(var7))
            ItemStack_free((ItemStack *)var7);
    }
}

static CraftingManager *s_sort_self = NULL;

static int recipe_cmp(const void *a, const void *b) {
    IRecipe *ra = *(IRecipe **)a;
    IRecipe *rb = *(IRecipe **)b;
    RecipeSorter rs;
    rs.craftingManager = s_sort_self;
    return RecipeSorter_compareRecipes(&rs, ra, rb);
}

static void CraftingManager_construct(CraftingManager *self) {
    memset(&self->recipes, 0, sizeof(self->recipes));

    {
        RecipesTools t;
        RecipesTools_addRecipes(&t, self);
    }
    {
        RecipesWeapons w;
        RecipesWeapons_addRecipes(&w, self);
    }
    {
        RecipesIngots i;
        RecipesIngots_addRecipes(&i, self);
    }
    {
        RecipesFood f;
        RecipesFood_addRecipes(&f, self);
    }
    {
        RecipesCrafting c;
        RecipesCrafting_addRecipes(&c, self);
    }
    {
        RecipesArmor a;
        RecipesArmor_addRecipes(&a, self);
    }
    {
        RecipesDyes d;
        RecipesDyes_addRecipes(&d, self);
    }

    {
        ItemStack *r = ItemStack_new(Item_paper->shiftedIndex, 3, 0);
        void *a[] = {"###", (void *)(uintptr_t)'#', (void *)Item_reed};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Item_book->shiftedIndex, 1, 0);
        void *a[] = {"#", "#", "#", (void *)(uintptr_t)'#', (void *)Item_paper};
        CraftingManager_addRecipe(self, r, a, 5);
    }

    {
        ItemStack *r = ItemStack_new(Block_fence->blockID, 2, 0);
        void *a[] = {"###", "###", (void *)(uintptr_t)'#', (void *)Item_stick};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_jukebox->blockID, 1, 0);
        void *a[] = {"###",
                     "#X#",
                     "###",
                     (void *)(uintptr_t)'#',
                     (void *)Block_planks,
                     (void *)(uintptr_t)'X',
                     (void *)Item_diamond};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Block_musicBlock->blockID, 1, 0);
        void *a[] = {"###",
                     "#X#",
                     "###",
                     (void *)(uintptr_t)'#',
                     (void *)Block_planks,
                     (void *)(uintptr_t)'X',
                     (void *)Item_redstone};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Block_bookShelf->blockID, 1, 0);
        void *a[] = {"###",
                     "XXX",
                     "###",
                     (void *)(uintptr_t)'#',
                     (void *)Block_planks,
                     (void *)(uintptr_t)'X',
                     (void *)Item_book};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Block_blockSnow->blockID, 1, 0);
        void *a[] = {"##", "##", (void *)(uintptr_t)'#', (void *)Item_snowball};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_blockClay->blockID, 1, 0);
        void *a[] = {"##", "##", (void *)(uintptr_t)'#', (void *)Item_clay};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_brick->blockID, 1, 0);
        void *a[] = {"##", "##", (void *)(uintptr_t)'#', (void *)Item_brick};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_glowStone->blockID, 1, 0);
        void *a[] = {"##", "##", (void *)(uintptr_t)'#', (void *)Item_lightStoneDust};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_cloth->blockID, 1, 0);
        void *a[] = {"##", "##", (void *)(uintptr_t)'#', (void *)Item_silk};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_tnt->blockID, 1, 0);
        void *a[] = {"X#X",
                     "#X#",
                     "X#X",
                     (void *)(uintptr_t)'X',
                     (void *)Item_gunpowder,
                     (void *)(uintptr_t)'#',
                     (void *)Block_sand};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Block_stairSingle->blockID, 3, 3);
        void *a[] = {"###", (void *)(uintptr_t)'#', (void *)Block_cobblestone};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Block_stairSingle->blockID, 3, 0);
        void *a[] = {"###", (void *)(uintptr_t)'#', (void *)Block_stone};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Block_stairSingle->blockID, 3, 1);
        void *a[] = {"###", (void *)(uintptr_t)'#', (void *)Block_sandStone};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Block_stairSingle->blockID, 3, 2);
        void *a[] = {"###", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Block_ladder->blockID, 2, 0);
        void *a[] = {"# #", "###", "# #", (void *)(uintptr_t)'#', (void *)Item_stick};
        CraftingManager_addRecipe(self, r, a, 5);
    }

    {
        ItemStack *r = ItemStack_new(Item_doorWood->shiftedIndex, 1, 0);
        void *a[] = {"##", "##", "##", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 5);
    }

    {
        ItemStack *r = ItemStack_new(Block_trapdoor->blockID, 2, 0);
        void *a[] = {"###", "###", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Item_doorSteel->shiftedIndex, 1, 0);
        void *a[] = {"##", "##", "##", (void *)(uintptr_t)'#', (void *)Item_ingotIron};
        CraftingManager_addRecipe(self, r, a, 5);
    }

    {
        ItemStack *r = ItemStack_new(Item_sign->shiftedIndex, 1, 0);
        void *a[] = {"###",
                     "###",
                     " X ",
                     (void *)(uintptr_t)'#',
                     (void *)Block_planks,
                     (void *)(uintptr_t)'X',
                     (void *)Item_stick};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Item_cake->shiftedIndex, 1, 0);
        void *a[] = {"AAA",
                     "BEB",
                     "CCC",
                     (void *)(uintptr_t)'A',
                     (void *)Item_bucketMilk,
                     (void *)(uintptr_t)'B',
                     (void *)Item_sugar,
                     (void *)(uintptr_t)'C',
                     (void *)Item_wheat,
                     (void *)(uintptr_t)'E',
                     (void *)Item_egg};
        CraftingManager_addRecipe(self, r, a, 11);
    }

    {
        ItemStack *r = ItemStack_new(Item_sugar->shiftedIndex, 1, 0);
        void *a[] = {"#", (void *)(uintptr_t)'#', (void *)Item_reed};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Block_planks->blockID, 4, 0);
        void *a[] = {"#", (void *)(uintptr_t)'#', (void *)Block_wood};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Item_stick->shiftedIndex, 4, 0);
        void *a[] = {"#", "#", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_torchWood->blockID, 4, 0);
        void *a[] = {"X",
                     "#",
                     (void *)(uintptr_t)'X',
                     (void *)Item_coal,
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    {
        ItemStack *r = ItemStack_new(Block_torchWood->blockID, 4, 0);
        ItemStack *charcoal = ItemStack_new(Item_coal->shiftedIndex, 1, 1);
        void *a[] = {"X",
                     "#",
                     (void *)(uintptr_t)'X',
                     (void *)charcoal,
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick};
        CraftingManager_addRecipe(self, r, a, 6);
        ItemStack_free(charcoal);
    }

    {
        ItemStack *r = ItemStack_new(Item_bowlEmpty->shiftedIndex, 4, 0);
        void *a[] = {"# #", " # ", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_rail->blockID, 16, 0);
        void *a[] = {"X X",
                     "X#X",
                     "X X",
                     (void *)(uintptr_t)'X',
                     (void *)Item_ingotIron,
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Block_railPowered->blockID, 6, 0);
        void *a[] = {"X X",
                     "X#X",
                     "XRX",
                     (void *)(uintptr_t)'X',
                     (void *)Item_ingotGold,
                     (void *)(uintptr_t)'R',
                     (void *)Item_redstone,
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick};
        CraftingManager_addRecipe(self, r, a, 9);
    }

    {
        ItemStack *r = ItemStack_new(Block_railDetector->blockID, 6, 0);
        void *a[] = {"X X",
                     "X#X",
                     "XRX",
                     (void *)(uintptr_t)'X',
                     (void *)Item_ingotIron,
                     (void *)(uintptr_t)'R',
                     (void *)Item_redstone,
                     (void *)(uintptr_t)'#',
                     (void *)Block_pressurePlateStone};
        CraftingManager_addRecipe(self, r, a, 9);
    }

    {
        ItemStack *r = ItemStack_new(Item_minecartEmpty->shiftedIndex, 1, 0);
        void *a[] = {"# #", "###", (void *)(uintptr_t)'#', (void *)Item_ingotIron};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_pumpkinLantern->blockID, 1, 0);
        void *a[] = {"A",
                     "B",
                     (void *)(uintptr_t)'A',
                     (void *)Block_pumpkin,
                     (void *)(uintptr_t)'B',
                     (void *)Block_torchWood};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    {
        ItemStack *r = ItemStack_new(Item_minecartCrate->shiftedIndex, 1, 0);
        void *a[] = {"A",
                     "B",
                     (void *)(uintptr_t)'A',
                     (void *)Block_chest,
                     (void *)(uintptr_t)'B',
                     (void *)Item_minecartEmpty};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    {
        ItemStack *r = ItemStack_new(Item_minecartPowered->shiftedIndex, 1, 0);
        void *a[] = {"A",
                     "B",
                     (void *)(uintptr_t)'A',
                     (void *)Block_stoneOvenIdle,
                     (void *)(uintptr_t)'B',
                     (void *)Item_minecartEmpty};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    {
        ItemStack *r = ItemStack_new(Item_boat->shiftedIndex, 1, 0);
        void *a[] = {"# #", "###", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Item_bucketEmpty->shiftedIndex, 1, 0);
        void *a[] = {"# #", " # ", (void *)(uintptr_t)'#', (void *)Item_ingotIron};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Item_flintAndSteel->shiftedIndex, 1, 0);
        void *a[] = {"A ",
                     " B",
                     (void *)(uintptr_t)'A',
                     (void *)Item_ingotIron,
                     (void *)(uintptr_t)'B',
                     (void *)Item_flint};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    {
        ItemStack *r = ItemStack_new(Item_bread->shiftedIndex, 1, 0);
        void *a[] = {"###", (void *)(uintptr_t)'#', (void *)Item_wheat};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Block_stairCompactPlanks->blockID, 4, 0);
        void *a[] = {"#  ", "## ", "###", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 5);
    }

    {
        ItemStack *r = ItemStack_new(Item_fishingRod->shiftedIndex, 1, 0);
        void *a[] = {"  #",
                     " #X",
                     "# X",
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick,
                     (void *)(uintptr_t)'X',
                     (void *)Item_silk};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Block_stairCompactCobblestone->blockID, 4, 0);
        void *a[] = {"#  ", "## ", "###", (void *)(uintptr_t)'#', (void *)Block_cobblestone};
        CraftingManager_addRecipe(self, r, a, 5);
    }

    {
        ItemStack *r = ItemStack_new(Item_painting->shiftedIndex, 1, 0);
        void *a[] = {"###",
                     "#X#",
                     "###",
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick,
                     (void *)(uintptr_t)'X',
                     (void *)Block_cloth};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Item_appleGold->shiftedIndex, 1, 0);
        void *a[] = {"###",
                     "#X#",
                     "###",
                     (void *)(uintptr_t)'#',
                     (void *)Block_blockGold,
                     (void *)(uintptr_t)'X',
                     (void *)Item_appleRed};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Block_lever->blockID, 1, 0);
        void *a[] = {"X",
                     "#",
                     (void *)(uintptr_t)'#',
                     (void *)Block_cobblestone,
                     (void *)(uintptr_t)'X',
                     (void *)Item_stick};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    {
        ItemStack *r = ItemStack_new(Block_torchRedstoneActive->blockID, 1, 0);
        void *a[] = {"X",
                     "#",
                     (void *)(uintptr_t)'#',
                     (void *)Item_stick,
                     (void *)(uintptr_t)'X',
                     (void *)Item_redstone};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    {
        ItemStack *r = ItemStack_new(Item_redstoneRepeater->shiftedIndex, 1, 0);
        void *a[] = {"#X#",
                     "III",
                     (void *)(uintptr_t)'#',
                     (void *)Block_torchRedstoneActive,
                     (void *)(uintptr_t)'X',
                     (void *)Item_redstone,
                     (void *)(uintptr_t)'I',
                     (void *)Block_stone};
        CraftingManager_addRecipe(self, r, a, 8);
    }

    {
        ItemStack *r = ItemStack_new(Item_pocketSundial->shiftedIndex, 1, 0);
        void *a[] = {" # ",
                     "#X#",
                     " # ",
                     (void *)(uintptr_t)'#',
                     (void *)Item_ingotGold,
                     (void *)(uintptr_t)'X',
                     (void *)Item_redstone};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Item_compass->shiftedIndex, 1, 0);
        void *a[] = {" # ",
                     "#X#",
                     " # ",
                     (void *)(uintptr_t)'#',
                     (void *)Item_ingotIron,
                     (void *)(uintptr_t)'X',
                     (void *)Item_redstone};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Item_mapItem->shiftedIndex, 1, 0);
        void *a[] = {"###",
                     "#X#",
                     "###",
                     (void *)(uintptr_t)'#',
                     (void *)Item_paper,
                     (void *)(uintptr_t)'X',
                     (void *)Item_compass};
        CraftingManager_addRecipe(self, r, a, 7);
    }

    {
        ItemStack *r = ItemStack_new(Block_button->blockID, 1, 0);
        void *a[] = {"#", "#", (void *)(uintptr_t)'#', (void *)Block_stone};
        CraftingManager_addRecipe(self, r, a, 4);
    }

    {
        ItemStack *r = ItemStack_new(Block_pressurePlateStone->blockID, 1, 0);
        void *a[] = {"##", (void *)(uintptr_t)'#', (void *)Block_stone};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Block_pressurePlatePlanks->blockID, 1, 0);
        void *a[] = {"##", (void *)(uintptr_t)'#', (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 3);
    }

    {
        ItemStack *r = ItemStack_new(Block_dispenser->blockID, 1, 0);
        void *a[] = {"###",
                     "#X#",
                     "#R#",
                     (void *)(uintptr_t)'#',
                     (void *)Block_cobblestone,
                     (void *)(uintptr_t)'X',
                     (void *)Item_bow,
                     (void *)(uintptr_t)'R',
                     (void *)Item_redstone};
        CraftingManager_addRecipe(self, r, a, 9);
    }

    {
        ItemStack *r = ItemStack_new(Block_pistonBase->blockID, 1, 0);
        void *a[] = {"TTT",
                     "#X#",
                     "#R#",
                     (void *)(uintptr_t)'#',
                     (void *)Block_cobblestone,
                     (void *)(uintptr_t)'X',
                     (void *)Item_ingotIron,
                     (void *)(uintptr_t)'R',
                     (void *)Item_redstone,
                     (void *)(uintptr_t)'T',
                     (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 11);
    }

    {
        ItemStack *r = ItemStack_new(Block_pistonStickyBase->blockID, 1, 0);
        void *a[] = {"S",
                     "P",
                     (void *)(uintptr_t)'S',
                     (void *)Item_slimeBall,
                     (void *)(uintptr_t)'P',
                     (void *)Block_pistonBase};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    {
        ItemStack *r = ItemStack_new(Item_bed->shiftedIndex, 1, 0);
        void *a[] = {"###",
                     "XXX",
                     (void *)(uintptr_t)'#',
                     (void *)Block_cloth,
                     (void *)(uintptr_t)'X',
                     (void *)Block_planks};
        CraftingManager_addRecipe(self, r, a, 6);
    }

    s_sort_self = self;
    Arrays_sort((void **)self->recipes.entries, self->recipes.count, recipe_cmp);
    s_sort_self = NULL;

    printf("%d recipes\n", self->recipes.count);
}

ItemStack *CraftingManager_findMatchingRecipe(CraftingManager *self,
                                              struct InventoryCrafting *var1) {
    int var2;
    for (var2 = 0; var2 < self->recipes.count; ++var2) {
        IRecipe *var3 = self->recipes.entries[var2];
        if (var3->vtable->matches(var3, var1)) {
            return var3->vtable->getCraftingResult(var3, var1);
        }
    }
    return NULL;
}

IRecipeList *CraftingManager_getRecipeList(CraftingManager *self) { return &self->recipes; }

void CraftingManager_destroy(CraftingManager *self) {
    for (int i = 0; i < self->recipes.count; i++) {
        IRecipe *r = self->recipes.entries[i];
        r->vtable->destroy(r);
    }
    free(self->recipes.entries);
    self->recipes.entries = NULL;
    self->recipes.count = 0;
    self->recipes.capacity = 0;
}
