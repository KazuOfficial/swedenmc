#include "RegionFile.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zlib.h>

static const uint8_t emptySector[REGION_SECTOR_SIZE];

static int read_int_be(FILE *f) {
    uint8_t b[4];
    if (fread(b, 1, 4, f) != 4)
        return 0;
    return (int)(((unsigned int)b[0] << 24) | ((unsigned int)b[1] << 16) |
                 ((unsigned int)b[2] << 8) | (unsigned int)b[3]);
}

static void write_int_be(FILE *f, int v) {
    uint8_t b[4];
    b[0] = (uint8_t)(v >> 24);
    b[1] = (uint8_t)(v >> 16);
    b[2] = (uint8_t)(v >> 8);
    b[3] = (uint8_t)(v);
    fwrite(b, 1, 4, f);
}

static int rf_getOffset(RegionFile *self, int x, int z) { return self->offsets[x + z * 32]; }

static void rf_setOffset(RegionFile *self, int x, int z, int offset) {
    self->offsets[x + z * 32] = offset;
    fseek(self->dataFile, (long)(x + z * 32) * 4, SEEK_SET);
    write_int_be(self->dataFile, offset);
}

static void rf_setTimestamp(RegionFile *self, int x, int z, int value) {
    self->field_22217_e[x + z * 32] = value;
    fseek(self->dataFile, (long)(REGION_SECTOR_SIZE + (x + z * 32) * 4), SEEK_SET);
    write_int_be(self->dataFile, value);
}

static void rf_ensureSectors(RegionFile *self, int needed) {
    if (needed > self->sectorFreeCount) {
        self->sectorFree = (uint8_t *)realloc(self->sectorFree, needed);
        memset(self->sectorFree + self->sectorFreeCount, 1, needed - self->sectorFreeCount);
        self->sectorFreeCount = needed;
    }
}

RegionFile *RegionFile_create(const char *path) {
    RegionFile *self = (RegionFile *)calloc(1, sizeof(RegionFile));

    self->dataFile = fopen(path, "r+b");
    if (self->dataFile == NULL) {

        self->dataFile = fopen(path, "w+b");
        if (self->dataFile == NULL) {
            free(self);
            return NULL;
        }
    }

    fseek(self->dataFile, 0, SEEK_END);
    long fileSize = ftell(self->dataFile);
    fseek(self->dataFile, 0, SEEK_SET);

    if (fileSize < REGION_SECTOR_SIZE) {

        fwrite(emptySector, 1, REGION_SECTOR_SIZE, self->dataFile);
        fwrite(emptySector, 1, REGION_SECTOR_SIZE, self->dataFile);
        self->sizeDelta += 8192;
        fileSize = REGION_SECTOR_SIZE * REGION_HEADER_SECTORS;
    }

    if ((fileSize & (REGION_SECTOR_SIZE - 1)) != 0) {
        long padded = (fileSize + REGION_SECTOR_SIZE - 1) & ~(long)(REGION_SECTOR_SIZE - 1);
        fseek(self->dataFile, fileSize, SEEK_SET);
        for (long i = fileSize; i < padded; ++i) {
            fputc(0, self->dataFile);
        }
        fileSize = padded;
    }

    int numSectors = (int)(fileSize / REGION_SECTOR_SIZE);
    self->sectorFree = (uint8_t *)malloc(numSectors);
    self->sectorFreeCount = numSectors;
    memset(self->sectorFree, 1, numSectors);

    self->sectorFree[0] = 0;
    if (numSectors > 1)
        self->sectorFree[1] = 0;

    fseek(self->dataFile, 0, SEEK_SET);
    for (int i = 0; i < 1024; ++i) {
        self->offsets[i] = read_int_be(self->dataFile);
    }

    for (int i = 0; i < 1024; ++i) {
        self->field_22217_e[i] = read_int_be(self->dataFile);
    }

    for (int i = 0; i < 1024; ++i) {
        int offset = self->offsets[i];
        if (offset != 0) {
            int sectorNumber = offset >> 8;
            int sectorCount = offset & 0xFF;
            if (sectorNumber + sectorCount <= self->sectorFreeCount) {
                for (int s = sectorNumber; s < sectorNumber + sectorCount; ++s) {
                    self->sectorFree[s] = 0;
                }
            }
        }
    }

    return self;
}

