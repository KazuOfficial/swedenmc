#ifndef MC_HASH_ENTRY_H
#define MC_HASH_ENTRY_H

typedef struct MCHashEntry {

    int hashEntry;

    void *valueEntry;

    struct MCHashEntry *nextEntry;

    int slotHash;
} MCHashEntry;

MCHashEntry *MCHashEntry_create(int var1, int var2, void *var3, MCHashEntry *var4);

int MCHashEntry_getHash(const MCHashEntry *self);

void *MCHashEntry_getValue(const MCHashEntry *self);

int MCHashEntry_equals(const MCHashEntry *self, const MCHashEntry *var1);

int MCHashEntry_hashCode(const MCHashEntry *self);

char *MCHashEntry_toString(const MCHashEntry *self);

#endif
