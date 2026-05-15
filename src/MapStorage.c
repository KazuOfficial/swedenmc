#include "MapStorage.h"
#include "ISaveHandler.h"
#include "NBTTagCompound.h"
#include "NBTTagShort.h"
#include "NBTBase.h"
#include "CompressedStreamTools.h"
#include "DataStream.h"
#include "stb_ds.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static MapDataBase *ldm_get(MapStorage *self, const char *key) {
    for (int i = 0; i < self->loadedDataMap_size; ++i) {
        if (strcmp(self->loadedDataMap_keys[i], key) == 0)
            return self->loadedDataMap_vals[i];
    }
    return NULL;
}

static int ldm_containsKey(MapStorage *self, const char *key) {
    for (int i = 0; i < self->loadedDataMap_size; ++i) {
        if (strcmp(self->loadedDataMap_keys[i], key) == 0)
            return 1;
    }
    return 0;
}

static void ldm_put(MapStorage *self, const char *key, MapDataBase *val) {

    for (int i = 0; i < self->loadedDataMap_size; ++i) {
        if (strcmp(self->loadedDataMap_keys[i], key) == 0) {
            self->loadedDataMap_vals[i] = val;
            return;
        }
    }

    if (self->loadedDataMap_size >= self->loadedDataMap_cap) {
        int nc = self->loadedDataMap_cap ? self->loadedDataMap_cap * 2 : 4;
        self->loadedDataMap_keys = realloc(self->loadedDataMap_keys, nc * sizeof(char *));
        self->loadedDataMap_vals = realloc(self->loadedDataMap_vals, nc * sizeof(MapDataBase *));
        self->loadedDataMap_cap = nc;
    }
    self->loadedDataMap_keys[self->loadedDataMap_size] = strdup(key);
    self->loadedDataMap_vals[self->loadedDataMap_size] = val;
    ++self->loadedDataMap_size;
}

static MapDataBase *ldm_remove(MapStorage *self, const char *key) {
    for (int i = 0; i < self->loadedDataMap_size; ++i) {
        if (strcmp(self->loadedDataMap_keys[i], key) == 0) {
            MapDataBase *val = self->loadedDataMap_vals[i];
            free(self->loadedDataMap_keys[i]);
            self->loadedDataMap_keys[i] = self->loadedDataMap_keys[self->loadedDataMap_size - 1];
            self->loadedDataMap_vals[i] = self->loadedDataMap_vals[self->loadedDataMap_size - 1];
            --self->loadedDataMap_size;
            return val;
        }
    }
    return NULL;
}

static void ldl_add(MapStorage *self, MapDataBase *val) {
    if (self->loadedDataList_size >= self->loadedDataList_cap) {
        int nc = self->loadedDataList_cap ? self->loadedDataList_cap * 2 : 4;
        self->loadedDataList = realloc(self->loadedDataList, nc * sizeof(MapDataBase *));
        self->loadedDataList_cap = nc;
    }
    self->loadedDataList[self->loadedDataList_size++] = val;
}

static void ldl_remove(MapStorage *self, MapDataBase *val) {
    for (int i = 0; i < self->loadedDataList_size; ++i) {
        if (self->loadedDataList[i] == val) {
            self->loadedDataList[i] = self->loadedDataList[self->loadedDataList_size - 1];
            --self->loadedDataList_size;
            return;
        }
    }
}

static int idc_get(MapStorage *self, const char *key, int16_t *out) {
    for (int i = 0; i < self->idCounts_size; ++i) {
        if (strcmp(self->idCounts_keys[i], key) == 0) {
            *out = self->idCounts_vals[i];
            return 1;
        }
    }
    return 0;
}

static void idc_put(MapStorage *self, const char *key, int16_t val) {
    for (int i = 0; i < self->idCounts_size; ++i) {
        if (strcmp(self->idCounts_keys[i], key) == 0) {
            self->idCounts_vals[i] = val;
            return;
        }
    }
    if (self->idCounts_size >= self->idCounts_cap) {
        int nc = self->idCounts_cap ? self->idCounts_cap * 2 : 4;
        self->idCounts_keys = realloc(self->idCounts_keys, nc * sizeof(char *));
        self->idCounts_vals = realloc(self->idCounts_vals, nc * sizeof(int16_t));
        self->idCounts_cap = nc;
    }
    self->idCounts_keys[self->idCounts_size] = strdup(key);
    self->idCounts_vals[self->idCounts_size] = val;
    ++self->idCounts_size;
}

static void idc_clear(MapStorage *self) {
    for (int i = 0; i < self->idCounts_size; ++i)
        free(self->idCounts_keys[i]);
    self->idCounts_size = 0;
}

void MapStorage_destroy(MapStorage *self) {
    for (int i = 0; i < self->loadedDataMap_size; ++i)
        free(self->loadedDataMap_keys[i]);
    free(self->loadedDataMap_keys);
    free(self->loadedDataMap_vals);
    free(self->loadedDataList);
    idc_clear(self);
    free(self->idCounts_keys);
    free(self->idCounts_vals);
}

