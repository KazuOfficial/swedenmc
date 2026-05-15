#ifndef NBT_TAG_BYTE_ARRAY_H
#define NBT_TAG_BYTE_ARRAY_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    uint8_t *byteArray;
    int32_t length;
} NBTTagByteArray;
NBTTagByteArray *NBTTagByteArray_create(void);
NBTTagByteArray *NBTTagByteArray_createWithValue(uint8_t *data, int32_t len);
char *NBTTagByteArray_toString(NBTTagByteArray *self);
#endif
