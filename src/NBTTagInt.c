#include "NBTTagInt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void base_free(NBTBase *self) {
    free(self->key);
    free(self);
}

static void int_write(NBTBase *self, DataStream *out) {
    DataStream_writeInt(out, ((NBTTagInt *)self)->intValue);
}
static void int_read(NBTBase *self, DataStream *in) {
    ((NBTTagInt *)self)->intValue = DataStream_readInt(in);
}
static uint8_t int_type(NBTBase *self) {
    (void)self;
    return 3;
}
static const NBTBaseVtable INT_VT = {int_write, int_read, int_type, base_free};

NBTTagInt *NBTTagInt_create(void) {
    NBTTagInt *t = (NBTTagInt *)calloc(1, sizeof(NBTTagInt));
    t->base.vtable = &INT_VT;
    return t;
}
NBTTagInt *NBTTagInt_createWithValue(int32_t v) {
    NBTTagInt *t = NBTTagInt_create();
    t->intValue = v;
    return t;
}
char *NBTTagInt_toString(NBTTagInt *self) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", self->intValue);
    return strdup(buf);
}