static void MapStorage_saveData(MapStorage *self, MapDataBase *var1) {
    if (self->field_28191_a != NULL) {
        char *var2 = ISaveHandler_func_28113_a(self->field_28191_a, var1->field_28168_a);
        if (var2 != NULL) {
            NBTTagCompound *var3 = NBTTagCompound_create();
            var1->vtable->writeToNBT(var1, var3);
            NBTTagCompound *var4 = NBTTagCompound_create();
            NBTTagCompound_setCompoundTag(var4, "data", var3);
            FILE *var5 = fopen(var2, "wb");
            if (var5) {
                CompressedStreamTools_writeGzippedCompoundToOutputStream(var4, var5);
                fclose(var5);
            } else {
                perror(var2);
            }
            free(var2);
        }
    }
}

static void MapStorage_loadIdCounts(MapStorage *self) {
    idc_clear(self);
    if (self->field_28191_a == NULL)
        return;

    char *var1 = ISaveHandler_func_28113_a(self->field_28191_a, "idcounts");
    if (var1 != NULL) {
        FILE *f = fopen(var1, "rb");
        if (f) {

            DataStream ds = DataStream_fromFile(f);
            NBTTagCompound *var3 = CompressedStreamTools_func_1141_a(&ds);
            fclose(f);
            if (var3) {

                NBTEntry *var4 = NBTTagCompound_func_28110_c(var3);
                int n = (int)shlen(var4);

                for (int i = 0; i < n; ++i) {
                    NBTBase *var5 = var4[i].value;

                    if (NBT_getType(var5) == 2) {
                        NBTTagShort *var6 = (NBTTagShort *)var5;
                        const char *var7 = NBTBase_getKey((NBTBase *)var6);
                        int16_t var8 = var6->shortValue;
                        idc_put(self, var7, var8);
                    }
                }
                NBT_free(var3);
            }
        }
        free(var1);
    }
}

void MapStorage_init(MapStorage *self, struct ISaveHandler *var1) {
    self->field_28191_a = var1;

    self->loadedDataMap_keys = NULL;
    self->loadedDataMap_vals = NULL;
    self->loadedDataMap_size = 0;
    self->loadedDataMap_cap = 0;

    self->loadedDataList = NULL;
    self->loadedDataList_size = 0;
    self->loadedDataList_cap = 0;

    self->idCounts_keys = NULL;
    self->idCounts_vals = NULL;
    self->idCounts_size = 0;
    self->idCounts_cap = 0;

    MapStorage_loadIdCounts(self);
}

MapDataBase *MapStorage_loadData(MapStorage *self, MapDataBase *(*factory)(const char *),
                                 const char *var2) {
    MapDataBase *var3 = ldm_get(self, var2);
    if (var3 != NULL) {
        return var3;
    }

    if (self->field_28191_a != NULL) {
        char *var4 = ISaveHandler_func_28113_a(self->field_28191_a, var2);
        if (var4 != NULL) {
            FILE *f = fopen(var4, "rb");
            if (f) {

                var3 = factory(var2);

                NBTTagCompound *var6 = CompressedStreamTools_func_1138_a(f);
                fclose(f);
                if (var6) {
                    var3->vtable->readFromNBT(var3, NBTTagCompound_getCompoundTag(var6, "data"));
                }
            }
            free(var4);
        }
    }

    if (var3 != NULL) {
        ldm_put(self, var2, var3);
        ldl_add(self, var3);
    }

    return var3;
}

void MapStorage_setData(MapStorage *self, const char *var1, MapDataBase *var2) {
    if (var2 == NULL) {

        fprintf(stderr, "MapStorage_setData: Can't set null data\n");
        return;
    }

    if (ldm_containsKey(self, var1)) {

        MapDataBase *removed = ldm_remove(self, var1);
        ldl_remove(self, removed);
    }

    ldm_put(self, var1, var2);
    ldl_add(self, var2);
}

void MapStorage_saveAllData(MapStorage *self) {
    for (int var1 = 0; var1 < self->loadedDataList_size; ++var1) {
        MapDataBase *var2 = self->loadedDataList[var1];
        if (MapDataBase_isDirty(var2)) {
            MapStorage_saveData(self, var2);
            MapDataBase_setDirty(var2, 0);
        }
    }
}

int MapStorage_getUniqueDataId(MapStorage *self, const char *var1) {
    int16_t var2;
    if (!idc_get(self, var1, &var2)) {
        var2 = (int16_t)0;
    } else {
        var2 = (int16_t)(var2 + 1);
    }

    idc_put(self, var1, var2);

    if (self->field_28191_a == NULL) {
        return (int)var2;
    }

    char *var3 = ISaveHandler_func_28113_a(self->field_28191_a, "idcounts");
    if (var3 != NULL) {
        NBTTagCompound *var4 = NBTTagCompound_create();
        for (int i = 0; i < self->idCounts_size; ++i) {

            NBTTagCompound_setShort(var4, self->idCounts_keys[i], self->idCounts_vals[i]);
        }

        FILE *var9 = fopen(var3, "wb");
        if (var9) {
            DataStream ds = DataStream_fromFile(var9);
            CompressedStreamTools_func_1139_a(var4, &ds);
            fclose(var9);
        }
        NBT_free(var4);
        free(var3);
    }

    return (int)var2;
}
