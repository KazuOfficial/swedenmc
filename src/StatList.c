#include "StatList.h"
#include "StatBase.h"
#include "StatBasic.h"
#include "StatCrafting.h"
#include "StatCollector.h"
#include "Block.h"
#include "Item.h"
#include "IRecipe.h"
#include "CraftingManager.h"
#include "FurnaceRecipes.h"
#include "AchievementList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

StatListMapEntry StatList_field_25169_C[STATLIST_MAP_SIZE];

static unsigned int sl_hash(int key) {
    return (unsigned int)((key * 0x9e3779b9) & (STATLIST_MAP_SIZE - 1));
}

int StatList_field_25169_C_containsKey(int key) {
    unsigned int h = sl_hash(key);
    for (int tries = 0; tries < STATLIST_MAP_SIZE; tries++) {
        if (!StatList_field_25169_C[h].used)
            return 0;
        if (StatList_field_25169_C[h].key == key)
            return 1;
        h = (h + 1) & (STATLIST_MAP_SIZE - 1);
    }
    return 0;
}

void StatList_field_25169_C_put(int key, StatBase *value) {
    unsigned int h = sl_hash(key);
    for (int tries = 0; tries < STATLIST_MAP_SIZE; tries++) {
        if (!StatList_field_25169_C[h].used || StatList_field_25169_C[h].key == key) {
            StatList_field_25169_C[h].used = 1;
            StatList_field_25169_C[h].key = key;
            StatList_field_25169_C[h].value = value;
            return;
        }
        h = (h + 1) & (STATLIST_MAP_SIZE - 1);
    }
}

StatBase *StatList_field_25169_C_get(int key) {
    unsigned int h = sl_hash(key);
    for (int tries = 0; tries < STATLIST_MAP_SIZE; tries++) {
        if (!StatList_field_25169_C[h].used)
            return NULL;
        if (StatList_field_25169_C[h].key == key)
            return StatList_field_25169_C[h].value;
        h = (h + 1) & (STATLIST_MAP_SIZE - 1);
    }
    return NULL;
}

StatBase **StatList_field_25188_a = NULL;
int StatList_field_25188_a_count = 0;
int StatList_field_25188_a_cap = 0;

StatBase **StatList_field_25187_b = NULL;
int StatList_field_25187_b_count = 0;
int StatList_field_25187_b_cap = 0;

StatCrafting **StatList_field_25186_c = NULL;
int StatList_field_25186_c_count = 0;
int StatList_field_25186_c_cap = 0;

StatCrafting **StatList_field_25185_d = NULL;
int StatList_field_25185_d_count = 0;
int StatList_field_25185_d_cap = 0;

#define LIST_ADD(arr, cnt, cap, val)                                                               \
    do {                                                                                           \
        if ((cnt) >= (cap)) {                                                                      \
            int nc = (cap) == 0 ? 64 : (cap) * 2;                                                  \
            (arr) = realloc((arr), (size_t)(nc) * sizeof(*(arr)));                                 \
            (cap) = nc;                                                                            \
        }                                                                                          \
        (arr)[(cnt)++] = (val);                                                                    \
    } while (0)

void StatList_field_25188_a_add(StatBase *stat) {
    LIST_ADD(StatList_field_25188_a, StatList_field_25188_a_count, StatList_field_25188_a_cap,
             stat);
}
void StatList_field_25187_b_add(StatBasic *stat) {
    LIST_ADD(StatList_field_25187_b, StatList_field_25187_b_count, StatList_field_25187_b_cap,
             (StatBase *)stat);
}
void StatList_field_25186_c_add(StatCrafting *stat) {
    LIST_ADD(StatList_field_25186_c, StatList_field_25186_c_count, StatList_field_25186_c_cap,
             stat);
}
void StatList_field_25185_d_add(StatCrafting *stat) {
    LIST_ADD(StatList_field_25185_d, StatList_field_25185_d_count, StatList_field_25185_d_cap,
             stat);
}

static void list_remove_base(StatBase **arr, int *cnt, StatBase *val) {
    for (int i = 0; i < *cnt; i++) {
        if (arr[i] == val) {
            memmove(&arr[i], &arr[i + 1], (size_t)(*cnt - i - 1) * sizeof(*arr));
            (*cnt)--;
            return;
        }
    }
}
void StatList_field_25188_a_remove(StatBase *stat) {
    list_remove_base(StatList_field_25188_a, &StatList_field_25188_a_count, stat);
}
void StatList_field_25187_b_remove(StatBase *stat) {
    list_remove_base(StatList_field_25187_b, &StatList_field_25187_b_count, stat);
}
void StatList_field_25185_d_remove(StatBase *stat) {
    list_remove_base((StatBase **)StatList_field_25185_d, &StatList_field_25185_d_count, stat);
}

