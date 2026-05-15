#ifndef CHUNK_PROVIDER_SKY_H
#define CHUNK_PROVIDER_SKY_H

#include "IChunkProvider.h"
#include "World.h"
#include "Chunk.h"
#include "JavaRandom.h"
#include "NoiseGeneratorOctaves.h"
#include "BiomeGenBase.h"
#include "MapGenCaves.h"
#include "WorldChunkManager.h"

typedef struct ChunkProviderSky {
    IChunkProvider base;

    JavaRandom field_28087_j;
    NoiseGeneratorOctaves field_28086_k;
    NoiseGeneratorOctaves field_28085_l;
    NoiseGeneratorOctaves field_28084_m;
    NoiseGeneratorOctaves field_28083_n;
    NoiseGeneratorOctaves field_28082_o;
    NoiseGeneratorOctaves field_28096_a;
    NoiseGeneratorOctaves field_28095_b;
    NoiseGeneratorOctaves field_28094_c;
    World *field_28081_p;
    double *field_28080_q;
    double field_28079_r[256];
    double field_28078_s[256];
    double field_28077_t[256];
    MapGenCaves field_28076_u;
    BiomeGenBase **field_28075_v;
    double *field_28093_d;
    double *field_28092_e;
    double *field_28091_f;
    double *field_28090_g;
    double *field_28089_h;
    int field_28088_i[32][32];
    double *field_28074_w;
} ChunkProviderSky;

ChunkProviderSky *ChunkProviderSky_create(World *world, long seed);
void ChunkProviderSky_free(ChunkProviderSky *self);

Chunk *ChunkProviderSky_provideChunk(ChunkProviderSky *self, int cx, int cz);
void ChunkProviderSky_populate(ChunkProviderSky *self, int cx, int cz);

#endif
