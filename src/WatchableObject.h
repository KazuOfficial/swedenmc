#ifndef WATCHABLE_OBJECT_H
#define WATCHABLE_OBJECT_H

#include <stdint.h>
#include "ItemStack.h"
#include "ChunkCoordinates.h"

#define DW_TYPE_BYTE 0
#define DW_TYPE_SHORT 1
#define DW_TYPE_INT 2
#define DW_TYPE_FLOAT 3
#define DW_TYPE_STRING 4
#define DW_TYPE_ITEM_STACK 5
#define DW_TYPE_CHUNK_COORD 6

typedef union {
    int8_t asByte;
    int16_t asShort;
    int32_t asInt;
    float asFloat;
    char *asString;
    ItemStack asItemStack;
    ChunkCoordinates asChunkCoord;
} DWValue;

typedef struct {
    int id;
    int type;
    DWValue value;
    int changed;
    int used;
} WatchableObject;

void WatchableObject_init(WatchableObject *self, int type, int id, DWValue value);

int WatchableObject_getDataValueId(WatchableObject *self);

int WatchableObject_getObjectType(WatchableObject *self);

DWValue WatchableObject_getObject(WatchableObject *self);

void WatchableObject_setObject(WatchableObject *self, DWValue value);

void WatchableObject_setWatching(WatchableObject *self, int watching);

#endif