StatBase *StatList_startGameStat = NULL;
StatBase *StatList_createWorldStat = NULL;
StatBase *StatList_loadWorldStat = NULL;
StatBase *StatList_joinMultiplayerStat = NULL;
StatBase *StatList_leaveGameStat = NULL;
StatBase *StatList_minutesPlayedStat = NULL;
StatBase *StatList_distanceWalkedStat = NULL;
StatBase *StatList_distanceSwumStat = NULL;
StatBase *StatList_distanceFallenStat = NULL;
StatBase *StatList_distanceClimbedStat = NULL;
StatBase *StatList_distanceFlownStat = NULL;
StatBase *StatList_distanceDoveStat = NULL;
StatBase *StatList_distanceByMinecartStat = NULL;
StatBase *StatList_distanceByBoatStat = NULL;
StatBase *StatList_distanceByPigStat = NULL;
StatBase *StatList_jumpStat = NULL;
StatBase *StatList_dropStat = NULL;
StatBase *StatList_damageDealtStat = NULL;
StatBase *StatList_damageTakenStat = NULL;
StatBase *StatList_deathsStat = NULL;
StatBase *StatList_mobKillsStat = NULL;
StatBase *StatList_playerKillsStat = NULL;
StatBase *StatList_fishCaughtStat = NULL;

StatBase *StatList_mineBlockStatArray[256];

StatBase **StatList_field_25158_z = NULL;
StatBase **StatList_field_25172_A = NULL;
StatBase **StatList_field_25170_B = NULL;

static int s_field_25166_D = 0;
static int s_field_25164_E = 0;

static void replaceSimilarBlocks(StatBase **var0, int var1, int var2) {
    if (var0[var1] != NULL && var0[var2] == NULL) {
        var0[var2] = var0[var1];
    } else {
        StatList_field_25188_a_remove(var0[var1]);
        StatList_field_25185_d_remove(var0[var1]);
        StatList_field_25187_b_remove(var0[var1]);
        var0[var1] = var0[var2];
    }
}

static void replaceAllSimilarBlocks(StatBase **var0) {
    replaceSimilarBlocks(var0, Block_waterStill->blockID, Block_waterMoving->blockID);
    replaceSimilarBlocks(var0, Block_lavaStill->blockID, Block_lavaStill->blockID);
    replaceSimilarBlocks(var0, Block_pumpkinLantern->blockID, Block_pumpkin->blockID);
    replaceSimilarBlocks(var0, Block_stoneOvenActive->blockID, Block_stoneOvenIdle->blockID);
    replaceSimilarBlocks(var0, Block_oreRedstoneGlowing->blockID, Block_oreRedstone->blockID);
    replaceSimilarBlocks(var0, Block_redstoneRepeaterActive->blockID,
                         Block_redstoneRepeaterIdle->blockID);
    replaceSimilarBlocks(var0, Block_torchRedstoneActive->blockID,
                         Block_torchRedstoneIdle->blockID);
    replaceSimilarBlocks(var0, Block_mushroomRed->blockID, Block_mushroomBrown->blockID);
    replaceSimilarBlocks(var0, Block_stairDouble->blockID, Block_stairSingle->blockID);
    replaceSimilarBlocks(var0, Block_grass->blockID, Block_dirt->blockID);
    replaceSimilarBlocks(var0, Block_tilledField->blockID, Block_dirt->blockID);
}

static void func_25153_a(StatBase **var2, const char *var0, int var1) {
    memset(var2, 0, 256 * sizeof(StatBase *));
    for (int var3 = 0; var3 < 256; var3++) {
        if (Block_blocksList[var3] != NULL &&
            Block_blocksList[var3]->vtable->getEnableStats(Block_blocksList[var3])) {

            const char *blockName =
                Block_blocksList[var3]->vtable->translateBlockName(Block_blocksList[var3]);
            const char *var4 = StatCollector_translateToLocalFormatted(var0, blockName);
            StatCrafting *sc = StatCrafting_create(var1 + var3, var4, var3);
            var2[var3] = &sc->base;
            var2[var3]->vtable->registerStat(var2[var3]);
            StatList_field_25185_d_add(sc);
        }
    }
    replaceAllSimilarBlocks(var2);
}

