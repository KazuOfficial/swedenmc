#ifndef MAP_STORAGE_H
#define MAP_STORAGE_H

#include "MapDataBase.h"

struct ISaveHandler;

typedef struct MapStorage {

    struct ISaveHandler *field_28191_a;

    char **loadedDataMap_keys;
    MapDataBase **loadedDataMap_vals;
    int loadedDataMap_size;
    int loadedDataMap_cap;

    MapDataBase **loadedDataList;
    int loadedDataList_size;
    int loadedDataList_cap;

    char **idCounts_keys;
    int16_t *idCounts_vals;
    int idCounts_size;
    int idCounts_cap;
} MapStorage;

void MapStorage_init(MapStorage *self, struct ISaveHandler *var1);

MapDataBase *MapStorage_loadData(MapStorage *self, MapDataBase *(*factory)(const char *),
                                 const char *var2);

void MapStorage_setData(MapStorage *self, const char *var1, MapDataBase *var2);

void MapStorage_saveAllData(MapStorage *self);

int MapStorage_getUniqueDataId(MapStorage *self, const char *var1);

void MapStorage_destroy(MapStorage *self);

#endif