void RegionFile_close(RegionFile *self) {
    if (self->dataFile != NULL) {
        fclose(self->dataFile);
        self->dataFile = NULL;
    }
    free(self->sectorFree);
    free(self);
}

int RegionFile_isChunkSaved(RegionFile *self, int x, int z) {
    return rf_getOffset(self, x, z) != 0;
}

DataStream RegionFile_getChunkDataInputStream(RegionFile *self, int x, int z) {
    DataStream ds;
    memset(&ds, 0, sizeof(ds));

    if (x < 0 || x >= 32 || z < 0 || z >= 32) {
        ds.error = 1;
        return ds;
    }

    int offset = rf_getOffset(self, x, z);
    if (offset == 0) {

        ds.error = 1;
        return ds;
    }

    int sectorNumber = (offset >> 8) & 0xFFFFFF;
    int numSectors = offset & 0xFF;

    if (sectorNumber + numSectors > self->sectorFreeCount) {
        ds.error = 1;
        return ds;
    }

    fseek(self->dataFile, (long)sectorNumber * REGION_SECTOR_SIZE, SEEK_SET);

    int length = read_int_be(self->dataFile);

    if (length > REGION_SECTOR_SIZE * numSectors) {
        ds.error = 1;
        return ds;
    }

    int version = fgetc(self->dataFile);

    int compLen = length - 1;
    uint8_t *compBuf = (uint8_t *)malloc(compLen);
    if ((int)fread(compBuf, 1, compLen, self->dataFile) != compLen) {
        free(compBuf);
        ds.error = 1;
        return ds;
    }

    uLongf decompLen = (uLongf)compLen * 4 + 1024;
    uint8_t *decompBuf = NULL;
    int ret;

    if (version == 1) {

        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        strm.next_in = compBuf;
        strm.avail_in = (uInt)compLen;

        if (inflateInit2(&strm, 15 + 16) != Z_OK) {
            free(compBuf);
            ds.error = 1;
            return ds;
        }
        int cap = compLen * 4 + 1024;
        decompBuf = (uint8_t *)malloc(cap);
        strm.next_out = decompBuf;
        strm.avail_out = (uInt)cap;
        while ((ret = inflate(&strm, Z_FINISH)) == Z_BUF_ERROR) {
            int used = cap - strm.avail_out;
            cap *= 2;
            decompBuf = (uint8_t *)realloc(decompBuf, cap);
            strm.next_out = decompBuf + used;
            strm.avail_out = (uInt)(cap - used);
        }
        decompLen = cap - strm.avail_out;
        inflateEnd(&strm);
        if (ret != Z_STREAM_END) {
            free(compBuf);
            free(decompBuf);
            ds.error = 1;
            return ds;
        }
    } else if (version == 2) {

        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        strm.next_in = compBuf;
        strm.avail_in = (uInt)compLen;
        if (inflateInit(&strm) != Z_OK) {
            free(compBuf);
            ds.error = 1;
            return ds;
        }
        int cap = compLen * 4 + 1024;
        decompBuf = (uint8_t *)malloc(cap);
        strm.next_out = decompBuf;
        strm.avail_out = (uInt)cap;
        while ((ret = inflate(&strm, Z_FINISH)) == Z_BUF_ERROR) {
            int used = cap - strm.avail_out;
            cap *= 2;
            decompBuf = (uint8_t *)realloc(decompBuf, cap);
            strm.next_out = decompBuf + used;
            strm.avail_out = (uInt)(cap - used);
        }
        decompLen = cap - strm.avail_out;
        inflateEnd(&strm);
        if (ret != Z_STREAM_END) {
            free(compBuf);
            free(decompBuf);
            ds.error = 1;
            return ds;
        }
    } else {

        free(compBuf);
        ds.error = 1;
        return ds;
    }

    free(compBuf);
    ds = DataStream_fromBuffer(decompBuf, (size_t)decompLen);
    return ds;
}

