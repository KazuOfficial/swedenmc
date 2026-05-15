#ifndef NBT_TAG_DOUBLE_H
#define NBT_TAG_DOUBLE_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    double doubleValue;
} NBTTagDouble;
NBTTagDouble *NBTTagDouble_create(void);
NBTTagDouble *NBTTagDouble_createWithValue(double v);
char *NBTTagDouble_toString(NBTTagDouble *self);
#endif
