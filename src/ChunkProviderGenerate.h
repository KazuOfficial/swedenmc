#ifndef CHUNK_PROVIDER_GENERATE_H
#define CHUNK_PROVIDER_GENERATE_H

#include "IChunkProvider.h"
#include "World.h"
#include "Chunk.h"
#include "JavaRandom.h"
#include "NoiseGeneratorOctaves.h"
#include "BiomeGenBase.h"
#include "MapGenCaves.h"

typedef struct ChunkProviderGenerate {
    IChunkProvider base;
    World *worldObj;
    JavaRandom rand;

    NoiseGeneratorOctaves field_912_k;
    NoiseGeneratorOctaves field_911_l;
    NoiseGeneratorOctaves field_910_m;
    NoiseGeneratorOctaves field_909_n;
    NoiseGeneratorOctaves field_908_o;
    NoiseGeneratorOctaves field_922_a;
    NoiseGeneratorOctaves field_921_b;
    NoiseGeneratorOctaves mobSpawnerNoise;

    double *field_4180_q;

    double sandNoise[256];
    double gravelNoise[256];
    double stoneNoise[256];

    double *generatedTemperatures;

    double *field_4185_d;
    double *field_4184_e;
    double *field_4183_f;
    double *field_4182_g;
    double *field_4181_h;

    BiomeGenBase *biomesForGeneration[16 * 16];

    MapGenCaves caveGen;
} ChunkProviderGenerate;

extern const IChunkProviderVtable ChunkProviderGenerate_vtable;
ChunkProviderGenerate *ChunkProviderGenerate_create(World *world, long seed);
void ChunkProviderGenerate_free(ChunkProviderGenerate *self);

Chunk *ChunkProviderGenerate_provideChunk(ChunkProviderGenerate *self, int cx, int cz);

void ChunkProviderGenerate_generateTerrain(ChunkProviderGenerate *self, int cx, int cz,
                                           uint8_t *blocks, BiomeGenBase **biomes,
                                           double *temperatures);

void ChunkProviderGenerate_replaceBlocksForBiome(ChunkProviderGenerate *self, int cx, int cz,
                                                 uint8_t *blocks, BiomeGenBase **biomes);

void ChunkProviderGenerate_populate(ChunkProviderGenerate *self, int cx, int cz);

#endif
