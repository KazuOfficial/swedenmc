#include "MapDataBase.h"

void MapDataBase_markDirty(MapDataBase *self) { MapDataBase_setDirty(self, 1); }

void MapDataBase_setDirty(MapDataBase *self, int var1) { self->dirty = var1; }

int MapDataBase_isDirty(MapDataBase *self) { return self->dirty; }
