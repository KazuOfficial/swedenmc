#include "MapData.h"
#include "NBTTagCompound.h"
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include <stdlib.h>
#include <string.h>

static MapDataBase_vtable mapdata_vtable = {
    MapData_readFromNBT,
    MapData_writeToNBT,
};

void MapData_init(MapData *self, const char *var1) {
    self->base.vtable = &mapdata_vtable;
    self->base.field_28168_a = var1;
    self->base.dirty = 0;

    self->field_28180_b = 0;
    self->field_28179_c = 0;
    self->field_28178_d = 0;
    self->field_28177_e = 0;
    memset(self->field_28176_f, 0, sizeof(self->field_28176_f));
    self->field_28175_g = 0;

    self->field_28174_h = NULL;
    self->field_28174_h_size = 0;
    self->field_28174_h_cap = 0;

    self->field_28172_j_keys = NULL;
    self->field_28172_j_vals = NULL;
    self->field_28172_j_size = 0;
    self->field_28172_j_cap = 0;

    self->field_28173_i = NULL;
    self->field_28173_i_size = 0;
    self->field_28173_i_cap = 0;
}

void MapData_readFromNBT(MapDataBase *base, NBTTagCompound *var1) {
    MapData *self = (MapData *)base;
    self->field_28178_d = NBTTagCompound_getByte(var1, "dimension");
    self->field_28180_b = NBTTagCompound_getInteger(var1, "xCenter");
    self->field_28179_c = NBTTagCompound_getInteger(var1, "zCenter");
    self->field_28177_e = NBTTagCompound_getByte(var1, "scale");
    if (self->field_28177_e < 0) {
        self->field_28177_e = 0;
    }
    if (self->field_28177_e > 4) {
        self->field_28177_e = 4;
    }

    int16_t var2 = NBTTagCompound_getShort(var1, "width");
    int16_t var3 = NBTTagCompound_getShort(var1, "height");
    if (var2 == 128 && var3 == 128) {

        uint8_t *colors = NBTTagCompound_getByteArray(var1, "colors");
        memcpy(self->field_28176_f, colors, 16384);
    } else {

        uint8_t *var4 = NBTTagCompound_getByteArray(var1, "colors");

        memset(self->field_28176_f, 0, sizeof(self->field_28176_f));
        int var5 = (128 - var2) / 2;
        int var6 = (128 - var3) / 2;

        for (int var7 = 0; var7 < var3; ++var7) {
            int var8 = var7 + var6;

            {
                for (int var9 = 0; var9 < var2; ++var9) {
                    int var10 = var9 + var5;

                    {
                        self->field_28176_f[var10 + var8 * 128] = (int8_t)var4[var9 + var7 * var2];
                    }
                }
            }
        }
    }
}

void MapData_writeToNBT(MapDataBase *base, NBTTagCompound *var1) {
    MapData *self = (MapData *)base;
    NBTTagCompound_setByte(var1, "dimension", self->field_28178_d);
    NBTTagCompound_setInteger(var1, "xCenter", self->field_28180_b);
    NBTTagCompound_setInteger(var1, "zCenter", self->field_28179_c);
    NBTTagCompound_setByte(var1, "scale", self->field_28177_e);
    NBTTagCompound_setShort(var1, "width", (int16_t)128);
    NBTTagCompound_setShort(var1, "height", (int16_t)128);
    NBTTagCompound_setByteArray(var1, "colors", (uint8_t *)self->field_28176_f, 16384);
}

static int mapdata_j_containsKey(MapData *self, struct EntityPlayer *key) {
    for (int i = 0; i < self->field_28172_j_size; ++i) {
        if (self->field_28172_j_keys[i] == key)
            return 1;
    }
    return 0;
}

static void mapdata_j_put(MapData *self, struct EntityPlayer *key, MapInfo *val) {

    if (self->field_28172_j_size >= self->field_28172_j_cap) {
        int newcap = self->field_28172_j_cap ? self->field_28172_j_cap * 2 : 4;
        self->field_28172_j_keys =
            realloc(self->field_28172_j_keys, newcap * sizeof(struct EntityPlayer *));
        self->field_28172_j_vals = realloc(self->field_28172_j_vals, newcap * sizeof(MapInfo *));
        self->field_28172_j_cap = newcap;
    }
    self->field_28172_j_keys[self->field_28172_j_size] = key;
    self->field_28172_j_vals[self->field_28172_j_size] = val;
    ++self->field_28172_j_size;
}

static void mapdata_j_remove(MapData *self, struct EntityPlayer *key) {
    for (int i = 0; i < self->field_28172_j_size; ++i) {
        if (self->field_28172_j_keys[i] == key) {
            self->field_28172_j_keys[i] = self->field_28172_j_keys[self->field_28172_j_size - 1];
            self->field_28172_j_vals[i] = self->field_28172_j_vals[self->field_28172_j_size - 1];
            --self->field_28172_j_size;
            return;
        }
    }
}

