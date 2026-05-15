#ifndef REGION_FILE_CACHE_H
#define REGION_FILE_CACHE_H

#include "RegionFile.h"
#include "DataStream.h"

RegionFileChunkBuffer *RegionFileCache_getChunkOutputStream(const char *worldDir, int chunkX,
                                                            int chunkZ);

DataStream RegionFileCache_getChunkInputStream(const char *worldDir, int chunkX, int chunkZ);

void RegionFileCache_func_22192_a(void);

RegionFile *RegionFileCache_func_22193_a(const char *worldDir, int chunkX, int chunkZ);

int RegionFileCache_getSizeDelta(const char *worldDir, int chunkX, int chunkZ);

#endif
