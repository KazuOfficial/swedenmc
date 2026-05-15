#include "CompressedStreamTools.h"
#include "NBTBase.h"
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

NBTTagCompound *CompressedStreamTools_func_1141_a(DataStream *in) {
    NBTBase *var1 = NBTBase_readTag(in);
    if (var1 == NULL)
        return NULL;
    if (NBT_getType(var1) == 10) {
        return (NBTTagCompound *)var1;
    }

    NBT_free(var1);
    return NULL;
}

void CompressedStreamTools_func_1139_a(NBTTagCompound *tag, DataStream *out) {
    NBTBase_writeTag((NBTBase *)tag, out);
}

NBTTagCompound *CompressedStreamTools_func_1138_a(FILE *in) {

    fseek(in, 0, SEEK_END);
    long fileLen = ftell(in);
    fseek(in, 0, SEEK_SET);
    if (fileLen <= 0)
        return NULL;

    uint8_t *compBuf = (uint8_t *)malloc((size_t)fileLen);
    if ((long)fread(compBuf, 1, (size_t)fileLen, in) != fileLen) {
        free(compBuf);
        return NULL;
    }

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)fileLen;
    if (inflateInit2(&strm, 15 + 16) != Z_OK) {
        free(compBuf);
        return NULL;
    }

    int cap = (int)fileLen * 4 + 1024;
    uint8_t *decompBuf = (uint8_t *)malloc(cap);
    strm.next_out = decompBuf;
    strm.avail_out = (uInt)cap;
    int ret;
    while ((ret = inflate(&strm, Z_FINISH)) == Z_BUF_ERROR) {
        int used = cap - (int)strm.avail_out;
        cap *= 2;
        decompBuf = (uint8_t *)realloc(decompBuf, cap);
        strm.next_out = decompBuf + used;
        strm.avail_out = (uInt)(cap - used);
    }
    int decompLen = cap - (int)strm.avail_out;
    inflateEnd(&strm);
    free(compBuf);

    if (ret != Z_STREAM_END) {
        free(decompBuf);
        return NULL;
    }

    DataStream ds = DataStream_fromBuffer(decompBuf, (size_t)decompLen);
    NBTTagCompound *result = CompressedStreamTools_func_1141_a(&ds);
    free(decompBuf);
    return result;
}

void CompressedStreamTools_writeGzippedCompoundToOutputStream(NBTTagCompound *tag, FILE *out) {

    int cap = 65536;
    uint8_t *rawBuf = (uint8_t *)malloc(cap);
    DataStream ds = DataStream_fromBuffer(rawBuf, (size_t)cap);
    CompressedStreamTools_func_1139_a(tag, &ds);

    uLongf compBound = compressBound((uLong)ds.pos) + 64;
    uint8_t *compBuf = (uint8_t *)malloc((size_t)compBound);

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.next_in = rawBuf;
    strm.avail_in = (uInt)ds.pos;

    deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compBound;
    deflate(&strm, Z_FINISH);
    uLong compLen = compBound - strm.avail_out;
    deflateEnd(&strm);

    fwrite(compBuf, 1, (size_t)compLen, out);

    free(rawBuf);
    free(compBuf);
}
