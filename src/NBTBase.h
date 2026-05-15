#ifndef NBT_BASE_H
#define NBT_BASE_H

#include "DataStream.h"
#include <stdint.h>

struct NBTBase;
struct NBTTagCompound;
struct NBTTagList;

typedef struct {
    void (*writeTagContents)(struct NBTBase *self, DataStream *out);
    void (*readTagContents)(struct NBTBase *self, DataStream *in);
    uint8_t (*getType)(struct NBTBase *self);
    void (*free)(struct NBTBase *self);
} NBTBaseVtable;

typedef struct NBTBase {
    const NBTBaseVtable *vtable;
    char *key;
} NBTBase;

#define NBT_getType(tag) ((NBTBase *)(tag))->vtable->getType((NBTBase *)(tag))
#define NBT_writeTagContents(tag, s)                                                               \
    ((NBTBase *)(tag))->vtable->writeTagContents((NBTBase *)(tag), s)
#define NBT_readTagContents(tag, s) ((NBTBase *)(tag))->vtable->readTagContents((NBTBase *)(tag), s)
#define NBT_free(tag) ((NBTBase *)(tag))->vtable->free((NBTBase *)(tag))

const char *NBTBase_getKey(NBTBase *self);
NBTBase *NBTBase_setKey(NBTBase *self, const char *key);

NBTBase *NBTBase_readTag(DataStream *in);
void NBTBase_writeTag(NBTBase *tag, DataStream *out);
NBTBase *NBTBase_createTagOfType(uint8_t type);
const char *NBTBase_getTagName(uint8_t type);

#endif
