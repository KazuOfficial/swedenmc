#ifndef METADATA_CHUNK_BLOCK_H
#define METADATA_CHUNK_BLOCK_H

#include "EnumSkyBlock.h"

struct World;

typedef struct MetadataChunkBlock {

    EnumSkyBlock field_1299_a;

    int field_1298_b;

    int field_1304_c;

    int field_1303_d;

    int field_1302_e;

    int field_1301_f;

    int field_1300_g;
} MetadataChunkBlock;

void MetadataChunkBlock_init(MetadataChunkBlock *self, EnumSkyBlock var1, int var2, int var3,
                             int var4, int var5, int var6, int var7);

void MetadataChunkBlock_func_4127_a(MetadataChunkBlock *self, struct World *var1);

int MetadataChunkBlock_func_866_a(MetadataChunkBlock *self, int var1, int var2, int var3, int var4,
                                  int var5, int var6);

#endif
