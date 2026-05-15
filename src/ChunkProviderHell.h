#ifndef CHUNK_PROVIDER_HELL_H
#define CHUNK_PROVIDER_HELL_H

#include "World.h"
#include "Chunk.h"
#include "JavaRandom.h"
#include "NoiseGeneratorOctaves.h"
#include "MapGenCavesHell.h"

typedef struct ChunkProviderHell {
    IChunkProvider base;

    JavaRandom hellRNG;
    NoiseGeneratorOctaves field_4169_i;
    NoiseGeneratorOctaves field_4168_j;
    NoiseGeneratorOctaves field_4167_k;
    NoiseGeneratorOctaves field_4166_l;
    NoiseGeneratorOctaves field_4165_m;
    NoiseGeneratorOctaves field_4177_a;
    NoiseGeneratorOctaves field_4176_b;
    World *worldObj;
    double *field_4163_o;
    double field_4162_p[256];
    double field_4161_q[256];
    double field_4160_r[256];
    MapGenCavesHell field_4159_s;
    double *field_4175_c;
    double *field_4174_d;
    double *field_4173_e;
    double *field_4172_f;
    double *field_4171_g;
} ChunkProviderHell;

extern const IChunkProviderVtable ChunkProviderHell_vtable;
ChunkProviderHell *ChunkProviderHell_create(World *world, long seed);
void ChunkProviderHell_free(ChunkProviderHell *self);

Chunk *ChunkProviderHell_provideChunk(ChunkProviderHell *self, int cx, int cz);
void ChunkProviderHell_populate(ChunkProviderHell *self, int cx, int cz);

#endif
