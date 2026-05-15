#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include "StatBase.h"
#include "IStatStringFormat.h"
#include "ItemStack.h"

struct Item;
struct Block;

typedef struct Achievement {
    StatBase base;

    int displayColumn;

    int displayRow;

    struct Achievement *parentAchievement;

    char *achievementDescription;

    IStatStringFormat *statStringFormatter;

    ItemStack theItemStack;

    int isSpecial;
} Achievement;

Achievement *Achievement_create_item(int var1, const char *var2, int var3, int var4,
                                     struct Item *var5, Achievement *var6);

Achievement *Achievement_create_block(int var1, const char *var2, int var3, int var4,
                                      struct Block *var5, Achievement *var6);

Achievement *Achievement_create(int var1, const char *var2, int var3, int var4, ItemStack var5,
                                Achievement *var6);

Achievement *Achievement_func_27089_a(Achievement *self);

Achievement *Achievement_setSpecial(Achievement *self);

Achievement *Achievement_registerAchievement(Achievement *self);

int Achievement_func_25067_a(Achievement *self);

const char *Achievement_getDescription(Achievement *self);

Achievement *Achievement_setStatStringFormatter(Achievement *self, IStatStringFormat *var1);

int Achievement_getSpecial(Achievement *self);

void Achievement_destroy(Achievement *self);

#endif
