#include "NBTTagLong.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void base_free(NBTBase *self) {
    free(self->key);
    free(self);
}

static void long_write(NBTBase *self, DataStream *out) {
    DataStream_writeLong(out, ((NBTTagLong *)self)->longValue);
}
static void long_read(NBTBase *self, DataStream *in) {
    ((NBTTagLong *)self)->longValue = DataStream_readLong(in);
}
static uint8_t long_type(NBTBase *self) {
    (void)self;
    return 4;
}
static const NBTBaseVtable LONG_VT = {long_write, long_read, long_type, base_free};

NBTTagLong *NBTTagLong_create(void) {
    NBTTagLong *t = (NBTTagLong *)calloc(1, sizeof(NBTTagLong));
    t->base.vtable = &LONG_VT;
    return t;
}
NBTTagLong *NBTTagLong_createWithValue(int64_t v) {
    NBTTagLong *t = NBTTagLong_create();
    t->longValue = v;
    return t;
}
char *NBTTagLong_toString(NBTTagLong *self) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%lld", (long long)self->longValue);
    return strdup(buf);
}
