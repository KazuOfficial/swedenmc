#ifndef STAT_CRAFTING_H
#define STAT_CRAFTING_H

#include "StatBase.h"

typedef struct StatCrafting {
    StatBase base;

    int field_25073_a;
} StatCrafting;

StatCrafting *StatCrafting_create(int var1, const char *var2, int var3);

int StatCrafting_func_25072_b(StatCrafting *self);

#endif
