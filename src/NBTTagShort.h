#ifndef NBT_TAG_SHORT_H
#define NBT_TAG_SHORT_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    int16_t shortValue;
} NBTTagShort;
NBTTagShort *NBTTagShort_create(void);
NBTTagShort *NBTTagShort_createWithValue(int16_t v);
char *NBTTagShort_toString(NBTTagShort *self);
#endif
