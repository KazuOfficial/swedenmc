#ifndef NEXT_TICK_LIST_ENTRY_H
#define NEXT_TICK_LIST_ENTRY_H

#include <stdint.h>

typedef struct NextTickListEntry {
    int xCoord;
    int yCoord;
    int zCoord;
    int blockID;
    int64_t scheduledTime;
    int64_t tickEntryID;
} NextTickListEntry;

extern int64_t NextTickListEntry_nextTickEntryID;

NextTickListEntry *NextTickListEntry_create(int x, int y, int z, int blockID);
void NextTickListEntry_free(NextTickListEntry *self);

NextTickListEntry *NextTickListEntry_setScheduledTime(NextTickListEntry *self,
                                                      int64_t scheduledTime);

int NextTickListEntry_equals(const NextTickListEntry *self, const NextTickListEntry *other);

int NextTickListEntry_hashCode(const NextTickListEntry *self);

int NextTickListEntry_comparer(const NextTickListEntry *self, const NextTickListEntry *other);

#endif
