#include "MCHash.h"
#include <stdlib.h>
#include <string.h>

static int computeHash(int var0) {
    var0 ^= (unsigned int)var0 >> 20 ^ (unsigned int)var0 >> 12;
    return var0 ^ (unsigned int)var0 >> 7 ^ (unsigned int)var0 >> 4;
}

static int getSlotIndex(int var0, int var1) { return var0 & var1 - 1; }

void MCHash_init(MCHash *self) {
    self->slotsLen = MC_HASH_INITIAL_SLOTS;
    self->slots = (MCHashEntry **)calloc(MC_HASH_INITIAL_SLOTS, sizeof(MCHashEntry *));
    self->count = 0;
    self->threshold = 12;

    self->versionStamp = 0;
}

void MCHash_destroy(MCHash *self) {
    for (int i = 0; i < self->slotsLen; i++) {
        MCHashEntry *e = self->slots[i];
        while (e) {
            MCHashEntry *next = e->nextEntry;
            free(e);
            e = next;
        }
    }
    free(self->slots);
    self->slots = NULL;
}

void *MCHash_lookup(MCHash *self, int var1) {
    int var2 = computeHash(var1);
    for (MCHashEntry *var3 = self->slots[getSlotIndex(var2, self->slotsLen)]; var3 != NULL;
         var3 = var3->nextEntry) {
        if (var3->hashEntry == var1) {
            return var3->valueEntry;
        }
    }
    return NULL;
}

static void MCHash_insert(MCHash *self, int var1, int var2, void *var3, int var4);

static void MCHash_copyTo(MCHash *self, MCHashEntry **var1, int var1Len) {
    MCHashEntry **var2 = self->slots;
    int var3 = var1Len;
    for (int var4 = 0; var4 < self->slotsLen; ++var4) {
        MCHashEntry *var5 = var2[var4];
        if (var5 != NULL) {
            var2[var4] = NULL;
            MCHashEntry *var6;
            do {
                var6 = var5->nextEntry;
                int var7 = getSlotIndex(var5->slotHash, var3);
                var5->nextEntry = var1[var7];
                var1[var7] = var5;
                var5 = var6;
            } while (var6 != NULL);
        }
    }
}

static void MCHash_grow(MCHash *self, int var1) {
    int var3 = self->slotsLen;
    if (var3 == 1073741824) {
        self->threshold = 0x7fffffff;
    } else {
        MCHashEntry **var4 = (MCHashEntry **)calloc(var1, sizeof(MCHashEntry *));
        MCHash_copyTo(self, var4, var1);
        free(self->slots);
        self->slots = var4;
        self->slotsLen = var1;
        self->threshold = (int)((float)var1 * (12.0F / 16.0F));
    }
}

void MCHash_addKey(MCHash *self, int var1, void *var2) {
    int var3 = computeHash(var1);
    int var4 = getSlotIndex(var3, self->slotsLen);
    for (MCHashEntry *var5 = self->slots[var4]; var5 != NULL; var5 = var5->nextEntry) {
        if (var5->hashEntry == var1) {
            var5->valueEntry = var2;
        }
    }
    ++self->versionStamp;
    MCHash_insert(self, var3, var1, var2, var4);
}

static MCHashEntry *MCHash_removeEntry(MCHash *self, int var1) {
    int var2 = computeHash(var1);
    int var3 = getSlotIndex(var2, self->slotsLen);
    MCHashEntry *var4 = self->slots[var3];
    MCHashEntry *var5, *var6;
    for (var5 = var4; var5 != NULL; var5 = var6) {
        var6 = var5->nextEntry;
        if (var5->hashEntry == var1) {
            ++self->versionStamp;
            --self->count;
            if (var4 == var5) {
                self->slots[var3] = var6;
            } else {
                var4->nextEntry = var6;
            }
            return var5;
        }
        var4 = var5;
    }
    return var5;
}

void *MCHash_removeObject(MCHash *self, int var1) {
    MCHashEntry *var2 = MCHash_removeEntry(self, var1);
    if (var2 == NULL)
        return NULL;
    void *val = var2->valueEntry;
    free(var2);
    return val;
}

void MCHash_clearMap(MCHash *self) {
    ++self->versionStamp;
    for (int var2 = 0; var2 < self->slotsLen; ++var2) {
        MCHashEntry *e = self->slots[var2];
        while (e) {
            MCHashEntry *next = e->nextEntry;
            free(e);
            e = next;
        }
        self->slots[var2] = NULL;
    }
    self->count = 0;
}

static void MCHash_insert(MCHash *self, int var1, int var2, void *var3, int var4) {
    MCHashEntry *var5 = self->slots[var4];
    self->slots[var4] = MCHashEntry_create(var1, var2, var3, var5);
    if (self->count++ >= self->threshold) {
        MCHash_grow(self, 2 * self->slotsLen);
    }
}

int MCHash_getHash(int var0) { return computeHash(var0); }
