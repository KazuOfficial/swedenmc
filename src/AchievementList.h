#ifndef ACHIEVEMENT_LIST_H
#define ACHIEVEMENT_LIST_H

#include "Achievement.h"

extern int AchievementList_minDisplayColumn;

extern int AchievementList_minDisplayRow;

extern int AchievementList_maxDisplayColumn;

extern int AchievementList_maxDisplayRow;

extern Achievement **AchievementList_achievementList;
extern int AchievementList_achievementList_count;

extern Achievement *AchievementList_openInventory;

extern Achievement *AchievementList_mineWood;

extern Achievement *AchievementList_buildWorkBench;

extern Achievement *AchievementList_buildPickaxe;

extern Achievement *AchievementList_buildFurnace;

extern Achievement *AchievementList_acquireIron;

extern Achievement *AchievementList_buildHoe;

extern Achievement *AchievementList_makeBread;

extern Achievement *AchievementList_bakeCake;

extern Achievement *AchievementList_buildBetterPickaxe;

extern Achievement *AchievementList_cookFish;

extern Achievement *AchievementList_onARail;

extern Achievement *AchievementList_buildSword;

extern Achievement *AchievementList_killEnemy;

extern Achievement *AchievementList_killCow;

extern Achievement *AchievementList_flyPig;

void AchievementList_func_27374_a(void);

void AchievementList_achievementList_add(Achievement *a);

void AchievementList_init(void);

#endif
