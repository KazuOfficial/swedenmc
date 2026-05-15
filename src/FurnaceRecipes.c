#include "FurnaceRecipes.h"
#include "Block.h"
#include "Item.h"
#include <stdlib.h>
#include <string.h>

static FurnaceRecipes s_smeltingBase;
static int s_smeltingBase_init = 0;

static void FurnaceRecipes_init(FurnaceRecipes *self);

FurnaceRecipes *FurnaceRecipes_smelting(void) {
    if (!s_smeltingBase_init) {
        s_smeltingBase_init = 1;
        FurnaceRecipes_init(&s_smeltingBase);
    }
    return &s_smeltingBase;
}

void FurnaceRecipes_addSmelting(FurnaceRecipes *self, int var1, ItemStack var2) {

    for (int i = 0; i < self->smeltingCount; ++i) {
        if (self->smeltingList[i].key == var1) {
            self->smeltingList[i].value = var2;
            return;
        }
    }
    if (self->smeltingCount >= self->smeltingCap) {
        int nc = self->smeltingCap ? self->smeltingCap * 2 : 16;
        self->smeltingList =
            (SmeltingEntry *)realloc(self->smeltingList, nc * sizeof(SmeltingEntry));
        self->smeltingCap = nc;
    }
    self->smeltingList[self->smeltingCount].key = var1;
    self->smeltingList[self->smeltingCount].value = var2;
    ++self->smeltingCount;
}

ItemStack *FurnaceRecipes_getSmeltingResult(FurnaceRecipes *self, int var1) {
    for (int i = 0; i < self->smeltingCount; ++i) {
        if (self->smeltingList[i].key == var1) {
            return &self->smeltingList[i].value;
        }
    }
    return NULL;
}

SmeltingEntry *FurnaceRecipes_getSmeltingList(FurnaceRecipes *self, int *outCount) {
    if (outCount)
        *outCount = self->smeltingCount;
    return self->smeltingList;
}

static void FurnaceRecipes_init(FurnaceRecipes *self) {
    memset(self, 0, sizeof(*self));

    FurnaceRecipes_addSmelting(self, Block_oreIron->blockID,
                               ItemStack_fromItemID(Item_ingotIron->shiftedIndex, 1, 0));
    FurnaceRecipes_addSmelting(self, Block_oreGold->blockID,
                               ItemStack_fromItemID(Item_ingotGold->shiftedIndex, 1, 0));
    FurnaceRecipes_addSmelting(self, Block_oreDiamond->blockID,
                               ItemStack_fromItemID(Item_diamond->shiftedIndex, 1, 0));

    FurnaceRecipes_addSmelting(self, Block_sand->blockID,
                               ItemStack_fromBlockID(Block_glass->blockID, 1, 0));
    FurnaceRecipes_addSmelting(self, Item_porkRaw->shiftedIndex,
                               ItemStack_fromItemID(Item_porkCooked->shiftedIndex, 1, 0));
    FurnaceRecipes_addSmelting(self, Item_fishRaw->shiftedIndex,
                               ItemStack_fromItemID(Item_fishCooked->shiftedIndex, 1, 0));

    FurnaceRecipes_addSmelting(self, Block_cobblestone->blockID,
                               ItemStack_fromBlockID(Block_stone->blockID, 1, 0));
    FurnaceRecipes_addSmelting(self, Item_clay->shiftedIndex,
                               ItemStack_fromItemID(Item_brick->shiftedIndex, 1, 0));

    FurnaceRecipes_addSmelting(self, Block_cactus->blockID,
                               ItemStack_fromItemID(Item_dyePowder->shiftedIndex, 1, 2));

    FurnaceRecipes_addSmelting(self, Block_wood->blockID,
                               ItemStack_fromItemID(Item_coal->shiftedIndex, 1, 1));
}