RegionFileChunkBuffer *RegionFile_getChunkDataOutputStream(RegionFile *self, int x, int z) {

    if (x < 0 || x >= 32 || z < 0 || z >= 32)
        return NULL;
    return RegionFileChunkBuffer_create(self, x, z);
}

void RegionFile_write(RegionFile *self, int x, int z, const uint8_t *data, int length) {

    uLongf compBound = compressBound((uLong)length);
    uint8_t *compBuf = (uint8_t *)malloc(compBound);
    uLongf compLen = compBound;
    compress2(compBuf, &compLen, data, (uLong)length, Z_BEST_SPEED);

    int chunkDataLen = (int)compLen + 1;
    int sectorsNeeded = ((int)compLen + 5) / REGION_SECTOR_SIZE + 1;

    if (sectorsNeeded >= 256) {
        free(compBuf);
        return;
    }

    int offset = rf_getOffset(self, x, z);
    int sectorNumber = (offset >> 8) & 0xFFFFFF;
    int sectorCount = offset & 0xFF;

    if (sectorNumber != 0 && sectorCount == sectorsNeeded) {

        fseek(self->dataFile, (long)sectorNumber * REGION_SECTOR_SIZE, SEEK_SET);
        write_int_be(self->dataFile, chunkDataLen);
        fputc(2, self->dataFile);
        fwrite(compBuf, 1, compLen, self->dataFile);
    } else {

        for (int i = sectorNumber; i < sectorNumber + sectorCount; ++i) {
            if (i < self->sectorFreeCount)
                self->sectorFree[i] = 1;
        }

        int runStart = -1, runLen = 0;
        for (int i = 0; i < self->sectorFreeCount; ++i) {
            if (self->sectorFree[i]) {
                if (runLen == 0)
                    runStart = i;
                ++runLen;
                if (runLen >= sectorsNeeded)
                    break;
            } else {
                runStart = -1;
                runLen = 0;
            }
        }

        if (runLen >= sectorsNeeded) {

            sectorNumber = runStart;
            rf_setOffset(self, x, z, (sectorNumber << 8) | (sectorsNeeded & 0xFF));
            for (int i = sectorNumber; i < sectorNumber + sectorsNeeded; ++i) {
                self->sectorFree[i] = 0;
            }
            fseek(self->dataFile, (long)sectorNumber * REGION_SECTOR_SIZE, SEEK_SET);
            write_int_be(self->dataFile, chunkDataLen);
            fputc(2, self->dataFile);
            fwrite(compBuf, 1, compLen, self->dataFile);
        } else {

            fseek(self->dataFile, 0, SEEK_END);
            long pos = ftell(self->dataFile);
            sectorNumber = (int)(pos / REGION_SECTOR_SIZE);

            rf_ensureSectors(self, sectorNumber + sectorsNeeded);

            write_int_be(self->dataFile, chunkDataLen);
            fputc(2, self->dataFile);
            fwrite(compBuf, 1, compLen, self->dataFile);

            long written = 4 + 1 + (long)compLen;
            long padTo = (long)sectorsNeeded * REGION_SECTOR_SIZE;
            for (long i = written; i < padTo; ++i) {
                fputc(0, self->dataFile);
            }

            self->sizeDelta += 4096 * sectorsNeeded;
            rf_setOffset(self, x, z, (sectorNumber << 8) | (sectorsNeeded & 0xFF));
            for (int i = sectorNumber; i < sectorNumber + sectorsNeeded; ++i) {
                self->sectorFree[i] = 0;
            }
        }
    }

    rf_setTimestamp(self, x, z, (int)(time(NULL)));

    fflush(self->dataFile);

    free(compBuf);
}

int RegionFile_func_22209_a(RegionFile *self) {
    int var1 = self->sizeDelta;
    self->sizeDelta = 0;
    return var1;
}
