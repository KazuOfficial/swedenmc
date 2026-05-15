#include "StatTypeSimple.h"
#include "StatBase.h"
#include <stdlib.h>
#include <stdio.h>

static const char *StatTypeSimple_func_27192_a(IStatType *self, int var1) {
    (void)self;
    static char buf[64];
    long v = (long)var1;
    int neg = (v < 0);
    unsigned long uv = neg ? (unsigned long)(-v) : (unsigned long)v;

    char digits[32];
    int ndigits = 0;
    if (uv == 0) {
        digits[ndigits++] = '0';
    } else {
        while (uv > 0) {
            digits[ndigits++] = (char)('0' + uv % 10);
            uv /= 10;
        }
    }

    int out = 0;
    if (neg)
        buf[out++] = '-';
    for (int i = ndigits - 1; i >= 0; --i) {
        buf[out++] = digits[i];

        if (i > 0 && i % 3 == 0)
            buf[out++] = ',';
    }
    buf[out] = '\0';
    return buf;
}

static const IStatTypeVtable s_vtable = {.func_27192_a = StatTypeSimple_func_27192_a};

IStatType *StatTypeSimple_create(void) {
    IStatType *self = (IStatType *)calloc(1, sizeof(IStatType));
    self->vtable = &s_vtable;
    return self;
}
