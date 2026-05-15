#include "NBTTagDouble.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static void base_free(NBTBase *self) {
    free(self->key);
    free(self);
}

static void double_write(NBTBase *self, DataStream *out) {
    DataStream_writeDouble(out, ((NBTTagDouble *)self)->doubleValue);
}
static void double_read(NBTBase *self, DataStream *in) {
    ((NBTTagDouble *)self)->doubleValue = DataStream_readDouble(in);
}
static uint8_t double_type(NBTBase *self) {
    (void)self;
    return 6;
}
static const NBTBaseVtable DOUBLE_VT = {double_write, double_read, double_type, base_free};

NBTTagDouble *NBTTagDouble_create(void) {
    NBTTagDouble *t = (NBTTagDouble *)calloc(1, sizeof(NBTTagDouble));
    t->base.vtable = &DOUBLE_VT;
    return t;
}
NBTTagDouble *NBTTagDouble_createWithValue(double v) {
    NBTTagDouble *t = NBTTagDouble_create();
    t->doubleValue = v;
    return t;
}
static void java_double_str(char *buf, int size, double v) {
    if (isnan(v)) {
        snprintf(buf, size, "NaN");
        return;
    }
    if (isinf(v)) {
        snprintf(buf, size, v < 0.0 ? "-Infinity" : "Infinity");
        return;
    }
    const char *p;
    snprintf(buf, size, "%g", v);
    for (p = buf; *p; p++) {
        if (*p == '.' || *p == 'e' || *p == 'E')
            return;
    }
    strncat(buf, ".0", size - (int)strlen(buf) - 1);
}
char *NBTTagDouble_toString(NBTTagDouble *self) {
    char buf[32];
    java_double_str(buf, sizeof(buf), self->doubleValue);
    return strdup(buf);
}
