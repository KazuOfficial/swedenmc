#include "NBTTagEnd.h"
#include <stdlib.h>

static void base_free(NBTBase *self) {
    free(self->key);
    free(self);
}

static void end_write(NBTBase *self, DataStream *out) {
    (void)self;
    (void)out;
}
static void end_read(NBTBase *self, DataStream *in) {
    (void)self;
    (void)in;
}
static uint8_t end_type(NBTBase *self) {
    (void)self;
    return 0;
}
static const NBTBaseVtable END_VT = {end_write, end_read, end_type, base_free};

NBTTagEnd *NBTTagEnd_create(void) {
    NBTTagEnd *t = (NBTTagEnd *)calloc(1, sizeof(NBTTagEnd));
    t->base.vtable = &END_VT;
    return t;
}
const char *NBTTagEnd_toString(NBTTagEnd *self) {
    (void)self;
    return "END";
}
