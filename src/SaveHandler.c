#include "SaveHandler.h"
#include "ChunkLoader.h"
#include "IChunkLoader.h"
#include "WorldInfo.h"
#include "CompressedStreamTools.h"
#include "NBTTagCompound.h"
#include "WorldProvider.h"
#include "EntityPlayer.h"
#include "MinecraftException.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include <time.h>

static WorldInfo *sh_loadWorldInfo(ISaveHandler *iself) {
    SaveHandler *self = (SaveHandler *)iself;
    char path[4096];

    snprintf(path, sizeof(path), "%s/level.dat", self->saveDirectory);
    FILE *f = fopen(path, "rb");
    if (f != NULL) {
        NBTTagCompound *var2 = CompressedStreamTools_func_1138_a(f);
        fclose(f);
        if (var2 != NULL) {
            NBTTagCompound *var3 = NBTTagCompound_getCompoundTag(var2, "Data");
            WorldInfo *wi = WorldInfo_create_from_nbt(var3);
            wi->nbtRoot = var2;
            return wi;
        }
    }

    snprintf(path, sizeof(path), "%s/level.dat_old", self->saveDirectory);
    f = fopen(path, "rb");
    if (f != NULL) {
        NBTTagCompound *var2 = CompressedStreamTools_func_1138_a(f);
        fclose(f);
        if (var2 != NULL) {
            NBTTagCompound *var3 = NBTTagCompound_getCompoundTag(var2, "Data");
            WorldInfo *wi = WorldInfo_create_from_nbt(var3);
            wi->nbtRoot = var2;
            return wi;
        }
    }

    return NULL;
}

static void sh_func_22150_b(ISaveHandler *iself) {
    SaveHandler *self = (SaveHandler *)iself;
    char lockPath[4096];
    snprintf(lockPath, sizeof(lockPath), "%s/session.lock", self->saveDirectory);
    FILE *f = fopen(lockPath, "rb");
    if (f == NULL) {

        fprintf(stderr, "Failed to check session lock, aborting\n");
        exit(1);
    }
    uint8_t buf[8];
    int ok = (fread(buf, 1, 8, f) == 8);
    fclose(f);
    if (!ok) {
        fprintf(stderr, "Failed to check session lock, aborting\n");
        exit(1);
    }

    int64_t stored = ((int64_t)buf[0] << 56) | ((int64_t)buf[1] << 48) | ((int64_t)buf[2] << 40) |
                     ((int64_t)buf[3] << 32) | ((int64_t)buf[4] << 24) | ((int64_t)buf[5] << 16) |
                     ((int64_t)buf[6] << 8) | (int64_t)buf[7];
    if (stored != self->now) {

        fprintf(stderr, "The save is being accessed from another location, aborting\n");
        exit(1);
    }
}

static IChunkLoader *sh_getChunkLoader(ISaveHandler *iself, WorldProvider *provider) {
    SaveHandler *self = (SaveHandler *)iself;

    if (provider != NULL && provider->isHellWorld) {
        char dimPath[4096];
        snprintf(dimPath, sizeof(dimPath), "%s/DIM-1", self->saveDirectory);
        mc_mkdir(dimPath);
        return (IChunkLoader *)ChunkLoader_create(dimPath, 1);
    }
    return (IChunkLoader *)ChunkLoader_create(self->saveDirectory, 1);
}

static void sh_saveWorldInfoAndPlayer(ISaveHandler *iself, WorldInfo *info,
                                      struct EntityPlayer **players, int playerCount) {
    SaveHandler *self = (SaveHandler *)iself;

    NBTTagCompound *var3 = WorldInfo_getNBTTagCompoundWithPlayer(info, players, playerCount);
    NBTTagCompound *var4 = NBTTagCompound_create();
    NBTTagCompound_setTag(var4, "Data", (NBTBase *)var3);

    char newPath[4096], oldPath[4096], datPath[4096];
    snprintf(newPath, sizeof(newPath), "%s/level.dat_new", self->saveDirectory);
    snprintf(oldPath, sizeof(oldPath), "%s/level.dat_old", self->saveDirectory);
    snprintf(datPath, sizeof(datPath), "%s/level.dat", self->saveDirectory);

    FILE *f = fopen(newPath, "wb");
    if (f == NULL)
        return;
    CompressedStreamTools_writeGzippedCompoundToOutputStream(var4, f);
    fclose(f);
    NBT_free(var4);

    remove(oldPath);
    rename(datPath, oldPath);

    remove(datPath);
    rename(newPath, datPath);

    remove(newPath);
}

