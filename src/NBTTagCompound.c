#include "NBTTagCompound.h"
#include "NBTTagByte.h"
#include <stdio.h>
#include "NBTTagShort.h"
#include "NBTTagInt.h"
#include "NBTTagLong.h"
#include "NBTTagFloat.h"
#include "NBTTagDouble.h"
#include "NBTTagString.h"
#include "NBTTagByteArray.h"
#include <stdlib.h>
#include <string.h>

#include "stb_ds.h"

static void compound_write(NBTBase *self, DataStream *out) {
    NBTTagCompound *t = (NBTTagCompound *)self;
    int i, n = (int)shlen(t->tagMap);
    for (i = 0; i < n; i++)
        NBTBase_writeTag(t->tagMap[i].value, out);
    DataStream_writeByte(out, 0);
}

static void compound_read(NBTBase *self, DataStream *in) {
    NBTTagCompound *t = (NBTTagCompound *)self;

    int i, n = (int)shlen(t->tagMap);
    for (i = 0; i < n; i++)
        NBT_free(t->tagMap[i].value);
    shfree(t->tagMap);
    t->tagMap = NULL;

    while (1) {
        NBTBase *tag = NBTBase_readTag(in);
        if (NBT_getType(tag) == 0) {
            NBT_free(tag);
            return;
        }
        int existing = shgeti(t->tagMap, tag->key);
        if (existing >= 0) {
            NBTBase *old = t->tagMap[existing].value;
            shdel(t->tagMap, tag->key);
            NBT_free(old);
        }
        shput(t->tagMap, tag->key, tag);
    }
}

static uint8_t compound_type(NBTBase *self) {
    (void)self;
    return 10;
}

static void compound_free(NBTBase *self) {
    NBTTagCompound *t = (NBTTagCompound *)self;
    int i, n = (int)shlen(t->tagMap);
    for (i = 0; i < n; i++)
        NBT_free(t->tagMap[i].value);
    shfree(t->tagMap);
    free(self->key);
    free(self);
}

static const NBTBaseVtable COMPOUND_VT = {compound_write, compound_read, compound_type,
                                          compound_free};

NBTTagCompound *NBTTagCompound_create(void) {
    NBTTagCompound *t = (NBTTagCompound *)calloc(1, sizeof(NBTTagCompound));
    t->base.vtable = &COMPOUND_VT;
    return t;
}

void NBTTagCompound_setTag(NBTTagCompound *self, const char *key, NBTBase *tag) {
    NBTBase_setKey(tag, key);
    int existing = shgeti(self->tagMap, (char *)key);
    if (existing >= 0) {
        NBTBase *old = self->tagMap[existing].value;
        shdel(self->tagMap, (char *)key);
        NBT_free(old);
    }
    shput(self->tagMap, (char *)key, tag);
}
void NBTTagCompound_setByte(NBTTagCompound *self, const char *key, int8_t v) {
    NBTTagCompound_setTag(self, key, (NBTBase *)NBTTagByte_createWithValue(v));
}
void NBTTagCompound_setShort(NBTTagCompound *self, const char *key, int16_t v) {
    NBTTagCompound_setTag(self, key, (NBTBase *)NBTTagShort_createWithValue(v));
}
void NBTTagCompound_setInteger(NBTTagCompound *self, const char *key, int32_t v) {
    NBTTagCompound_setTag(self, key, (NBTBase *)NBTTagInt_createWithValue(v));
}
void NBTTagCompound_setLong(NBTTagCompound *self, const char *key, int64_t v) {
    NBTTagCompound_setTag(self, key, (NBTBase *)NBTTagLong_createWithValue(v));
}
void NBTTagCompound_setFloat(NBTTagCompound *self, const char *key, float v) {
    NBTTagCompound_setTag(self, key, (NBTBase *)NBTTagFloat_createWithValue(v));
}
void NBTTagCompound_setDouble(NBTTagCompound *self, const char *key, double v) {
    NBTTagCompound_setTag(self, key, (NBTBase *)NBTTagDouble_createWithValue(v));
}
void NBTTagCompound_setString(NBTTagCompound *self, const char *key, const char *v) {
    NBTTagCompound_setTag(self, key, (NBTBase *)NBTTagString_createWithValue(v));
}
void NBTTagCompound_setByteArray(NBTTagCompound *self, const char *key, uint8_t *data,
                                 int32_t len) {
    uint8_t *copy = (uint8_t *)malloc((size_t)len);
    memcpy(copy, data, (size_t)len);
    NBTTagCompound_setTag(self, key, (NBTBase *)NBTTagByteArray_createWithValue(copy, len));
}
void NBTTagCompound_setCompoundTag(NBTTagCompound *self, const char *key, NBTTagCompound *v) {
    NBTTagCompound_setTag(self, key, (NBTBase *)v);
}
void NBTTagCompound_setBoolean(NBTTagCompound *self, const char *key, int v) {
    NBTTagCompound_setByte(self, key, (int8_t)(v ? 1 : 0));
}

NBTEntry *NBTTagCompound_func_28110_c(NBTTagCompound *self) { return self->tagMap; }

int NBTTagCompound_hasKey(NBTTagCompound *self, const char *key) {
    return shgeti(self->tagMap, (char *)key) >= 0;
}
int8_t NBTTagCompound_getByte(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? 0 : ((NBTTagByte *)self->tagMap[i].value)->byteValue;
}
int16_t NBTTagCompound_getShort(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? 0 : ((NBTTagShort *)self->tagMap[i].value)->shortValue;
}
int32_t NBTTagCompound_getInteger(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? 0 : ((NBTTagInt *)self->tagMap[i].value)->intValue;
}
int64_t NBTTagCompound_getLong(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? 0 : ((NBTTagLong *)self->tagMap[i].value)->longValue;
}
float NBTTagCompound_getFloat(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? 0.0f : ((NBTTagFloat *)self->tagMap[i].value)->floatValue;
}
double NBTTagCompound_getDouble(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? 0.0 : ((NBTTagDouble *)self->tagMap[i].value)->doubleValue;
}
const char *NBTTagCompound_getString(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? "" : ((NBTTagString *)self->tagMap[i].value)->stringValue;
}
uint8_t *NBTTagCompound_getByteArray(NBTTagCompound *self, const char *key) {
    static uint8_t empty[1] = {0};
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? empty : ((NBTTagByteArray *)self->tagMap[i].value)->byteArray;
}
NBTTagCompound *NBTTagCompound_getCompoundTag(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? NBTTagCompound_create() : (NBTTagCompound *)self->tagMap[i].value;
}
NBTTagList *NBTTagCompound_getTagList(NBTTagCompound *self, const char *key) {
    int i = shgeti(self->tagMap, (char *)key);
    return i < 0 ? NBTTagList_create() : (NBTTagList *)self->tagMap[i].value;
}
int NBTTagCompound_getBoolean(NBTTagCompound *self, const char *key) {
    return NBTTagCompound_getByte(self, key) != 0;
}
char *NBTTagCompound_toString(NBTTagCompound *self) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%d entries", (int)shlen(self->tagMap));
    return strdup(buf);
}