static void func_25155_a(StatBase **var0, const char *var1, int var2, int var3, int var4) {
    for (int var5 = var3; var5 < var4; var5++) {
        if (Item_itemsList[var5] != NULL) {
            const char *itemName = Item_itemsList[var5]->vtable->getItemName(Item_itemsList[var5]);

            char nameBuf[512];
            snprintf(nameBuf, sizeof(nameBuf), "%s.name", itemName ? itemName : "");
            const char *statName = StatCollector_translateToLocal(nameBuf);
            const char *var6 = StatCollector_translateToLocalFormatted(var1, statName);
            StatCrafting *sc = StatCrafting_create(var2 + var5, var6, var5);
            var0[var5] = &sc->base;
            var0[var5]->vtable->registerStat(var0[var5]);

            if (var5 >= BLOCK_COUNT) {
                StatList_field_25186_c_add(sc);
            }
        }
    }
    replaceAllSimilarBlocks(var0);
}

static void func_25149_b(StatBase **var0, const char *var1, int var2, int var3, int var4) {
    for (int var5 = var3; var5 < var4; var5++) {
        if (Item_itemsList[var5] != NULL &&
            Item_itemsList[var5]->vtable->isDamagable(Item_itemsList[var5])) {
            const char *itemName = Item_itemsList[var5]->vtable->getItemName(Item_itemsList[var5]);
            char nameBuf[512];
            snprintf(nameBuf, sizeof(nameBuf), "%s.name", itemName ? itemName : "");
            const char *statName = StatCollector_translateToLocal(nameBuf);
            const char *var6 = StatCollector_translateToLocalFormatted(var1, statName);
            StatCrafting *sc = StatCrafting_create(var2 + var5, var6, var5);
            var0[var5] = &sc->base;
            var0[var5]->vtable->registerStat(var0[var5]);
        }
    }
    replaceAllSimilarBlocks(var0);
}

void StatList_func_27360_a(void) {}

void StatList_func_25154_a(void) {
    if (StatList_field_25172_A == NULL)
        StatList_field_25172_A = (StatBase **)calloc(ITEM_LIST_SIZE, sizeof(StatBase *));
    if (StatList_field_25170_B == NULL)
        StatList_field_25170_B = (StatBase **)calloc(ITEM_LIST_SIZE, sizeof(StatBase *));
    func_25155_a(StatList_field_25172_A, "stat.useItem", 16908288, 0, BLOCK_COUNT);
    func_25149_b(StatList_field_25170_B, "stat.breakItem", 16973824, 0, BLOCK_COUNT);
    s_field_25166_D = 1;
    StatList_func_25157_c();
}

void StatList_func_25151_b(void) {
    if (StatList_field_25172_A == NULL)
        StatList_field_25172_A = (StatBase **)calloc(ITEM_LIST_SIZE, sizeof(StatBase *));
    if (StatList_field_25170_B == NULL)
        StatList_field_25170_B = (StatBase **)calloc(ITEM_LIST_SIZE, sizeof(StatBase *));
    func_25155_a(StatList_field_25172_A, "stat.useItem", 16908288, BLOCK_COUNT, ITEM_LIST_SIZE);
    func_25149_b(StatList_field_25170_B, "stat.breakItem", 16973824, BLOCK_COUNT, ITEM_LIST_SIZE);
    s_field_25164_E = 1;
    StatList_func_25157_c();
}

void StatList_func_25157_c(void) {
    if (!s_field_25166_D || !s_field_25164_E)
        return;

    static int var0[ITEM_LIST_SIZE];
    memset(var0, 0, sizeof(var0));

    CraftingManager *cm = CraftingManager_getInstance();
    IRecipeList *recipes = CraftingManager_getRecipeList(cm);
    for (int i = 0; i < recipes->count; i++) {
        ItemStack *out = recipes->entries[i]->vtable->getRecipeOutput(recipes->entries[i]);
        if (out != NULL && out->itemID >= 0 && out->itemID < ITEM_LIST_SIZE) {
            var0[out->itemID] = 1;
        }
    }

    FurnaceRecipes *fr = FurnaceRecipes_smelting();
    int smeltCount = 0;
    SmeltingEntry *smeltList = FurnaceRecipes_getSmeltingList(fr, &smeltCount);
    for (int i = 0; i < smeltCount; i++) {
        int id = smeltList[i].value.itemID;
        if (id >= 0 && id < ITEM_LIST_SIZE)
            var0[id] = 1;
    }

    if (StatList_field_25158_z == NULL)
        StatList_field_25158_z = (StatBase **)calloc(ITEM_LIST_SIZE, sizeof(StatBase *));

    for (int id = 0; id < ITEM_LIST_SIZE; id++) {
        if (!var0[id])
            continue;
        if (Item_itemsList[id] == NULL)
            continue;

        const char *itemName = Item_itemsList[id]->vtable->getItemName(Item_itemsList[id]);
        char nameBuf[512];
        snprintf(nameBuf, sizeof(nameBuf), "%s.name", itemName ? itemName : "");
        const char *statName = StatCollector_translateToLocal(nameBuf);
        const char *var3 = StatCollector_translateToLocalFormatted("stat.craftItem", statName);
        StatCrafting *sc = StatCrafting_create(16842752 + id, var3, id);
        StatList_field_25158_z[id] = &sc->base;
        StatList_field_25158_z[id]->vtable->registerStat(StatList_field_25158_z[id]);
    }

    replaceAllSimilarBlocks(StatList_field_25158_z);
}

