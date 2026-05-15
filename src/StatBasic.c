#include "StatBasic.h"
#include "StatList.h"
#include <stdlib.h>

static StatBase *StatBasic_registerStat(StatBase *self) {

    StatBase_registerStat(self);

    StatList_field_25187_b_add((StatBasic *)self);
    return self;
}

static StatBaseVtable s_vtable;
static int s_vtableInited = 0;

static void initVtable(void) {
    if (s_vtableInited)
        return;
    s_vtable = StatBase_defaultVtable;
    s_vtable.registerStat = StatBasic_registerStat;
    s_vtableInited = 1;
}

StatBasic *StatBasic_create(int var1, const char *var2, IStatType *var3) {
    initVtable();
    StatBasic *self = (StatBasic *)calloc(1, sizeof(StatBasic));
    StatBase_init(&self->base, var1, var2, var3);
    self->base.vtable = &s_vtable;
    return self;
}

StatBasic *StatBasic_create2(int var1, const char *var2) {
    initVtable();
    StatBasic *self = (StatBasic *)calloc(1, sizeof(StatBasic));
    StatBase_init2(&self->base, var1, var2);
    self->base.vtable = &s_vtable;
    return self;
}
