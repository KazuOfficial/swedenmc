#include "SaveConverterMcRegion.h"
#include "RegionFileCache.h"
#include "RegionFile.h"
#include "RegionFileChunkBuffer.h"
#include "ChunkFolderPattern.h"
#include "ChunkFilePattern.h"
#include "WorldInfo.h"
#include "MathHelper.h"
#include "ISaveHandler.h"
#include "SaveOldDir.h"
#include "JavaSort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "platform.h"
#include <dirent.h>
#include <zlib.h>

static const char *scmr_func_22178_a(ISaveFormat *self);
static SaveFormatComparatorList *scmr_func_22176_b(ISaveFormat *self);
static void scmr_flushCache(ISaveFormat *self);
static ISaveHandler *scmr_getSaveLoader(ISaveFormat *self, const char *var1, int var2);
static int scmr_isOldMapFormat(ISaveFormat *self, const char *var1);
static int scmr_convertMapFormat(ISaveFormat *self, const char *var1, IProgressUpdate *var2);

static const ISaveFormatVtable s_scmr_vtable = {
    scmr_func_22178_a,          scmr_getSaveLoader,
    scmr_func_22176_b,          scmr_flushCache,
    SaveFormatOld_func_22173_b, SaveFormatOld_func_22172_c,
    SaveFormatOld_func_22170_a, scmr_isOldMapFormat,
    scmr_convertMapFormat};

SaveConverterMcRegion *SaveConverterMcRegion_create(const char *savesDir) {
    SaveConverterMcRegion *self = (SaveConverterMcRegion *)calloc(1, sizeof(SaveConverterMcRegion));

    self->base.base.vtable = &s_scmr_vtable;
    strncpy(self->base.field_22180_a, savesDir, sizeof(self->base.field_22180_a) - 1);

    struct stat st;
    if (stat(self->base.field_22180_a, &st) != 0) {
        mc_mkdir(self->base.field_22180_a);
    }
    return self;
}

static const char *scmr_func_22178_a(ISaveFormat *self) {
    (void)self;
    return "Scaevolus\' McRegion";
}

static SaveFormatComparatorList *scmr_func_22176_b(ISaveFormat *self) {
    SaveConverterMcRegion *scmr = (SaveConverterMcRegion *)self;
    SaveFormatComparatorList *var1 =
        (SaveFormatComparatorList *)calloc(1, sizeof(SaveFormatComparatorList));
    SaveFormatComparatorList_init(var1);

    DIR *d = opendir(scmr->base.field_22180_a);
    if (d == NULL)
        return var1;

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        char var6[4096];
        snprintf(var6, sizeof(var6), "%s/%s", scmr->base.field_22180_a, ent->d_name);
        struct stat st;
        if (stat(var6, &st) != 0 || !S_ISDIR(st.st_mode))
            continue;

        const char *var7 = ent->d_name;

        WorldInfo *var8 = SaveFormatOld_func_22173_b(self, var7);
        if (var8 == NULL)
            continue;

        int var9 = WorldInfo_getSaveVersion(var8) != 19132;

        const char *var10 = WorldInfo_getWorldName(var8);
        if (var10 == NULL || MathHelper_stringNullOrLengthZero(var10)) {
            var10 = var7;
        }

        SaveFormatComparator *cmp = (SaveFormatComparator *)calloc(1, sizeof(SaveFormatComparator));
        SaveFormatComparator_init(cmp, var7, var10, WorldInfo_getLastTimePlayed(var8),
                                  WorldInfo_getSizeOnDisk(var8), var9);
        SaveFormatComparatorList_add(var1, cmp);
        WorldInfo_free(var8);
    }
    closedir(d);
    return var1;
}

static void scmr_flushCache(ISaveFormat *self) {
    (void)self;
    RegionFileCache_func_22192_a();
}

static ISaveHandler *scmr_getSaveLoader(ISaveFormat *self, const char *var1, int var2) {
    SaveConverterMcRegion *scmr = (SaveConverterMcRegion *)self;
    return (ISaveHandler *)SaveOldDir_create(scmr->base.field_22180_a, var1, var2);
}