StatBase *StatList_func_27361_a(int var0) { return StatList_field_25169_C_get(var0); }

void StatList_init(void) {

#define MAKE2(field, id, key)                                                                      \
    field = &StatBasic_create2(id, StatCollector_translateToLocal(key))->base
#define MAKE3(field, id, key, type)                                                                \
    field = &StatBasic_create(id, StatCollector_translateToLocal(key), type)->base
#define H(field)                                                                                   \
    field->vtable->func_27082_h(field);                                                            \
    field->vtable->registerStat(field)
#define R(field) field->vtable->registerStat(field)

    MAKE2(StatList_startGameStat, 1000, "stat.startGame");
    H(StatList_startGameStat);
    MAKE2(StatList_createWorldStat, 1001, "stat.createWorld");
    H(StatList_createWorldStat);
    MAKE2(StatList_loadWorldStat, 1002, "stat.loadWorld");
    H(StatList_loadWorldStat);
    MAKE2(StatList_joinMultiplayerStat, 1003, "stat.joinMultiplayer");
    H(StatList_joinMultiplayerStat);
    MAKE2(StatList_leaveGameStat, 1004, "stat.leaveGame");
    H(StatList_leaveGameStat);
    MAKE3(StatList_minutesPlayedStat, 1100, "stat.playOneMinute", StatBase_field_27086_j);
    H(StatList_minutesPlayedStat);
    MAKE3(StatList_distanceWalkedStat, 2000, "stat.walkOneCm", StatBase_field_27085_k);
    H(StatList_distanceWalkedStat);
    MAKE3(StatList_distanceSwumStat, 2001, "stat.swimOneCm", StatBase_field_27085_k);
    H(StatList_distanceSwumStat);
    MAKE3(StatList_distanceFallenStat, 2002, "stat.fallOneCm", StatBase_field_27085_k);
    H(StatList_distanceFallenStat);
    MAKE3(StatList_distanceClimbedStat, 2003, "stat.climbOneCm", StatBase_field_27085_k);
    H(StatList_distanceClimbedStat);
    MAKE3(StatList_distanceFlownStat, 2004, "stat.flyOneCm", StatBase_field_27085_k);
    H(StatList_distanceFlownStat);
    MAKE3(StatList_distanceDoveStat, 2005, "stat.diveOneCm", StatBase_field_27085_k);
    H(StatList_distanceDoveStat);
    MAKE3(StatList_distanceByMinecartStat, 2006, "stat.minecartOneCm", StatBase_field_27085_k);
    H(StatList_distanceByMinecartStat);
    MAKE3(StatList_distanceByBoatStat, 2007, "stat.boatOneCm", StatBase_field_27085_k);
    H(StatList_distanceByBoatStat);
    MAKE3(StatList_distanceByPigStat, 2008, "stat.pigOneCm", StatBase_field_27085_k);
    H(StatList_distanceByPigStat);
    MAKE2(StatList_jumpStat, 2010, "stat.jump");
    H(StatList_jumpStat);
    MAKE2(StatList_dropStat, 2011, "stat.drop");
    H(StatList_dropStat);

    MAKE2(StatList_damageDealtStat, 2020, "stat.damageDealt");
    R(StatList_damageDealtStat);
    MAKE2(StatList_damageTakenStat, 2021, "stat.damageTaken");
    R(StatList_damageTakenStat);
    MAKE2(StatList_deathsStat, 2022, "stat.deaths");
    R(StatList_deathsStat);
    MAKE2(StatList_mobKillsStat, 2023, "stat.mobKills");
    R(StatList_mobKillsStat);
    MAKE2(StatList_playerKillsStat, 2024, "stat.playerKills");
    R(StatList_playerKillsStat);
    MAKE2(StatList_fishCaughtStat, 2025, "stat.fishCaught");
    R(StatList_fishCaughtStat);

#undef MAKE2
#undef MAKE3
#undef H
#undef R

    func_25153_a(StatList_mineBlockStatArray, "stat.mineBlock", 16777216);

    AchievementList_func_27374_a();
    s_field_25166_D = 0;
    s_field_25164_E = 0;
}
