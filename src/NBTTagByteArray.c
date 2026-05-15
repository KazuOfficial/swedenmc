#include "NBTTagByteArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void bytearray_write(NBTBase *self, DataStream *out) {
    NBTTagByteArray *t = (NBTTagByteArray *)self;
    DataStream_writeInt(out, t->length);
    DataStream_writeBytes(out, t->byteArray, (size_t)t->length);
}
static void bytearray_read(NBTBase *self, DataStream *in) {
    NBTTagByteArray *t = (NBTTagByteArray *)self;
    t->length = DataStream_readInt(in);
    free(t->byteArray);
    t->byteArray = (uint8_t *)malloc((size_t)t->length);
    DataStream_readBytes(in, t->byteArray, (size_t)t->length);
}
static uint8_t bytearray_type(NBTBase *self) {
    (void)self;
    return 7;
}
static void bytearray_free(NBTBase *self) {
    free(((NBTTagByteArray *)self)->byteArray);
    free(self->key);
    free(self);
}
static const NBTBaseVtable BYTEARRAY_VT = {bytearray_write, bytearray_read, bytearray_type,
                                           bytearray_free};

NBTTagByteArray *NBTTagByteArray_create(void) {
    NBTTagByteArray *t = (NBTTagByteArray *)calloc(1, sizeof(NBTTagByteArray));
    t->base.vtable = &BYTEARRAY_VT;
    return t;
}
NBTTagByteArray *NBTTagByteArray_createWithValue(uint8_t *data, int32_t len) {
    NBTTagByteArray *t = NBTTagByteArray_create();
    t->byteArray = data;
    t->length = len;
    return t;
}
char *NBTTagByteArray_toString(NBTTagByteArray *self) {
    char buf[32];
    snprintf(buf, sizeof(buf), "[%d bytes]", self->length);
    return strdup(buf);
}
