#ifndef NBT_TAG_FLOAT_H
#define NBT_TAG_FLOAT_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    float floatValue;
} NBTTagFloat;
NBTTagFloat *NBTTagFloat_create(void);
NBTTagFloat *NBTTagFloat_createWithValue(float v);
char *NBTTagFloat_toString(NBTTagFloat *self);
#endif
