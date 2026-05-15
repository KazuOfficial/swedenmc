#ifndef STAT_BASIC_H
#define STAT_BASIC_H

#include "StatBase.h"

typedef struct StatBasic {
    StatBase base;
} StatBasic;

StatBasic *StatBasic_create(int var1, const char *var2, IStatType *var3);

StatBasic *StatBasic_create2(int var1, const char *var2);

#endif
