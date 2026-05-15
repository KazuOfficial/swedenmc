#include "SaveFormatOld.h"
#include "CompressedStreamTools.h"
#include "SaveHandler.h"
#include "WorldInfo.h"
#include "NBTTagCompound.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "platform.h"
#include <dirent.h>
#include <unistd.h>

static const ISaveFormatVtable s_saveFormatOld_vtable = {
    SaveFormatOld_func_22178_a, SaveFormatOld_getSaveLoader,  SaveFormatOld_func_22176_b,
    SaveFormatOld_flushCache,   SaveFormatOld_func_22173_b,   SaveFormatOld_func_22172_c,
    SaveFormatOld_func_22170_a, SaveFormatOld_isOldMapFormat, SaveFormatOld_convertMapFormat};

SaveFormatOld *SaveFormatOld_create(const char *savesDir) {
    SaveFormatOld *self = (SaveFormatOld *)calloc(1, sizeof(SaveFormatOld));
    self->base.vtable = &s_saveFormatOld_vtable;
    strncpy(self->field_22180_a, savesDir, sizeof(self->field_22180_a) - 1);

    struct stat st;
    if (stat(self->field_22180_a, &st) != 0) {

        mc_mkdir(self->field_22180_a);
    }
    return self;
}

const char *SaveFormatOld_func_22178_a(ISaveFormat *self) {
    (void)self;
    return "Old Format";
}

SaveFormatComparatorList *SaveFormatOld_func_22176_b(ISaveFormat *self) {
    SaveFormatComparatorList *var1 =
        (SaveFormatComparatorList *)calloc(1, sizeof(SaveFormatComparatorList));
    SaveFormatComparatorList_init(var1);

    for (int var2 = 0; var2 < 5; ++var2) {
        char var3[16];
        snprintf(var3, sizeof(var3), "World%d", var2 + 1);
        WorldInfo *var4 = SaveFormatOld_func_22173_b(self, var3);
        if (var4 != NULL) {
            SaveFormatComparator *cmp =
                (SaveFormatComparator *)calloc(1, sizeof(SaveFormatComparator));
            SaveFormatComparator_init(cmp, var3, "", WorldInfo_getLastTimePlayed(var4),
                                      WorldInfo_getSizeOnDisk(var4), 0);
            SaveFormatComparatorList_add(var1, cmp);
            WorldInfo_free(var4);
        }
    }
    return var1;
}

void SaveFormatOld_flushCache(ISaveFormat *self) { (void)self; }

WorldInfo *SaveFormatOld_func_22173_b(ISaveFormat *self, const char *var1) {
    SaveFormatOld *sfo = (SaveFormatOld *)self;
    char var2[4096];
    snprintf(var2, sizeof(var2), "%s/%s", sfo->field_22180_a, var1);

    struct stat st;
    if (stat(var2, &st) != 0)
        return NULL;

    char var3[4096];
    snprintf(var3, sizeof(var3), "%s/level.dat", var2);
    if (stat(var3, &st) == 0) {
        FILE *f = fopen(var3, "rb");
        if (f != NULL) {
            NBTTagCompound *var4 = CompressedStreamTools_func_1138_a(f);
            fclose(f);
            if (var4 != NULL) {
                NBTTagCompound *var5 = NBTTagCompound_getCompoundTag(var4, "Data");
                WorldInfo *wi = NULL;
                if (var5 != NULL)
                    wi = WorldInfo_create_from_nbt(var5);
                if (wi != NULL) {
                    wi->nbtRoot = var4;
                    return wi;
                }
                NBT_free(var4);
            }
        }
    }

    snprintf(var3, sizeof(var3), "%s/level.dat_old", var2);
    if (stat(var3, &st) == 0) {
        FILE *f = fopen(var3, "rb");
        if (f != NULL) {
            NBTTagCompound *var4 = CompressedStreamTools_func_1138_a(f);
            fclose(f);
            if (var4 != NULL) {
                NBTTagCompound *var5 = NBTTagCompound_getCompoundTag(var4, "Data");
                WorldInfo *wi = NULL;
                if (var5 != NULL)
                    wi = WorldInfo_create_from_nbt(var5);
                if (wi != NULL) {
                    wi->nbtRoot = var4;
                    return wi;
                }
                NBT_free(var4);
            }
        }
    }

    return NULL;
}

void SaveFormatOld_func_22170_a(ISaveFormat *self, const char *var1, const char *var2) {
    SaveFormatOld *sfo = (SaveFormatOld *)self;
    char var3[4096];
    snprintf(var3, sizeof(var3), "%s/%s", sfo->field_22180_a, var1);

    struct stat st;
    if (stat(var3, &st) != 0)
        return;

    char var4path[4096];
    snprintf(var4path, sizeof(var4path), "%s/level.dat", var3);
    if (stat(var4path, &st) != 0)
        return;

    FILE *fin = fopen(var4path, "rb");
    if (fin == NULL)
        return;
    NBTTagCompound *var5 = CompressedStreamTools_func_1138_a(fin);
    fclose(fin);
    if (var5 == NULL)
        return;

    NBTTagCompound *var6 = NBTTagCompound_getCompoundTag(var5, "Data");
    if (var6 != NULL) {
        NBTTagCompound_setString(var6, "LevelName", var2);
    }

    FILE *fout = fopen(var4path, "wb");
    if (fout != NULL) {
        CompressedStreamTools_writeGzippedCompoundToOutputStream(var5, fout);
        fclose(fout);
    }
    NBT_free((NBTBase *)var5);
}

void SaveFormatOld_func_22172_c(ISaveFormat *self, const char *var1) {
    SaveFormatOld *sfo = (SaveFormatOld *)self;
    char var2[4096];
    snprintf(var2, sizeof(var2), "%s/%s", sfo->field_22180_a, var1);

    struct stat st;
    if (stat(var2, &st) != 0)
        return;

    SaveFormatOld_func_22179_a(var2);
    rmdir(var2);
}

void SaveFormatOld_func_22179_a(const char *dirPath) {
    DIR *d = opendir(dirPath);
    if (d == NULL)
        return;

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        char child[4096];
        snprintf(child, sizeof(child), "%s/%s", dirPath, ent->d_name);
        struct stat st;
        if (stat(child, &st) == 0 && S_ISDIR(st.st_mode)) {
            SaveFormatOld_func_22179_a(child);
            rmdir(child);
        } else {
            remove(child);
        }
    }
    closedir(d);
}

ISaveHandler *SaveFormatOld_getSaveLoader(ISaveFormat *self, const char *var1, int var2) {
    SaveFormatOld *sfo = (SaveFormatOld *)self;
    return (ISaveHandler *)SaveHandler_create(sfo->field_22180_a, var1, var2);
}

int SaveFormatOld_isOldMapFormat(ISaveFormat *self, const char *var1) {
    (void)self;
    (void)var1;
    return 0;
}

int SaveFormatOld_convertMapFormat(ISaveFormat *self, const char *var1, IProgressUpdate *var2) {
    (void)self;
    (void)var1;
    (void)var2;
    return 0;
}
