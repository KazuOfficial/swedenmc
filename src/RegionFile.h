#ifndef REGION_FILE_H
#define REGION_FILE_H

#include <stdio.h>
#include <stdint.h>
#include "DataStream.h"
#include "RegionFileChunkBuffer.h"

#define REGION_SECTOR_SIZE 4096

#define REGION_HEADER_SECTORS 2

typedef struct RegionFile {
    FILE *dataFile;
    int offsets[1024];
    int field_22217_e[1024];
    uint8_t *sectorFree;
    int sectorFreeCount;
    int sizeDelta;
} RegionFile;

RegionFile *RegionFile_create(const char *path);

void RegionFile_close(RegionFile *self);

int RegionFile_isChunkSaved(RegionFile *self, int x, int z);

DataStream RegionFile_getChunkDataInputStream(RegionFile *self, int x, int z);

RegionFileChunkBuffer *RegionFile_getChunkDataOutputStream(RegionFile *self, int x, int z);

void RegionFile_write(RegionFile *self, int x, int z, const uint8_t *data, int length);

int RegionFile_func_22209_a(RegionFile *self);

#endif
