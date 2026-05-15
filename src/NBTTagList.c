#include "NBTTagList.h"
#include "NBTBase.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

static void list_write(NBTBase *self, DataStream *out) {
    NBTTagList *t = (NBTTagList *)self;
    int count = (int)arrlen(t->tagList);
    t->tagType = count > 0 ? NBT_getType(t->tagList[0]) : 1;
    DataStream_writeByte(out, t->tagType);
    DataStream_writeInt(out, count);
    int i;
    for (i = 0; i < count; i++)
        NBT_writeTagContents(t->tagList[i], out);
}

static void list_read(NBTBase *self, DataStream *in) {
    NBTTagList *t = (NBTTagList *)self;
    t->tagType = DataStream_readByte(in);
    int32_t count = DataStream_readInt(in);
    int i;
    for (i = 0; i < (int)arrlen(t->tagList); i++)
        NBT_free(t->tagList[i]);
    arrfree(t->tagList);
    t->tagList = NULL;
    for (i = 0; i < count; i++) {
        NBTBase *tag = NBTBase_createTagOfType(t->tagType);
        NBT_readTagContents(tag, in);
        arrpush(t->tagList, tag);
    }
}

static uint8_t list_type(NBTBase *self) {
    (void)self;
    return 9;
}

static void list_free(NBTBase *self) {
    NBTTagList *t = (NBTTagList *)self;
    int i;
    for (i = 0; i < (int)arrlen(t->tagList); i++)
        NBT_free(t->tagList[i]);
    arrfree(t->tagList);
    free(self->key);
    free(self);
}

static const NBTBaseVtable LIST_VT = {list_write, list_read, list_type, list_free};

NBTTagList *NBTTagList_create(void) {
    NBTTagList *t = (NBTTagList *)calloc(1, sizeof(NBTTagList));
    t->base.vtable = &LIST_VT;
    return t;
}

void NBTTagList_setTag(NBTTagList *self, NBTBase *tag) {
    self->tagType = NBT_getType(tag);
    arrpush(self->tagList, tag);
}

NBTBase *NBTTagList_tagAt(NBTTagList *self, int index) { return self->tagList[index]; }

int NBTTagList_tagCount(NBTTagList *self) { return (int)arrlen(self->tagList); }
char *NBTTagList_toString(NBTTagList *self) {
    char buf[128];
    snprintf(buf, sizeof(buf), "%d entries of type %s", (int)arrlen(self->tagList),
             NBTBase_getTagName(self->tagType));
    return strdup(buf);
}
