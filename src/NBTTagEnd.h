#ifndef NBT_TAG_END_H
#define NBT_TAG_END_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
} NBTTagEnd;
NBTTagEnd *NBTTagEnd_create(void);
const char *NBTTagEnd_toString(NBTTagEnd *self);
#endif