static void sh_saveWorldInfo(ISaveHandler *iself, WorldInfo *info) {
    SaveHandler *self = (SaveHandler *)iself;

    NBTTagCompound *var2 = WorldInfo_getNBTTagCompound(info);
    NBTTagCompound *var3 = NBTTagCompound_create();
    NBTTagCompound_setTag(var3, "Data", (NBTBase *)var2);

    char newPath[4096], oldPath[4096], datPath[4096];
    snprintf(newPath, sizeof(newPath), "%s/level.dat_new", self->saveDirectory);
    snprintf(oldPath, sizeof(oldPath), "%s/level.dat_old", self->saveDirectory);
    snprintf(datPath, sizeof(datPath), "%s/level.dat", self->saveDirectory);

    FILE *f = fopen(newPath, "wb");
    if (f == NULL)
        return;
    CompressedStreamTools_writeGzippedCompoundToOutputStream(var3, f);
    fclose(f);
    NBT_free(var3);

    remove(oldPath);
    rename(datPath, oldPath);
    remove(datPath);
    rename(newPath, datPath);
    remove(newPath);
}

static char *sh_func_28113_a(ISaveHandler *iself, const char *var1) {
    SaveHandler *self = (SaveHandler *)iself;
    size_t len = strlen(self->field_28114_d) + 1 + strlen(var1) + 4 + 1;
    char *path = (char *)malloc(len);
    snprintf(path, len, "%s/%s.dat", self->field_28114_d, var1);
    return path;
}

static void sh_destroy(ISaveHandler *self) { SaveHandler_destroy((SaveHandler *)self); }

const ISaveHandlerVtable SaveHandler_vtable = {
    .loadWorldInfo = sh_loadWorldInfo,
    .func_22150_b = sh_func_22150_b,
    .getChunkLoader = sh_getChunkLoader,
    .saveWorldInfoAndPlayer = sh_saveWorldInfoAndPlayer,
    .saveWorldInfo = sh_saveWorldInfo,
    .func_28113_a = sh_func_28113_a,
    .destroy = sh_destroy,
};

SaveHandler *SaveHandler_create(const char *savesDir, const char *worldName, int createLock) {
    SaveHandler *self = (SaveHandler *)calloc(1, sizeof(SaveHandler));
    self->base.vtable = &SaveHandler_vtable;

    size_t sdLen = strlen(savesDir) + 1 + strlen(worldName) + 1;
    self->saveDirectory = (char *)malloc(sdLen);
    snprintf(self->saveDirectory, sdLen, "%s/%s", savesDir, worldName);

    mc_mkdir(self->saveDirectory);

    size_t pdLen = sdLen + 8;
    self->playersDirectory = (char *)malloc(pdLen);
    snprintf(self->playersDirectory, pdLen, "%s/players", self->saveDirectory);

    size_t ddLen = sdLen + 6;
    self->field_28114_d = (char *)malloc(ddLen);
    snprintf(self->field_28114_d, ddLen, "%s/data", self->saveDirectory);
    mc_mkdir(self->field_28114_d);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    self->now = (int64_t)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;

    if (createLock) {
        mc_mkdir(self->playersDirectory);
    }

    {
        char lockPath[4096];
        snprintf(lockPath, sizeof(lockPath), "%s/session.lock", self->saveDirectory);
        FILE *f = fopen(lockPath, "wb");
        if (f == NULL) {

            fprintf(stderr, "Failed to check session lock, aborting\n");
            exit(1);
        }
        int64_t n = self->now;
        uint8_t buf[8];
        buf[0] = (uint8_t)(n >> 56);
        buf[1] = (uint8_t)(n >> 48);
        buf[2] = (uint8_t)(n >> 40);
        buf[3] = (uint8_t)(n >> 32);
        buf[4] = (uint8_t)(n >> 24);
        buf[5] = (uint8_t)(n >> 16);
        buf[6] = (uint8_t)(n >> 8);
        buf[7] = (uint8_t)(n);
        fwrite(buf, 1, 8, f);
        fclose(f);
    }

    return self;
}

const char *SaveHandler_getSaveDirectory(SaveHandler *self) { return self->saveDirectory; }

void SaveHandler_destroy(SaveHandler *self) {
    free(self->saveDirectory);
    free(self->playersDirectory);
    free(self->field_28114_d);
    free(self);
}
