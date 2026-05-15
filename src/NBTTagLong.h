#ifndef NBT_TAG_LONG_H
#define NBT_TAG_LONG_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    int64_t longValue;
} NBTTagLong;
NBTTagLong *NBTTagLong_create(void);
NBTTagLong *NBTTagLong_createWithValue(int64_t v);
char *NBTTagLong_toString(NBTTagLong *self);
#endif
