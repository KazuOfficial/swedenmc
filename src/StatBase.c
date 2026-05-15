#include "StatBase.h"
#include "StatList.h"
#include "StatTypeSimple.h"
#include "StatTypeTime.h"
#include "StatTypeDistance.h"
#include "AchievementMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IStatType *StatBase_field_27087_i = NULL;

IStatType *StatBase_field_27086_j = NULL;

IStatType *StatBase_field_27085_k = NULL;

void StatBase_init_statics(void) {
    if (!StatBase_field_27087_i)
        StatBase_field_27087_i = StatTypeSimple_create();
    if (!StatBase_field_27086_j)
        StatBase_field_27086_j = StatTypeTime_create();
    if (!StatBase_field_27085_k)
        StatBase_field_27085_k = StatTypeDistance_create();
}

StatBase *StatBase_func_27082_h(StatBase *self) {
    self->field_27088_g = 1;
    return self;
}

StatBase *StatBase_registerStat(StatBase *self) {
    if (StatList_field_25169_C_containsKey(self->statId)) {
        StatBase *existing = StatList_field_25169_C_get(self->statId);
        fprintf(stderr, "Duplicate stat id: \"%s\" and \"%s\" at id %d\n",
                existing ? existing->statName : "", self->statName, self->statId);
        abort();
    }
    StatList_field_25188_a_add(self);
    StatList_field_25169_C_put(self->statId, self);
    self->statGuid = AchievementMap_getGuid(self->statId);
    return self;
}

int StatBase_func_25067_a(StatBase *self) {
    (void)self;
    return 0;
}

const char *StatBase_func_27084_a(StatBase *self, int var1) {
    return self->field_26902_a->vtable->func_27192_a(self->field_26902_a, var1);
}

const char *StatBase_toString(StatBase *self) { return self->statName; }

const StatBaseVtable StatBase_defaultVtable = {
    .func_27082_h = StatBase_func_27082_h,
    .registerStat = StatBase_registerStat,
    .func_25067_a = StatBase_func_25067_a,
    .func_27084_a = StatBase_func_27084_a,
};

void StatBase_init(StatBase *self, int var1, const char *var2, IStatType *var3) {
    self->vtable = &StatBase_defaultVtable;
    self->field_27088_g = 0;
    self->statId = var1;
    self->statName = var2 ? strdup(var2) : NULL;
    self->statGuid = NULL;
    self->field_26902_a = var3;
}

void StatBase_init2(StatBase *self, int var1, const char *var2) {
    StatBase_init(self, var1, var2, StatBase_field_27087_i);
}
