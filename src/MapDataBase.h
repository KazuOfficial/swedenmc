#ifndef MAP_DATA_BASE_H
#define MAP_DATA_BASE_H

#include "NBTTagCompound.h"

typedef struct MapDataBase MapDataBase;

typedef struct MapDataBase_vtable {

    void (*readFromNBT)(MapDataBase *self, NBTTagCompound *var1);

    void (*writeToNBT)(MapDataBase *self, NBTTagCompound *var1);
} MapDataBase_vtable;

struct MapDataBase {
    MapDataBase_vtable *vtable;

    const char *field_28168_a;

    int dirty;
};

void MapDataBase_markDirty(MapDataBase *self);

void MapDataBase_setDirty(MapDataBase *self, int var1);

int MapDataBase_isDirty(MapDataBase *self);

#endif
