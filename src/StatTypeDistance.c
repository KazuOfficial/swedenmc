#include "StatTypeDistance.h"
#include "StatBase.h"
#include <stdlib.h>
#include <stdio.h>

static const char *StatTypeDistance_func_27192_a(IStatType *self, int var1) {
    (void)self;

    static char buf[64];
    double var3 = (double)var1 / 100.0;
    double var5 = var3 / 1000.0;
    if (var5 > 0.5) {
        snprintf(buf, sizeof(buf), "%.2f km", var5);
    } else if (var3 > 0.5) {
        snprintf(buf, sizeof(buf), "%.2f m", var3);
    } else {
        snprintf(buf, sizeof(buf), "%d cm", var1);
    }
    return buf;
}

static const IStatTypeVtable s_vtable = {.func_27192_a = StatTypeDistance_func_27192_a};

IStatType *StatTypeDistance_create(void) {
    IStatType *self = (IStatType *)calloc(1, sizeof(IStatType));
    self->vtable = &s_vtable;
    return self;
}
