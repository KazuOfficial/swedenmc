#ifndef DATA_WATCHER_H
#define DATA_WATCHER_H

#include "WatchableObject.h"
#include "DataStream.h"

#define DW_MAX_OBJECTS 32

typedef struct {
    WatchableObject objects[DW_MAX_OBJECTS];
    int objectChanged;
} DataWatcher;

void DataWatcher_init(DataWatcher *dw);
void DataWatcher_destroy(DataWatcher *dw);

void DataWatcher_addObject(DataWatcher *dw, int id, int type, DWValue value);

void DataWatcher_addByte(DataWatcher *dw, int id, int8_t val);
void DataWatcher_addShort(DataWatcher *dw, int id, int16_t val);
void DataWatcher_addInt(DataWatcher *dw, int id, int32_t val);
void DataWatcher_addFloat(DataWatcher *dw, int id, float val);
void DataWatcher_addString(DataWatcher *dw, int id, const char *str);

void DataWatcher_updateObject(DataWatcher *dw, int id, int type, DWValue value);
void DataWatcher_updateByte(DataWatcher *dw, int id, int8_t val);
void DataWatcher_updateShort(DataWatcher *dw, int id, int16_t val);
void DataWatcher_updateInt(DataWatcher *dw, int id, int32_t val);
void DataWatcher_updateFloat(DataWatcher *dw, int id, float val);
void DataWatcher_updateString(DataWatcher *dw, int id, const char *str);

int8_t DataWatcher_getWatchableObjectByte(DataWatcher *dw, int id);
int16_t DataWatcher_getWatchableObjectShort(DataWatcher *dw, int id);
int32_t DataWatcher_getWatchableObjectInt(DataWatcher *dw, int id);
float DataWatcher_getWatchableObjectFloat(DataWatcher *dw, int id);
const char *DataWatcher_getWatchableObjectString(DataWatcher *dw, int id);

void DataWatcher_updateWatchedObjectsFromList(DataWatcher *dw, DataWatcher *list);

void DataWatcher_writeToStream(DataWatcher *dw, DataStream *s);
void DataWatcher_readFromStream(DataWatcher *dw, DataStream *s);

#endif