static int scmr_isOldMapFormat(ISaveFormat *self, const char *var1) {
    WorldInfo *var2 = SaveFormatOld_func_22173_b(self, var1);
    if (var2 == NULL)
        return 0;
    int result = WorldInfo_getSaveVersion(var2) == 0;
    WorldInfo_free(var2);
    return result;
}

void DirList_init(DirList *list) {
    list->data = NULL;
    list->size = 0;
    list->cap = 0;
}

void DirList_add(DirList *list, const char *path) {
    if (list->size >= list->cap) {
        int nc = list->cap == 0 ? 8 : list->cap * 2;
        list->data = (char **)realloc(list->data, nc * sizeof(char *));
        list->cap = nc;
    }
    list->data[list->size++] = strdup(path);
}

void DirList_free(DirList *list) {
    for (int i = 0; i < list->size; i++)
        free(list->data[i]);
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->cap = 0;
}

void ChunkFileList_init(ChunkFileList *list) {
    list->data = NULL;
    list->size = 0;
    list->cap = 0;
}

void ChunkFileList_add(ChunkFileList *list, ChunkFile *cf) {
    if (list->size >= list->cap) {
        int nc = list->cap == 0 ? 8 : list->cap * 2;
        list->data = (ChunkFile **)realloc(list->data, nc * sizeof(ChunkFile *));
        list->cap = nc;
    }
    list->data[list->size++] = cf;
}

static int chunkfile_cmp(const void *a, const void *b) {
    const ChunkFile *ca = *(const ChunkFile *const *)a;
    const ChunkFile *cb = *(const ChunkFile *const *)b;
    return ChunkFile_func_22322_a(ca, cb);
}

void ChunkFileList_sort(ChunkFileList *list) {
    if (list->size > 1)
        Arrays_sort((void **)list->data, list->size, chunkfile_cmp);
}

void ChunkFileList_free(ChunkFileList *list) {
    for (int i = 0; i < list->size; i++)
        ChunkFile_destroy(list->data[i]);
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->cap = 0;
}

static void func_22183_a(const char *var1, ChunkFileList *var2, DirList *var3) {
    ChunkFolderPattern var4;
    ChunkFolderPattern_init();
    ChunkFolderPattern_construct(&var4, NULL);

    DIR *d = opendir(var1);
    if (d == NULL)
        return;

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        char var10[4096];
        snprintf(var10, sizeof(var10), "%s/%s", var1, ent->d_name);
        struct stat st;
        if (stat(var10, &st) != 0 || !S_ISDIR(st.st_mode))
            continue;
        if (!ChunkFolderPattern_accept(&var4, var10))
            continue;

        DirList_add(var3, var10);

        DIR *d2 = opendir(var10);
        if (d2 == NULL)
            continue;
        struct dirent *ent2;
        while ((ent2 = readdir(d2)) != NULL) {
            if (strcmp(ent2->d_name, ".") == 0 || strcmp(ent2->d_name, "..") == 0)
                continue;

            char var15[4096];
            snprintf(var15, sizeof(var15), "%s/%s", var10, ent2->d_name);
            struct stat st2;
            if (stat(var15, &st2) != 0 || !S_ISDIR(st2.st_mode))
                continue;
            if (!ChunkFolderPattern_accept(&var4, var15))
                continue;

            DIR *d3 = opendir(var15);
            if (d3 == NULL)
                continue;
            struct dirent *ent3;
            while ((ent3 = readdir(d3)) != NULL) {
                if (strcmp(ent3->d_name, ".") == 0 || strcmp(ent3->d_name, "..") == 0)
                    continue;
                if (!ChunkFilePattern_accept(ent3->d_name))
                    continue;

                char var20[4096];
                snprintf(var20, sizeof(var20), "%s/%s", var15, ent3->d_name);
                ChunkFile *cf = ChunkFile_create(var20);
                if (cf != NULL)
                    ChunkFileList_add(var2, cf);
            }
            closedir(d3);
        }
        closedir(d2);
    }
    closedir(d);
}

