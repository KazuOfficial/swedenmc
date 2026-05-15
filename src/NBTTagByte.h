#ifndef NBT_TAG_BYTE_H
#define NBT_TAG_BYTE_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    int8_t byteValue;
} NBTTagByte;
NBTTagByte *NBTTagByte_create(void);
NBTTagByte *NBTTagByte_createWithValue(int8_t v);
char *NBTTagByte_toString(NBTTagByte *self);
#endif
