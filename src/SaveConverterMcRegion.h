#ifndef SAVE_CONVERTER_MC_REGION_H
#define SAVE_CONVERTER_MC_REGION_H

#include "SaveFormatOld.h"
#include "ChunkFile.h"

typedef struct SaveConverterMcRegion {
    SaveFormatOld base;
} SaveConverterMcRegion;

SaveConverterMcRegion *SaveConverterMcRegion_create(const char *savesDir);

typedef struct {
    char **data;
    int size;
    int cap;
} DirList;

void DirList_init(DirList *list);
void DirList_add(DirList *list, const char *path);
void DirList_free(DirList *list);

typedef struct {
    ChunkFile **data;
    int size;
    int cap;
} ChunkFileList;

void ChunkFileList_init(ChunkFileList *list);
void ChunkFileList_add(ChunkFileList *list, ChunkFile *cf);
void ChunkFileList_sort(ChunkFileList *list);
void ChunkFileList_free(ChunkFileList *list);

#endif
