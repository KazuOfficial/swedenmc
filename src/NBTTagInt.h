#ifndef NBT_TAG_INT_H
#define NBT_TAG_INT_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    int32_t intValue;
} NBTTagInt;
NBTTagInt *NBTTagInt_create(void);
NBTTagInt *NBTTagInt_createWithValue(int32_t v);
char *NBTTagInt_toString(NBTTagInt *self);
#endif
