#include "NBTBase.h"
#include "NBTTagEnd.h"
#include "NBTTagByte.h"
#include "NBTTagShort.h"
#include "NBTTagInt.h"
#include "NBTTagLong.h"
#include "NBTTagFloat.h"
#include "NBTTagDouble.h"
#include "NBTTagByteArray.h"
#include "NBTTagString.h"
#include "NBTTagList.h"
#include "NBTTagCompound.h"
#include <stdlib.h>
#include <string.h>

const char *NBTBase_getKey(NBTBase *self) { return self->key ? self->key : ""; }

NBTBase *NBTBase_setKey(NBTBase *self, const char *key) {
    free(self->key);
    self->key = key ? strdup(key) : NULL;
    return self;
}

NBTBase *NBTBase_readTag(DataStream *in) {
    uint8_t type = DataStream_readByte(in);
    if (type == 0)
        return (NBTBase *)NBTTagEnd_create();
    NBTBase *tag = NBTBase_createTagOfType(type);
    tag->key = DataStream_readUTF(in);
    NBT_readTagContents(tag, in);
    return tag;
}

void NBTBase_writeTag(NBTBase *tag, DataStream *out) {
    DataStream_writeByte(out, NBT_getType(tag));
    if (NBT_getType(tag) != 0) {
        DataStream_writeUTF(out, NBTBase_getKey(tag));
        NBT_writeTagContents(tag, out);
    }
}

NBTBase *NBTBase_createTagOfType(uint8_t type) {
    switch (type) {
    case 0:
        return (NBTBase *)NBTTagEnd_create();
    case 1:
        return (NBTBase *)NBTTagByte_create();
    case 2:
        return (NBTBase *)NBTTagShort_create();
    case 3:
        return (NBTBase *)NBTTagInt_create();
    case 4:
        return (NBTBase *)NBTTagLong_create();
    case 5:
        return (NBTBase *)NBTTagFloat_create();
    case 6:
        return (NBTBase *)NBTTagDouble_create();
    case 7:
        return (NBTBase *)NBTTagByteArray_create();
    case 8:
        return (NBTBase *)NBTTagString_create();
    case 9:
        return (NBTBase *)NBTTagList_create();
    case 10:
        return (NBTBase *)NBTTagCompound_create();
    default:
        return NULL;
    }
}

const char *NBTBase_getTagName(uint8_t type) {
    switch (type) {
    case 0:
        return "TAG_End";
    case 1:
        return "TAG_Byte";
    case 2:
        return "TAG_Short";
    case 3:
        return "TAG_Int";
    case 4:
        return "TAG_Long";
    case 5:
        return "TAG_Float";
    case 6:
        return "TAG_Double";
    case 7:
        return "TAG_Byte_Array";
    case 8:
        return "TAG_String";
    case 9:
        return "TAG_List";
    case 10:
        return "TAG_Compound";
    default:
        return "UNKNOWN";
    }
}