static void func_22181_a(const char *var1, ChunkFileList *var2, int var3, int var4,
                         IProgressUpdate *var5) {

    ChunkFileList_sort(var2);

    uint8_t var6[4096];

    for (int i = 0; i < var2->size; i++) {
        ChunkFile *var8 = var2->data[i];
        int var9 = ChunkFile_func_22323_b(var8);
        int var10 = ChunkFile_func_22321_c(var8);

        RegionFile *var11 = RegionFileCache_func_22193_a(var1, var9, var10);
        if (!RegionFile_isChunkSaved(var11, var9 & 31, var10 & 31)) {

            gzFile var12 = gzopen(ChunkFile_func_22324_a(var8), "rb");
            if (var12 != NULL) {
                RegionFileChunkBuffer *var13 =
                    RegionFile_getChunkDataOutputStream(var11, var9 & 31, var10 & 31);
                while (1) {
                    int var17 = gzread(var12, var6, sizeof(var6));
                    if (var17 <= 0) {

                        RegionFileChunkBuffer_close(var13);
                        RegionFileChunkBuffer_destroy(var13);
                        gzclose(var12);
                        break;
                    }
                    RegionFileChunkBuffer_writeBytes(var13, var6, 0, var17);
                }
            }
        }

        ++var3;
        int var16 = (int)round(100.0 * (double)var3 / (double)var4);
        var5->setLoadingProgress(var5, var16);
    }

    RegionFileCache_func_22192_a();
}

static void func_22182_a(DirList *var1, int var2, int var3, IProgressUpdate *var4) {
    for (int i = 0; i < var1->size; i++) {
        const char *var6 = var1->data[i];
        SaveFormatOld_func_22179_a(var6);
        rmdir(var6);
        ++var2;
        int var8 = (int)round(100.0 * (double)var2 / (double)var3);
        var4->setLoadingProgress(var4, var8);
    }
}

static int scmr_convertMapFormat(ISaveFormat *self, const char *var1, IProgressUpdate *var2) {
    SaveConverterMcRegion *scmr = (SaveConverterMcRegion *)self;

    var2->setLoadingProgress(var2, 0);

    ChunkFileList var3;
    ChunkFileList_init(&var3);
    DirList var4;
    DirList_init(&var4);
    ChunkFileList var5;
    ChunkFileList_init(&var5);
    DirList var6;
    DirList_init(&var6);

    char var7[4096];
    snprintf(var7, sizeof(var7), "%s/%s", scmr->base.field_22180_a, var1);

    char var8[4096];
    snprintf(var8, sizeof(var8), "%s/DIM-1", var7);

    printf("Scanning folders...\n");
    func_22183_a(var7, &var3, &var4);

    struct stat st;
    int var8exists = (stat(var8, &st) == 0 && S_ISDIR(st.st_mode));
    if (var8exists) {
        func_22183_a(var8, &var5, &var6);
    }

    int var9 = var3.size + var5.size + var4.size + var6.size;
    printf("Total conversion count is %d\n", var9);

    func_22181_a(var7, &var3, 0, var9, var2);
    func_22181_a(var8, &var5, var3.size, var9, var2);

    WorldInfo *var10 = SaveFormatOld_func_22173_b(self, var1);
    WorldInfo_setSaveVersion(var10, 19132);
    ISaveHandler *var11 = scmr_getSaveLoader(self, var1, 0);
    ISaveHandler_saveWorldInfo(var11, var10);
    WorldInfo_free(var10);

    func_22182_a(&var4, var3.size + var5.size, var9, var2);
    if (var8exists) {
        func_22182_a(&var6, var3.size + var5.size + var4.size, var9, var2);
    }

    ChunkFileList_free(&var3);
    DirList_free(&var4);
    ChunkFileList_free(&var5);
    DirList_free(&var6);

    return 1;
}
