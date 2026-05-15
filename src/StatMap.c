#include "StatMap.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned stat_map_hash(StatBase *key) {

    return (unsigned)((uintptr_t)key >> 3) % STAT_MAP_CAPACITY;
}

StatMap *StatMap_create(void) { return (StatMap *)calloc(1, sizeof(StatMap)); }

StatMap *StatMap_copy(const StatMap *src) {
    StatMap *dst = (StatMap *)calloc(1, sizeof(StatMap));
    if (dst)
        memcpy(dst, src, sizeof(StatMap));
    return dst;
}

void StatMap_destroy(StatMap *self) { free(self); }

void StatMap_put(StatMap *self, StatBase *key, int value) {
    unsigned h = stat_map_hash(key);
    unsigned i;
    for (i = 0; i < STAT_MAP_CAPACITY; i++) {
        unsigned idx = (h + i) % STAT_MAP_CAPACITY;
        if (self->entries[idx].key == NULL) {
            self->entries[idx].key = key;
            self->entries[idx].value = value;
            self->count++;
            return;
        }
        if (self->entries[idx].key == key) {
            self->entries[idx].value = value;
            return;
        }
    }
}

int StatMap_containsKey(const StatMap *self, StatBase *key) {
    unsigned h = stat_map_hash(key);
    unsigned i;
    for (i = 0; i < STAT_MAP_CAPACITY; i++) {
        unsigned idx = (h + i) % STAT_MAP_CAPACITY;
        if (self->entries[idx].key == NULL)
            return 0;
        if (self->entries[idx].key == key)
            return 1;
    }
    return 0;
}

int StatMap_get(const StatMap *self, StatBase *key, int *found) {
    unsigned h = stat_map_hash(key);
    unsigned i;
    for (i = 0; i < STAT_MAP_CAPACITY; i++) {
        unsigned idx = (h + i) % STAT_MAP_CAPACITY;
        if (self->entries[idx].key == NULL) {
            if (found)
                *found = 0;
            return 0;
        }
        if (self->entries[idx].key == key) {
            if (found)
                *found = 1;
            return self->entries[idx].value;
        }
    }
    if (found)
        *found = 0;
    return 0;
}
