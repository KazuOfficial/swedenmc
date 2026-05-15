#ifndef STAT_BASE_H
#define STAT_BASE_H

#include "IStatType.h"

struct StatBase;

typedef struct StatBaseVtable {

    struct StatBase *(*func_27082_h)(struct StatBase *self);

    struct StatBase *(*registerStat)(struct StatBase *self);

    int (*func_25067_a)(struct StatBase *self);

    const char *(*func_27084_a)(struct StatBase *self, int var1);
} StatBaseVtable;

typedef struct StatBase {
    const StatBaseVtable *vtable;

    int statId;

    char *statName;

    int field_27088_g;

    const char *statGuid;

    IStatType *field_26902_a;
} StatBase;

extern IStatType *StatBase_field_27087_i;

extern IStatType *StatBase_field_27086_j;

extern IStatType *StatBase_field_27085_k;

void StatBase_init(StatBase *self, int var1, const char *var2, IStatType *var3);

void StatBase_init2(StatBase *self, int var1, const char *var2);

StatBase *StatBase_func_27082_h(StatBase *self);

StatBase *StatBase_registerStat(StatBase *self);

int StatBase_func_25067_a(StatBase *self);

const char *StatBase_func_27084_a(StatBase *self, int var1);

const char *StatBase_toString(StatBase *self);

extern const StatBaseVtable StatBase_defaultVtable;

void StatBase_init_statics(void);

#endif
