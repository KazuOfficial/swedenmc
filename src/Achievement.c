#include "Achievement.h"
#include "AchievementList.h"
#include "StatCollector.h"
#include "Block.h"
#include "Item.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Achievement *Achievement_func_27089_a(Achievement *self) {
    self->base.field_27088_g = 1;
    return self;
}

static StatBase *Achievement_func_27082_h(StatBase *base) {
    return &Achievement_func_27089_a((Achievement *)base)->base;
}

Achievement *Achievement_registerAchievement(Achievement *self) {
    StatBase_registerStat(&self->base);

    AchievementList_achievementList_add(self);
    return self;
}

static StatBase *Achievement_registerStat(StatBase *base) {
    return &Achievement_registerAchievement((Achievement *)base)->base;
}

int Achievement_func_25067_a(Achievement *self) {
    (void)self;
    return 1;
}

static int Achievement_func_25067_a_vt(StatBase *base) {
    return Achievement_func_25067_a((Achievement *)base);
}

static const StatBaseVtable Achievement_vtable = {
    .func_27082_h = Achievement_func_27082_h,
    .registerStat = Achievement_registerStat,
    .func_25067_a = Achievement_func_25067_a_vt,
    .func_27084_a = StatBase_func_27084_a,
};

Achievement *Achievement_create(int var1, const char *var2, int var3, int var4, ItemStack var5,
                                Achievement *var6) {
    Achievement *self = (Achievement *)calloc(1, sizeof(Achievement));

    char nameKey[256];
    snprintf(nameKey, sizeof(nameKey), "achievement.%s", var2);
    StatBase_init2(&self->base, 5242880 + var1, StatCollector_translateToLocal(nameKey));
    self->base.vtable = &Achievement_vtable;

    self->theItemStack = var5;

    char descKey[256];
    snprintf(descKey, sizeof(descKey), "achievement.%s.desc", var2);
    self->achievementDescription = strdup(StatCollector_translateToLocal(descKey));

    self->displayColumn = var3;
    self->displayRow = var4;

    if (var3 < AchievementList_minDisplayColumn)
        AchievementList_minDisplayColumn = var3;
    if (var4 < AchievementList_minDisplayRow)
        AchievementList_minDisplayRow = var4;
    if (var3 > AchievementList_maxDisplayColumn)
        AchievementList_maxDisplayColumn = var3;
    if (var4 > AchievementList_maxDisplayRow)
        AchievementList_maxDisplayRow = var4;

    self->parentAchievement = var6;

    return self;
}

Achievement *Achievement_create_item(int var1, const char *var2, int var3, int var4, Item *var5,
                                     Achievement *var6) {
    ItemStack stack = ItemStack_fromItemID(var5->shiftedIndex, 1, 0);
    return Achievement_create(var1, var2, var3, var4, stack, var6);
}

Achievement *Achievement_create_block(int var1, const char *var2, int var3, int var4, Block *var5,
                                      Achievement *var6) {
    ItemStack stack = ItemStack_fromBlockID(var5->blockID, 1, 0);
    return Achievement_create(var1, var2, var3, var4, stack, var6);
}

Achievement *Achievement_setSpecial(Achievement *self) {
    self->isSpecial = 1;
    return self;
}

const char *Achievement_getDescription(Achievement *self) {
    if (self->statStringFormatter != NULL) {
        return self->statStringFormatter->vtable->formatString(self->statStringFormatter,
                                                               self->achievementDescription);
    }
    return self->achievementDescription;
}

Achievement *Achievement_setStatStringFormatter(Achievement *self, IStatStringFormat *var1) {
    self->statStringFormatter = var1;
    return self;
}

int Achievement_getSpecial(Achievement *self) { return self->isSpecial; }

void Achievement_destroy(Achievement *self) {
    free(self->base.statName);

    free(self->achievementDescription);
    free(self);
}
