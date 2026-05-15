#include "AchievementList.h"
#include "Item.h"
#include "Block.h"
#include <stdio.h>
#include <stdlib.h>

int AchievementList_minDisplayColumn = 0;

int AchievementList_minDisplayRow = 0;

int AchievementList_maxDisplayColumn = 0;

int AchievementList_maxDisplayRow = 0;

Achievement **AchievementList_achievementList = NULL;
int AchievementList_achievementList_count = 0;
static int s_achievementList_cap = 0;

Achievement *AchievementList_openInventory = NULL;
Achievement *AchievementList_mineWood = NULL;
Achievement *AchievementList_buildWorkBench = NULL;
Achievement *AchievementList_buildPickaxe = NULL;
Achievement *AchievementList_buildFurnace = NULL;
Achievement *AchievementList_acquireIron = NULL;
Achievement *AchievementList_buildHoe = NULL;
Achievement *AchievementList_makeBread = NULL;
Achievement *AchievementList_bakeCake = NULL;
Achievement *AchievementList_buildBetterPickaxe = NULL;
Achievement *AchievementList_cookFish = NULL;
Achievement *AchievementList_onARail = NULL;
Achievement *AchievementList_buildSword = NULL;
Achievement *AchievementList_killEnemy = NULL;
Achievement *AchievementList_killCow = NULL;
Achievement *AchievementList_flyPig = NULL;

void AchievementList_achievementList_add(Achievement *a) {
    if (AchievementList_achievementList_count == s_achievementList_cap) {
        int newcap = s_achievementList_cap == 0 ? 16 : s_achievementList_cap * 2;
        AchievementList_achievementList = (Achievement **)realloc(AchievementList_achievementList,
                                                                  newcap * sizeof(Achievement *));
        s_achievementList_cap = newcap;
    }
    AchievementList_achievementList[AchievementList_achievementList_count++] = a;
}

void AchievementList_func_27374_a(void) {}

void AchievementList_init(void) {
    static int s_init = 0;
    if (s_init)
        return;
    s_init = 1;

    AchievementList_openInventory = Achievement_registerAchievement(Achievement_func_27089_a(
        Achievement_create_item(0, "openInventory", 0, 0, Item_book, NULL)));

    AchievementList_mineWood = Achievement_registerAchievement(
        Achievement_create_block(1, "mineWood", 2, 1, Block_wood, AchievementList_openInventory));

    AchievementList_buildWorkBench = Achievement_registerAchievement(Achievement_create_block(
        2, "buildWorkBench", 4, -1, Block_workbench, AchievementList_mineWood));

    AchievementList_buildPickaxe = Achievement_registerAchievement(Achievement_create_item(
        3, "buildPickaxe", 4, 2, Item_pickaxeWood, AchievementList_buildWorkBench));

    AchievementList_buildFurnace = Achievement_registerAchievement(Achievement_create_block(
        4, "buildFurnace", 3, 4, Block_stoneOvenActive, AchievementList_buildPickaxe));

    AchievementList_acquireIron = Achievement_registerAchievement(Achievement_create_item(
        5, "acquireIron", 1, 4, Item_ingotIron, AchievementList_buildFurnace));

    AchievementList_buildHoe = Achievement_registerAchievement(Achievement_create_item(
        6, "buildHoe", 2, -3, Item_hoeWood, AchievementList_buildWorkBench));

    AchievementList_makeBread = Achievement_registerAchievement(
        Achievement_create_item(7, "makeBread", -1, -3, Item_bread, AchievementList_buildHoe));

    AchievementList_bakeCake = Achievement_registerAchievement(
        Achievement_create_item(8, "bakeCake", 0, -5, Item_cake, AchievementList_buildHoe));

    AchievementList_buildBetterPickaxe = Achievement_registerAchievement(Achievement_create_item(
        9, "buildBetterPickaxe", 6, 2, Item_pickaxeStone, AchievementList_buildPickaxe));

    AchievementList_cookFish = Achievement_registerAchievement(Achievement_create_item(
        10, "cookFish", 2, 6, Item_fishCooked, AchievementList_buildFurnace));

    AchievementList_onARail = Achievement_registerAchievement(Achievement_setSpecial(
        Achievement_create_block(11, "onARail", 2, 3, Block_rail, AchievementList_acquireIron)));

    AchievementList_buildSword = Achievement_registerAchievement(Achievement_create_item(
        12, "buildSword", 6, -1, Item_swordWood, AchievementList_buildWorkBench));

    AchievementList_killEnemy = Achievement_registerAchievement(
        Achievement_create_item(13, "killEnemy", 8, -1, Item_bone, AchievementList_buildSword));

    AchievementList_killCow = Achievement_registerAchievement(
        Achievement_create_item(14, "killCow", 7, -3, Item_leather, AchievementList_buildSword));

    AchievementList_flyPig = Achievement_registerAchievement(Achievement_setSpecial(
        Achievement_create_item(15, "flyPig", 8, -4, Item_saddle, AchievementList_killCow)));

    printf("%d achievements\n", AchievementList_achievementList_count);
}
