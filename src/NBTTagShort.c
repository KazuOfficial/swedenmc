#include "NBTTagShort.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void base_free(NBTBase *self) {
    free(self->key);
    free(self);
}

static void short_write(NBTBase *self, DataStream *out) {
    DataStream_writeShort(out, ((NBTTagShort *)self)->shortValue);
}
static void short_read(NBTBase *self, DataStream *in) {
    ((NBTTagShort *)self)->shortValue = DataStream_readShort(in);
}
static uint8_t short_type(NBTBase *self) {
    (void)self;
    return 2;
}
static const NBTBaseVtable SHORT_VT = {short_write, short_read, short_type, base_free};

NBTTagShort *NBTTagShort_create(void) {
    NBTTagShort *t = (NBTTagShort *)calloc(1, sizeof(NBTTagShort));
    t->base.vtable = &SHORT_VT;
    return t;
}
NBTTagShort *NBTTagShort_createWithValue(int16_t v) {
    NBTTagShort *t = NBTTagShort_create();
    t->shortValue = v;
    return t;
}
char *NBTTagShort_toString(NBTTagShort *self) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", (int)self->shortValue);
    return strdup(buf);
}
