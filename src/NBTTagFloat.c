#include "NBTTagFloat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static void base_free(NBTBase *self) {
    free(self->key);
    free(self);
}

static void float_write(NBTBase *self, DataStream *out) {
    DataStream_writeFloat(out, ((NBTTagFloat *)self)->floatValue);
}
static void float_read(NBTBase *self, DataStream *in) {
    ((NBTTagFloat *)self)->floatValue = DataStream_readFloat(in);
}
static uint8_t float_type(NBTBase *self) {
    (void)self;
    return 5;
}
static const NBTBaseVtable FLOAT_VT = {float_write, float_read, float_type, base_free};

NBTTagFloat *NBTTagFloat_create(void) {
    NBTTagFloat *t = (NBTTagFloat *)calloc(1, sizeof(NBTTagFloat));
    t->base.vtable = &FLOAT_VT;
    return t;
}
NBTTagFloat *NBTTagFloat_createWithValue(float v) {
    NBTTagFloat *t = NBTTagFloat_create();
    t->floatValue = v;
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
char *NBTTagFloat_toString(NBTTagFloat *self) {
    char buf[32];
    java_double_str(buf, sizeof(buf), (double)self->floatValue);
    return strdup(buf);
}
