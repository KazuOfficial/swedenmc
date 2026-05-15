#ifndef NBT_TAG_LIST_H
#define NBT_TAG_LIST_H
#include "NBTBase.h"
typedef struct {
    NBTBase base;
    NBTBase **tagList;
    uint8_t tagType;
} NBTTagList;
NBTTagList *NBTTagList_create(void);
void NBTTagList_setTag(NBTTagList *self, NBTBase *tag);
NBTBase *NBTTagList_tagAt(NBTTagList *self, int index);
int NBTTagList_tagCount(NBTTagList *self);
char *NBTTagList_toString(NBTTagList *self);
#endif
