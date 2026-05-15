#include "RegionFileCache.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "platform.h"

#define RFC_MAP_SIZE 512

typedef struct {
    char *path;
    RegionFile *rf;
} RFCEntry;

static RFCEntry rfc_map[RFC_MAP_SIZE];
static int rfc_count = 0;

static unsigned int rfc_hash(const char *s) {
    unsigned int h = 5381;
    while (*s)
        h = h * 33 + (unsigned char)*s++;
    return h;
}

static RegionFile *rfc_get(const char *path) {
    unsigned int idx = rfc_hash(path) & (RFC_MAP_SIZE - 1);
    for (int i = 0; i < RFC_MAP_SIZE; ++i) {
        int slot = (idx + i) & (RFC_MAP_SIZE - 1);
        if (rfc_map[slot].path == NULL)
            return NULL;
        if (strcmp(rfc_map[slot].path, path) == 0)
            return rfc_map[slot].rf;
    }
    return NULL;
}

static void rfc_put(const char *path, RegionFile *rf) {
    unsigned int idx = rfc_hash(path) & (RFC_MAP_SIZE - 1);
    for (int i = 0; i < RFC_MAP_SIZE; ++i) {
        int slot = (idx + i) & (RFC_MAP_SIZE - 1);
        if (rfc_map[slot].path == NULL) {
            rfc_map[slot].path = strdup(path);
            rfc_map[slot].rf = rf;
            ++rfc_count;
            return;
        }
        if (strcmp(rfc_map[slot].path, path) == 0) {
            rfc_map[slot].rf = rf;
            return;
        }
    }
}

static RegionFile *rfc_func_22193_a(const char *worldDir, int chunkX, int chunkZ) {

    char path[4096];
    int rx = chunkX >> 5;
    int rz = chunkZ >> 5;
    snprintf(path, sizeof(path), "%s/region/r.%d.%d.mcr", worldDir, rx, rz);

    RegionFile *rf = rfc_get(path);
    if (rf != NULL)
        return rf;

    if (rfc_count >= 256) {
        RegionFileCache_func_22192_a();
    }

    char regionDir[4096];
    snprintf(regionDir, sizeof(regionDir), "%s/region", worldDir);
    mc_mkdir(regionDir);

    rf = RegionFile_create(path);
    rfc_put(path, rf);
    return rf;
}

RegionFileChunkBuffer *RegionFileCache_getChunkOutputStream(const char *worldDir, int chunkX,
                                                            int chunkZ) {
    RegionFile *rf = rfc_func_22193_a(worldDir, chunkX, chunkZ);
    if (rf == NULL)
        return NULL;
    return RegionFile_getChunkDataOutputStream(rf, chunkX & 31, chunkZ & 31);
}

DataStream RegionFileCache_getChunkInputStream(const char *worldDir, int chunkX, int chunkZ) {
    RegionFile *rf = rfc_func_22193_a(worldDir, chunkX, chunkZ);
    if (rf == NULL) {
        DataStream ds;
        memset(&ds, 0, sizeof(ds));
        ds.error = 1;
        return ds;
    }
    return RegionFile_getChunkDataInputStream(rf, chunkX & 31, chunkZ & 31);
}

RegionFile *RegionFileCache_func_22193_a(const char *worldDir, int chunkX, int chunkZ) {
    return rfc_func_22193_a(worldDir, chunkX, chunkZ);
}

int RegionFileCache_getSizeDelta(const char *worldDir, int chunkX, int chunkZ) {
    RegionFile *rf = rfc_func_22193_a(worldDir, chunkX, chunkZ);
    if (rf == NULL)
        return 0;
    return RegionFile_func_22209_a(rf);
}

void RegionFileCache_func_22192_a(void) {
    for (int i = 0; i < RFC_MAP_SIZE; ++i) {
        if (rfc_map[i].path != NULL) {
            RegionFile_close(rfc_map[i].rf);
            free(rfc_map[i].path);
            rfc_map[i].path = NULL;
            rfc_map[i].rf = NULL;
        }
    }
    rfc_count = 0;
}
