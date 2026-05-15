#include "NBTTagByte.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void base_free(NBTBase *self) {
    free(self->key);
    free(self);
}

static void byte_write(NBTBase *self, DataStream *out) {
    DataStream_writeByte(out, (uint8_t)((NBTTagByte *)self)->byteValue);
}
static void byte_read(NBTBase *self, DataStream *in) {
    ((NBTTagByte *)self)->byteValue = (int8_t)DataStream_readByte(in);
}
static uint8_t byte_type(NBTBase *self) {
    (void)self;
    return 1;
}
static const NBTBaseVtable BYTE_VT = {byte_write, byte_read, byte_type, base_free};

NBTTagByte *NBTTagByte_create(void) {
    NBTTagByte *t = (NBTTagByte *)calloc(1, sizeof(NBTTagByte));
    t->base.vtable = &BYTE_VT;
    return t;
}
NBTTagByte *NBTTagByte_createWithValue(int8_t v) {
    NBTTagByte *t = NBTTagByte_create();
    t->byteValue = v;
    return t;
}
char *NBTTagByte_toString(NBTTagByte *self) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", (int)self->byteValue);
    return strdup(buf);
}
