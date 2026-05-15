#ifndef BIOME_GEN_BASE_H
#define BIOME_GEN_BASE_H

#include <stdint.h>
#include "SpawnListEntry.h"
#include "EnumCreatureType.h"
#include "JavaRandom.h"

struct WorldGenerator;

typedef struct {
    SpawnListEntry *entries;
    int count;
    int capacity;
} SpawnList;

typedef struct BiomeGenBase BiomeGenBase;

typedef struct BiomeGenBaseVtable {

    struct WorldGenerator *(*getRandomWorldGenForTrees)(BiomeGenBase *self, JavaRandom *var1);

    int (*getSkyColorByTemp)(BiomeGenBase *self, float var1);
} BiomeGenBaseVtable;

struct BiomeGenBase {
    const BiomeGenBaseVtable *vtable;

    char *biomeName;

    int color;

    int8_t topBlock;

    int8_t fillerBlock;

    int field_6502_q;

    SpawnList spawnableMonsterList;

    SpawnList spawnableCreatureList;

    SpawnList spawnableWaterCreatureList;

    int enableSnow;

    int enableRain;
};

typedef enum {
    BIOME_RAINFOREST = 0,
    BIOME_SWAMPLAND,
    BIOME_SEASONAL_FOREST,
    BIOME_FOREST,
    BIOME_SAVANNA,
    BIOME_SHRUBLAND,
    BIOME_TAIGA,
    BIOME_DESERT,
    BIOME_PLAINS,
    BIOME_ICE_DESERT,
    BIOME_TUNDRA,
    BIOME_HELL,
    BIOME_SKY,
    BIOME_COUNT
} BiomeIndex;

extern BiomeGenBase BiomeGenBase_biomes[BIOME_COUNT];

extern BiomeGenBase *BiomeGenBase_rainforest;
extern BiomeGenBase *BiomeGenBase_swampland;
extern BiomeGenBase *BiomeGenBase_seasonalForest;
extern BiomeGenBase *BiomeGenBase_forest;
extern BiomeGenBase *BiomeGenBase_savanna;
extern BiomeGenBase *BiomeGenBase_shrubland;
extern BiomeGenBase *BiomeGenBase_taiga;
extern BiomeGenBase *BiomeGenBase_desert;
extern BiomeGenBase *BiomeGenBase_plains;
extern BiomeGenBase *BiomeGenBase_iceDesert;
extern BiomeGenBase *BiomeGenBase_tundra;
extern BiomeGenBase *BiomeGenBase_hell;
extern BiomeGenBase *BiomeGenBase_sky;

extern BiomeGenBase *BiomeGenBase_biomeLookupTable[4096];

extern const BiomeGenBaseVtable BiomeGenBase_defaultVtable;

void BiomeGenBase_construct(BiomeGenBase *self);

void BiomeGenBase_generateBiomeLookup(void);

BiomeGenBase *BiomeGenBase_getBiomeFromLookup(double var0, double var2);

BiomeGenBase *BiomeGenBase_getBiome(float var0, float var1);

struct WorldGenerator *BiomeGenBase_getRandomWorldGenForTrees(BiomeGenBase *self, JavaRandom *var1);

int BiomeGenBase_getSkyColorByTemp(BiomeGenBase *self, float var1);

SpawnList *BiomeGenBase_getSpawnableList(BiomeGenBase *self, EnumCreatureType var1);

int BiomeGenBase_getEnableSnow(BiomeGenBase *self);

int BiomeGenBase_canSpawnLightningBolt(BiomeGenBase *self);

BiomeGenBase *BiomeGenBase_setEnableSnow(BiomeGenBase *self);

BiomeGenBase *BiomeGenBase_setBiomeName(BiomeGenBase *self, const char *var1);

BiomeGenBase *BiomeGenBase_func_4124_a(BiomeGenBase *self, int var1);

BiomeGenBase *BiomeGenBase_setColor(BiomeGenBase *self, int var1);

void SpawnList_add(SpawnList *list, EntitySpawnType type, int rate);

int BiomeGenBase_getSkyColorByTemp_impl(BiomeGenBase *self, float var1);

void BiomeGenRainforest_construct(BiomeGenBase *self);
void BiomeGenSwamp_construct(BiomeGenBase *self);
void BiomeGenTaiga_construct(BiomeGenBase *self);
void BiomeGenHell_construct(BiomeGenBase *self);
void BiomeGenSky_construct(BiomeGenBase *self);

void BiomeGenBase_init(void);

#endif
