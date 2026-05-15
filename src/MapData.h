#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "MapDataBase.h"
#include "MapInfo.h"
#include "MapCoord.h"
#include "NBTTagCompound.h"
#include <stdint.h>

struct EntityPlayer;
struct ItemStack;

typedef struct MapData {

    MapDataBase base;

    int field_28180_b;

    int field_28179_c;

    int8_t field_28178_d;

    int8_t field_28177_e;

    int8_t field_28176_f[16384];

    int field_28175_g;

    MapInfo **field_28174_h;
    int field_28174_h_size;
    int field_28174_h_cap;

    struct EntityPlayer **field_28172_j_keys;
    MapInfo **field_28172_j_vals;
    int field_28172_j_size;
    int field_28172_j_cap;

    MapCoord **field_28173_i;
    int field_28173_i_size;
    int field_28173_i_cap;
} MapData;

void MapData_init(MapData *self, const char *var1);

void MapData_readFromNBT(MapDataBase *self, NBTTagCompound *var1);

void MapData_writeToNBT(MapDataBase *self, NBTTagCompound *var1);

void MapData_func_28169_a(MapData *self, struct EntityPlayer *var1, struct ItemStack *var2);

void MapData_func_28170_a(MapData *self, int var1, int var2, int var3);

void MapData_func_28171_a(MapData *self, int8_t *var1, int var1_len);

#endif
