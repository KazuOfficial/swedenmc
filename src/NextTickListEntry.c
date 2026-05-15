#include "NextTickListEntry.h"
#include <stdlib.h>

int64_t NextTickListEntry_nextTickEntryID = 0;

NextTickListEntry *NextTickListEntry_create(int x, int y, int z, int blockID) {
    NextTickListEntry *self = (NextTickListEntry *)calloc(1, sizeof(NextTickListEntry));
    self->xCoord = x;
    self->yCoord = y;
    self->zCoord = z;
    self->blockID = blockID;
    self->scheduledTime = 0;
    self->tickEntryID = NextTickListEntry_nextTickEntryID++;
    return self;
}

void NextTickListEntry_free(NextTickListEntry *self) { free(self); }

NextTickListEntry *NextTickListEntry_setScheduledTime(NextTickListEntry *self,
                                                      int64_t scheduledTime) {
    self->scheduledTime = scheduledTime;
    return self;
}

int NextTickListEntry_equals(const NextTickListEntry *self, const NextTickListEntry *other) {
    if (other == NULL)
        return 0;
    return self->xCoord == other->xCoord && self->yCoord == other->yCoord &&
           self->zCoord == other->zCoord && self->blockID == other->blockID;
}

int NextTickListEntry_hashCode(const NextTickListEntry *self) {
    return (self->xCoord * 128 * 1024 + self->zCoord * 128 + self->yCoord) * 256 + self->blockID;
}

int NextTickListEntry_comparer(const NextTickListEntry *self, const NextTickListEntry *other) {
    if (self->scheduledTime < other->scheduledTime)
        return -1;
    if (self->scheduledTime > other->scheduledTime)
        return 1;
    if (self->tickEntryID < other->tickEntryID)
        return -1;
    if (self->tickEntryID > other->tickEntryID)
        return 1;
    return 0;
}
