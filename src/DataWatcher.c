#include "DataWatcher.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void DataWatcher_init(DataWatcher *dw) { memset(dw, 0, sizeof(*dw)); }

void DataWatcher_destroy(DataWatcher *dw) {
    for (int i = 0; i < DW_MAX_OBJECTS; i++) {
        if (dw->objects[i].used && dw->objects[i].type == DW_TYPE_STRING &&
            dw->objects[i].value.asString) {
            free(dw->objects[i].value.asString);
            dw->objects[i].value.asString = NULL;
        }
    }
}

static WatchableObject *dw_find(DataWatcher *dw, int id) {
    if (id < 0 || id >= DW_MAX_OBJECTS)
        return NULL;
    if (!dw->objects[id].used)
        return NULL;
    return &dw->objects[id];
}

void DataWatcher_addObject(DataWatcher *dw, int id, int type, DWValue value) {
    if (id < 0 || id >= DW_MAX_OBJECTS)
        return;
    dw->objects[id].id = id;
    dw->objects[id].type = type;
    dw->objects[id].value = value;
    dw->objects[id].changed = 1;
    dw->objects[id].used = 1;

    if (type == DW_TYPE_STRING && value.asString) {
        dw->objects[id].value.asString = strdup(value.asString);
    }
}

void DataWatcher_addByte(DataWatcher *dw, int id, int8_t val) {
    DWValue v;
    v.asByte = val;
    DataWatcher_addObject(dw, id, DW_TYPE_BYTE, v);
}
void DataWatcher_addShort(DataWatcher *dw, int id, int16_t val) {
    DWValue v;
    v.asShort = val;
    DataWatcher_addObject(dw, id, DW_TYPE_SHORT, v);
}
void DataWatcher_addInt(DataWatcher *dw, int id, int32_t val) {
    DWValue v;
    v.asInt = val;
    DataWatcher_addObject(dw, id, DW_TYPE_INT, v);
}
void DataWatcher_addFloat(DataWatcher *dw, int id, float val) {
    DWValue v;
    v.asFloat = val;
    DataWatcher_addObject(dw, id, DW_TYPE_FLOAT, v);
}
void DataWatcher_addString(DataWatcher *dw, int id, const char *str) {
    DWValue v;
    v.asString = (char *)str;
    DataWatcher_addObject(dw, id, DW_TYPE_STRING, v);
}

void DataWatcher_updateObject(DataWatcher *dw, int id, int type, DWValue value) {
    WatchableObject *obj = dw_find(dw, id);
    if (!obj)
        return;

    int equal = 0;
    switch (type) {
    case DW_TYPE_BYTE:
        equal = (obj->value.asByte == value.asByte);
        break;
    case DW_TYPE_SHORT:
        equal = (obj->value.asShort == value.asShort);
        break;
    case DW_TYPE_INT:
        equal = (obj->value.asInt == value.asInt);
        break;
    case DW_TYPE_FLOAT:
        equal = (obj->value.asFloat == value.asFloat);
        break;
    case DW_TYPE_STRING:
        equal = (obj->value.asString == value.asString) ||
                (obj->value.asString && value.asString &&
                 strcmp(obj->value.asString, value.asString) == 0);
        break;
    case DW_TYPE_ITEM_STACK:
        equal = (obj->value.asItemStack.itemID == value.asItemStack.itemID &&
                 obj->value.asItemStack.stackSize == value.asItemStack.stackSize &&
                 obj->value.asItemStack.itemDamage == value.asItemStack.itemDamage);
        break;
    case DW_TYPE_CHUNK_COORD:
        equal = (obj->value.asChunkCoord.x == value.asChunkCoord.x &&
                 obj->value.asChunkCoord.y == value.asChunkCoord.y &&
                 obj->value.asChunkCoord.z == value.asChunkCoord.z);
        break;
    }
    if (!equal) {
        if (type == DW_TYPE_STRING) {
            if (obj->value.asString)
                free(obj->value.asString);
            obj->value.asString = value.asString ? strdup(value.asString) : NULL;
        } else {
            obj->value = value;
        }
        obj->changed = 1;
        dw->objectChanged = 1;
    }
}

void DataWatcher_updateByte(DataWatcher *dw, int id, int8_t val) {
    DWValue v;
    v.asByte = val;
    DataWatcher_updateObject(dw, id, DW_TYPE_BYTE, v);
}
void DataWatcher_updateShort(DataWatcher *dw, int id, int16_t val) {
    DWValue v;
    v.asShort = val;
    DataWatcher_updateObject(dw, id, DW_TYPE_SHORT, v);
}
void DataWatcher_updateInt(DataWatcher *dw, int id, int32_t val) {
    DWValue v;
    v.asInt = val;
    DataWatcher_updateObject(dw, id, DW_TYPE_INT, v);
}
void DataWatcher_updateFloat(DataWatcher *dw, int id, float val) {
    DWValue v;
    v.asFloat = val;
    DataWatcher_updateObject(dw, id, DW_TYPE_FLOAT, v);
}
void DataWatcher_updateString(DataWatcher *dw, int id, const char *str) {
    DWValue v;
    v.asString = (char *)str;
    DataWatcher_updateObject(dw, id, DW_TYPE_STRING, v);
}

