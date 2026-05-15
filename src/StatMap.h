#ifndef STAT_MAP_H
#define STAT_MAP_H

#include "StatBase.h"

#define STAT_MAP_CAPACITY 4096

typedef struct StatMapEntry {
    StatBase *key;
    int value;
} StatMapEntry;

typedef struct StatMap {
    StatMapEntry entries[STAT_MAP_CAPACITY];
    int count;
} StatMap;

StatMap *StatMap_create(void);

StatMap *StatMap_copy(const StatMap *src);
void StatMap_destroy(StatMap *self);

void StatMap_put(StatMap *self, StatBase *key, int value);

int StatMap_containsKey(const StatMap *self, StatBase *key);

int StatMap_get(const StatMap *self, StatBase *key, int *found);

#endif
