#include "StatTypeTime.h"
#include "StatBase.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char *StatTypeTime_func_27192_a(IStatType *self, int var1) {
    (void)self;
    static char buf[64];
    double var2 = (double)var1 / 20.0;
    double var4 = var2 / 60.0;
    double var6 = var4 / 60.0;
    double var8 = var6 / 24.0;
    double var10 = var8 / 365.0;
    if (var10 > 0.5) {
        snprintf(buf, sizeof(buf), "%.2f y", var10);
    } else if (var8 > 0.5) {
        snprintf(buf, sizeof(buf), "%.2f d", var8);
    } else if (var6 > 0.5) {
        snprintf(buf, sizeof(buf), "%.2f h", var6);
    } else if (var4 > 0.5) {
        snprintf(buf, sizeof(buf), "%.2f m", var4);
    } else {

        char tmp[32];
        snprintf(tmp, sizeof(tmp), "%g", var2);

        if (!strchr(tmp, '.') && !strchr(tmp, 'e') && !strchr(tmp, 'E')) {
            snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), ".0");
        }
        snprintf(buf, sizeof(buf), "%s s", tmp);
    }
    return buf;
}

static const IStatTypeVtable s_vtable = {.func_27192_a = StatTypeTime_func_27192_a};

IStatType *StatTypeTime_create(void) {
    IStatType *self = (IStatType *)calloc(1, sizeof(IStatType));
    self->vtable = &s_vtable;
    return self;
}