int8_t DataWatcher_getWatchableObjectByte(DataWatcher *dw, int id) {
    WatchableObject *obj = dw_find(dw, id);
    return obj ? obj->value.asByte : 0;
}
int16_t DataWatcher_getWatchableObjectShort(DataWatcher *dw, int id) {
    WatchableObject *obj = dw_find(dw, id);
    return obj ? obj->value.asShort : 0;
}
int32_t DataWatcher_getWatchableObjectInt(DataWatcher *dw, int id) {
    WatchableObject *obj = dw_find(dw, id);
    return obj ? obj->value.asInt : 0;
}
float DataWatcher_getWatchableObjectFloat(DataWatcher *dw, int id) {
    WatchableObject *obj = dw_find(dw, id);
    return obj ? obj->value.asFloat : 0.0f;
}
const char *DataWatcher_getWatchableObjectString(DataWatcher *dw, int id) {
    WatchableObject *obj = dw_find(dw, id);
    return obj ? obj->value.asString : "";
}

void DataWatcher_updateWatchedObjectsFromList(DataWatcher *dw, DataWatcher *list) {
    for (int i = 0; i < DW_MAX_OBJECTS; i++) {
        WatchableObject *var3 = &list->objects[i];
        if (!var3->used)
            continue;
        WatchableObject *var4 = dw_find(dw, var3->id);
        if (var4 != NULL) {

            if (var4->type == DW_TYPE_STRING) {
                if (var4->value.asString)
                    free(var4->value.asString);
                var4->value.asString = var3->value.asString ? strdup(var3->value.asString) : NULL;
            } else {
                var4->value = var3->value;
            }
        }
    }
}

void DataWatcher_writeToStream(DataWatcher *dw, DataStream *s) {
    for (int i = 0; i < DW_MAX_OBJECTS; i++) {
        WatchableObject *obj = &dw->objects[i];
        if (!obj->used)
            continue;
        uint8_t header = (uint8_t)((obj->type << 5) | (obj->id & 31));
        DataStream_writeByte(s, header);
        switch (obj->type) {
        case DW_TYPE_BYTE:
            DataStream_writeByte(s, (uint8_t)obj->value.asByte);
            break;
        case DW_TYPE_SHORT:
            DataStream_writeShort(s, obj->value.asShort);
            break;
        case DW_TYPE_INT:
            DataStream_writeInt(s, obj->value.asInt);
            break;
        case DW_TYPE_FLOAT:
            DataStream_writeFloat(s, obj->value.asFloat);
            break;
        case DW_TYPE_STRING:
            DataStream_writeUTF(s, obj->value.asString ? obj->value.asString : "");
            break;
        case DW_TYPE_ITEM_STACK: {
            ItemStack *is = &obj->value.asItemStack;
            DataStream_writeShort(s, (int16_t)is->itemID);
            DataStream_writeByte(s, (uint8_t)is->stackSize);
            DataStream_writeShort(s, (int16_t)is->itemDamage);
            break;
        }
        case DW_TYPE_CHUNK_COORD: {
            ChunkCoordinates *cc = &obj->value.asChunkCoord;
            DataStream_writeInt(s, cc->x);
            DataStream_writeInt(s, cc->y);
            DataStream_writeInt(s, cc->z);
            break;
        }
        }
    }
    DataStream_writeByte(s, 127);
}

void DataWatcher_readFromStream(DataWatcher *dw, DataStream *s) {
    uint8_t header;
    while ((header = DataStream_readByte(s)) != 127) {
        int type = (header >> 5) & 7;
        int id = header & 31;
        DWValue v;
        memset(&v, 0, sizeof(v));
        switch (type) {
        case DW_TYPE_BYTE:
            v.asByte = (int8_t)DataStream_readByte(s);
            break;
        case DW_TYPE_SHORT:
            v.asShort = DataStream_readShort(s);
            break;
        case DW_TYPE_INT:
            v.asInt = DataStream_readInt(s);
            break;
        case DW_TYPE_FLOAT:
            v.asFloat = DataStream_readFloat(s);
            break;
        case DW_TYPE_STRING:
            v.asString = DataStream_readUTF(s);
            break;
        case DW_TYPE_ITEM_STACK:
            v.asItemStack.itemID = DataStream_readShort(s);
            v.asItemStack.stackSize = DataStream_readByte(s);
            v.asItemStack.itemDamage = DataStream_readShort(s);
            break;
        case DW_TYPE_CHUNK_COORD:
            v.asChunkCoord.x = DataStream_readInt(s);
            v.asChunkCoord.y = DataStream_readInt(s);
            v.asChunkCoord.z = DataStream_readInt(s);
            break;
        }
        if (dw->objects[id].used) {
            DataWatcher_updateObject(dw, id, type, v);
        } else {
            DataWatcher_addObject(dw, id, type, v);
        }
        if (type == DW_TYPE_STRING && v.asString)
            free(v.asString);
    }
}
