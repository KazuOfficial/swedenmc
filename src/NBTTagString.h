#ifndef NBT_TAG_STRING_H
#define NBT_TAG_STRING_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    char *stringValue;
} NBTTagString;
NBTTagString *NBTTagString_create(void);
NBTTagString *NBTTagString_createWithValue(const char *v);
char *NBTTagString_toString(NBTTagString *self);
#endif
