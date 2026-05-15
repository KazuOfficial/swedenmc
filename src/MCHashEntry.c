#include "MCHashEntry.h"
#include "MCHash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

MCHashEntry *MCHashEntry_create(int var1, int var2, void *var3, MCHashEntry *var4) {
    MCHashEntry *self = (MCHashEntry *)calloc(1, sizeof(MCHashEntry));
    self->valueEntry = var3;
    self->nextEntry = var4;
    self->hashEntry = var2;
    self->slotHash = var1;
    return self;
}

int MCHashEntry_getHash(const MCHashEntry *self) { return self->hashEntry; }

void *MCHashEntry_getValue(const MCHashEntry *self) { return self->valueEntry; }

int MCHashEntry_equals(const MCHashEntry *self, const MCHashEntry *var1) {
    if (var1 == NULL)
        return 0;

    if (MCHashEntry_getHash(self) != MCHashEntry_getHash(var1))
        return 0;
    if (MCHashEntry_getValue(self) != MCHashEntry_getValue(var1))
        return 0;
    return 1;
}

int MCHashEntry_hashCode(const MCHashEntry *self) { return MCHash_getHash(self->hashEntry); }

char *MCHashEntry_toString(const MCHashEntry *self) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%d=%p", MCHashEntry_getHash(self), MCHashEntry_getValue(self));
    return strdup(buf);
}