static void mapdata_h_add(MapData *self, MapInfo *val) {
    if (self->field_28174_h_size >= self->field_28174_h_cap) {
        int newcap = self->field_28174_h_cap ? self->field_28174_h_cap * 2 : 4;
        self->field_28174_h = realloc(self->field_28174_h, newcap * sizeof(MapInfo *));
        self->field_28174_h_cap = newcap;
    }
    self->field_28174_h[self->field_28174_h_size++] = val;
}

static void mapdata_h_remove_at(MapData *self, int idx) {
    for (int i = idx; i < self->field_28174_h_size - 1; ++i) {
        self->field_28174_h[i] = self->field_28174_h[i + 1];
    }
    --self->field_28174_h_size;
}

static void mapdata_i_add(MapData *self, MapCoord *val) {
    if (self->field_28173_i_size >= self->field_28173_i_cap) {
        int newcap = self->field_28173_i_cap ? self->field_28173_i_cap * 2 : 4;
        self->field_28173_i = realloc(self->field_28173_i, newcap * sizeof(MapCoord *));
        self->field_28173_i_cap = newcap;
    }
    self->field_28173_i[self->field_28173_i_size++] = val;
}

void MapData_func_28169_a(MapData *self, struct EntityPlayer *var1, struct ItemStack *var2) {
    if (!mapdata_j_containsKey(self, var1)) {
        MapInfo *var3 = calloc(1, sizeof(MapInfo));
        MapInfo_init(var3, self, var1);
        mapdata_j_put(self, var1, var3);
        mapdata_h_add(self, var3);
    }

    self->field_28173_i_size = 0;

    for (int var14 = 0; var14 < self->field_28174_h_size; ++var14) {
        MapInfo *var4 = self->field_28174_h[var14];
        if (!var4->entityplayerObj->base.base.isDead &&
            InventoryPlayer_func_28018_c(&var4->entityplayerObj->inventory, var2)) {
            float var5 =
                (float)(var4->entityplayerObj->base.base.posX - (double)self->field_28180_b) /
                (float)(1 << self->field_28177_e);
            float var6 =
                (float)(var4->entityplayerObj->base.base.posZ - (double)self->field_28179_c) /
                (float)(1 << self->field_28177_e);
            int8_t var7 = 64;
            int8_t var8 = 64;
            if (var5 >= (float)(-var7) && var6 >= (float)(-var8) && var5 <= (float)var7 &&
                var6 <= (float)var8) {
                int8_t var9 = 0;
                int8_t var10 = (int8_t)((int)((double)(var5 * 2.0F) + 0.5));
                int8_t var11 = (int8_t)((int)((double)(var6 * 2.0F) + 0.5));

                int8_t var12 =
                    (int8_t)((int)((double)(var1->base.base.rotationYaw * 16.0F / 360.0F) + 0.5));
                if (self->field_28178_d < 0) {
                    int var13 = self->field_28175_g / 10;
                    var12 = (int8_t)(((var13 * var13 * 34187121 + var13 * 121) >> 15) & 15);
                }

                if (var4->entityplayerObj->dimension == (int)self->field_28178_d) {
                    MapCoord *mc = calloc(1, sizeof(MapCoord));
                    MapCoord_init(mc, self, var9, var10, var11, var12);
                    mapdata_i_add(self, mc);
                }
            }
        } else {

            mapdata_j_remove(self, var4->entityplayerObj);

            mapdata_h_remove_at(self, var14);
            --var14;
        }
    }
}

void MapData_func_28170_a(MapData *self, int var1, int var2, int var3) {

    MapDataBase_markDirty(&self->base);

    for (int var4 = 0; var4 < self->field_28174_h_size; ++var4) {
        MapInfo *var5 = self->field_28174_h[var4];
        if (var5->field_28119_b[var1] < 0 || var5->field_28119_b[var1] > var2) {
            var5->field_28119_b[var1] = var2;
        }
        if (var5->field_28124_c[var1] < 0 || var5->field_28124_c[var1] < var3) {
            var5->field_28124_c[var1] = var3;
        }
    }
}

void MapData_func_28171_a(MapData *self, int8_t *var1, int var1_len) {
    int var2;
    if (var1[0] == 0) {
        var2 = var1[1] & 255;
        int var3 = var1[2] & 255;

        for (int var4 = 0; var4 < var1_len - 3; ++var4) {
            self->field_28176_f[(var4 + var3) * 128 + var2] = var1[var4 + 3];
        }

        MapDataBase_markDirty(&self->base);
    } else if (var1[0] == 1) {

        self->field_28173_i_size = 0;

        for (var2 = 0; var2 < (var1_len - 1) / 3; ++var2) {
            int8_t var7 = (int8_t)(var1[var2 * 3 + 1] % 16);
            int8_t var8 = var1[var2 * 3 + 2];
            int8_t var5 = var1[var2 * 3 + 3];
            int8_t var6 = (int8_t)(var1[var2 * 3 + 1] / 16);
            MapCoord *mc = calloc(1, sizeof(MapCoord));
            MapCoord_init(mc, self, var7, var8, var5, var6);
            mapdata_i_add(self, mc);
        }
    }
}
