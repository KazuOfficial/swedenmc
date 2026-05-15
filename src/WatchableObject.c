#include "WatchableObject.h"

void WatchableObject_init(WatchableObject *self, int type, int id, DWValue value) {
    self->id = id;
    self->type = type;
    self->value = value;
    self->changed = 1;
    self->used = 1;
}

int WatchableObject_getDataValueId(WatchableObject *self) { return self->id; }

int WatchableObject_getObjectType(WatchableObject *self) { return self->type; }

DWValue WatchableObject_getObject(WatchableObject *self) { return self->value; }

void WatchableObject_setObject(WatchableObject *self, DWValue value) { self->value = value; }

void WatchableObject_setWatching(WatchableObject *self, int watching) { self->changed = watching; }
