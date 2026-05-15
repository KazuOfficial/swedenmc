#ifndef MC_HASH_H
#define MC_HASH_H

#include "MCHashEntry.h"

#define MC_HASH_INITIAL_SLOTS 16

typedef struct MCHash {

    MCHashEntry **slots;
    int slotsLen;

    int count;

    int threshold;

    int versionStamp;
} MCHash;

void MCHash_init(MCHash *self);
void MCHash_destroy(MCHash *self);

void *MCHash_lookup(MCHash *self, int var1);

void MCHash_addKey(MCHash *self, int var1, void *var2);

void *MCHash_removeObject(MCHash *self, int var1);

void MCHash_clearMap(MCHash *self);

int MCHash_getHash(int var0);

#endif
