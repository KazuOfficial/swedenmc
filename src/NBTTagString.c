#include "NBTTagString.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static void string_write(NBTBase *self, DataStream *out) {
    DataStream_writeUTF(out, ((NBTTagString *)self)->stringValue);
}
static void string_read(NBTBase *self, DataStream *in) {
    NBTTagString *t = (NBTTagString *)self;
    free(t->stringValue);
    t->stringValue = DataStream_readUTF(in);
}
static uint8_t string_type(NBTBase *self) {
    (void)self;
    return 8;
}
static void string_free(NBTBase *self) {
    free(((NBTTagString *)self)->stringValue);
    free(self->key);
    free(self);
}
static const NBTBaseVtable STRING_VT = {string_write, string_read, string_type, string_free};

NBTTagString *NBTTagString_create(void) {
    NBTTagString *t = (NBTTagString *)calloc(1, sizeof(NBTTagString));
    t->base.vtable = &STRING_VT;
    return t;
}
NBTTagString *NBTTagString_createWithValue(const char *v) {
    assert(v != NULL);
    NBTTagString *t = NBTTagString_create();
    t->stringValue = strdup(v);
    return t;
}
char *NBTTagString_toString(NBTTagString *self) {
    return strdup(self->stringValue ? self->stringValue : "null");
}
