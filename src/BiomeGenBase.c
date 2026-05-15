#include "BiomeGenBase.h"
#include "BiomeGenDesert.h"
#include "BiomeGenForest.h"
#include "Block.h"
#include "WorldGenerator.h"
#include "WorldGenTrees.h"
#include "WorldGenBigTree.h"
#include "WorldGenTaiga1.h"
#include "WorldGenTaiga2.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

BiomeGenBase BiomeGenBase_biomes[BIOME_COUNT];

BiomeGenBase *BiomeGenBase_rainforest = NULL;
BiomeGenBase *BiomeGenBase_swampland = NULL;
BiomeGenBase *BiomeGenBase_seasonalForest = NULL;
BiomeGenBase *BiomeGenBase_forest = NULL;
BiomeGenBase *BiomeGenBase_savanna = NULL;
BiomeGenBase *BiomeGenBase_shrubland = NULL;
BiomeGenBase *BiomeGenBase_taiga = NULL;
BiomeGenBase *BiomeGenBase_desert = NULL;
BiomeGenBase *BiomeGenBase_plains = NULL;
BiomeGenBase *BiomeGenBase_iceDesert = NULL;
BiomeGenBase *BiomeGenBase_tundra = NULL;
BiomeGenBase *BiomeGenBase_hell = NULL;
BiomeGenBase *BiomeGenBase_sky = NULL;

BiomeGenBase *BiomeGenBase_biomeLookupTable[4096];

void SpawnList_add(SpawnList *list, EntitySpawnType type, int rate) {
    if (list->count == list->capacity) {
        int newcap = list->capacity == 0 ? 8 : list->capacity * 2;
        list->entries = (SpawnListEntry *)realloc(list->entries, newcap * sizeof(SpawnListEntry));
        list->capacity = newcap;
    }
    list->entries[list->count].entityType = type;
    list->entries[list->count].spawnRarityRate = rate;
    list->count++;
}

static int Color_getHSBColor_getRGB(float hue, float saturation, float brightness) {
    int r = 0, g = 0, b = 0;
    if (saturation == 0.0f) {
        r = g = b = (int)(brightness * 255.0f + 0.5f);
    } else {
        float h = (hue - floorf(hue)) * 6.0f;
        float f = h - floorf(h);
        float p = brightness * (1.0f - saturation);
        float q = brightness * (1.0f - saturation * f);
        float t = brightness * (1.0f - (saturation * (1.0f - f)));
        switch ((int)h) {
        case 0:
            r = (int)(brightness * 255.0f + 0.5f);
            g = (int)(t * 255.0f + 0.5f);
            b = (int)(p * 255.0f + 0.5f);
            break;
        case 1:
            r = (int)(q * 255.0f + 0.5f);
            g = (int)(brightness * 255.0f + 0.5f);
            b = (int)(p * 255.0f + 0.5f);
            break;
        case 2:
            r = (int)(p * 255.0f + 0.5f);
            g = (int)(brightness * 255.0f + 0.5f);
            b = (int)(t * 255.0f + 0.5f);
            break;
        case 3:
            r = (int)(p * 255.0f + 0.5f);
            g = (int)(q * 255.0f + 0.5f);
            b = (int)(brightness * 255.0f + 0.5f);
            break;
        case 4:
            r = (int)(t * 255.0f + 0.5f);
            g = (int)(p * 255.0f + 0.5f);
            b = (int)(brightness * 255.0f + 0.5f);
            break;
        case 5:
            r = (int)(brightness * 255.0f + 0.5f);
            g = (int)(p * 255.0f + 0.5f);
            b = (int)(q * 255.0f + 0.5f);
            break;
        }
    }
    return (int)0xff000000 | (r << 16) | (g << 8) | b;
}

static struct WorldGenerator *BiomeGenBase_def_getRandomWorldGenForTrees(BiomeGenBase *self,
                                                                         JavaRandom *var1) {
    (void)self;
    if (JavaRandom_nextInt(var1, 10) == 0)
        return (struct WorldGenerator *)WorldGenBigTree_create();
    return (struct WorldGenerator *)WorldGenTrees_create();
}

int BiomeGenBase_getSkyColorByTemp_impl(BiomeGenBase *self, float var1) {
    (void)self;
    var1 /= 3.0f;
    if (var1 < -1.0f) {
        var1 = -1.0f;
    }
    if (var1 > 1.0f) {
        var1 = 1.0f;
    }
    return Color_getHSBColor_getRGB(224.0f / 360.0f - var1 * 0.05f, 0.5f + var1 * 0.1f, 1.0f);
}

const BiomeGenBaseVtable BiomeGenBase_defaultVtable = {BiomeGenBase_def_getRandomWorldGenForTrees,
                                                       BiomeGenBase_getSkyColorByTemp_impl};

static struct WorldGenerator *BiomeGenRainforest_getRandomWorldGenForTrees(BiomeGenBase *self,
                                                                           JavaRandom *var1) {
    (void)self;
    if (JavaRandom_nextInt(var1, 3) == 0)
        return (struct WorldGenerator *)WorldGenBigTree_create();
    return (struct WorldGenerator *)WorldGenTrees_create();
}

static const BiomeGenBaseVtable BiomeGenRainforest_vtable = {
    BiomeGenRainforest_getRandomWorldGenForTrees, BiomeGenBase_getSkyColorByTemp_impl};

void BiomeGenRainforest_construct(BiomeGenBase *self) {
    BiomeGenBase_construct(self);
    self->vtable = &BiomeGenRainforest_vtable;
}

void BiomeGenSwamp_construct(BiomeGenBase *self) { BiomeGenBase_construct(self); }

static struct WorldGenerator *BiomeGenTaiga_getRandomWorldGenForTrees(BiomeGenBase *self,
                                                                      JavaRandom *var1) {
    (void)self;
    if (JavaRandom_nextInt(var1, 3) == 0)
        return (struct WorldGenerator *)WorldGenTaiga1_create();
    return (struct WorldGenerator *)WorldGenTaiga2_create();
}

static const BiomeGenBaseVtable BiomeGenTaiga_vtable = {BiomeGenTaiga_getRandomWorldGenForTrees,
                                                        BiomeGenBase_getSkyColorByTemp_impl};

void BiomeGenTaiga_construct(BiomeGenBase *self) {
    BiomeGenBase_construct(self);
    self->vtable = &BiomeGenTaiga_vtable;
    SpawnList_add(&self->spawnableCreatureList, ENTITY_SPAWN_WOLF, 2);
}

void BiomeGenHell_construct(BiomeGenBase *self) {
    BiomeGenBase_construct(self);
    self->spawnableMonsterList.count = 0;
    self->spawnableCreatureList.count = 0;
    self->spawnableWaterCreatureList.count = 0;
    SpawnList_add(&self->spawnableMonsterList, ENTITY_SPAWN_GHAST, 10);
    SpawnList_add(&self->spawnableMonsterList, ENTITY_SPAWN_PIG_ZOMBIE, 10);
}

static int BiomeGenSky_getSkyColorByTemp(BiomeGenBase *self, float var1) {
    (void)self;
    (void)var1;
    return 12632319;
}

static const BiomeGenBaseVtable BiomeGenSky_vtable = {BiomeGenBase_def_getRandomWorldGenForTrees,
                                                      BiomeGenSky_getSkyColorByTemp};

void BiomeGenSky_construct(BiomeGenBase *self) {
    BiomeGenBase_construct(self);
    self->vtable = &BiomeGenSky_vtable;
    self->spawnableMonsterList.count = 0;
    self->spawnableCreatureList.count = 0;
    self->spawnableWaterCreatureList.count = 0;
    SpawnList_add(&self->spawnableCreatureList, ENTITY_SPAWN_CHICKEN, 10);
}

static BiomeGenBase *BiomeGenBase_setDisableRain(BiomeGenBase *self) {
    self->enableRain = 0;
    return self;
}

BiomeGenBase *BiomeGenBase_setEnableSnow(BiomeGenBase *self) {
    self->enableSnow = 1;
    return self;
}

BiomeGenBase *BiomeGenBase_setBiomeName(BiomeGenBase *self, const char *var1) {
    self->biomeName = (char *)var1;
    return self;
}

BiomeGenBase *BiomeGenBase_func_4124_a(BiomeGenBase *self, int var1) {
    self->field_6502_q = var1;
    return self;
}

BiomeGenBase *BiomeGenBase_setColor(BiomeGenBase *self, int var1) {
    self->color = var1;
    return self;
}

void BiomeGenBase_construct(BiomeGenBase *self) {
    self->vtable = &BiomeGenBase_defaultVtable;
    self->biomeName = NULL;
    self->color = 0;
    self->topBlock = (int8_t)Block_grass->blockID;
    self->fillerBlock = (int8_t)Block_dirt->blockID;
    self->field_6502_q = 5169201;
    memset(&self->spawnableMonsterList, 0, sizeof(SpawnList));
    memset(&self->spawnableCreatureList, 0, sizeof(SpawnList));
    memset(&self->spawnableWaterCreatureList, 0, sizeof(SpawnList));
    self->enableSnow = 0;
    self->enableRain = 1;

    SpawnList_add(&self->spawnableMonsterList, ENTITY_SPAWN_SPIDER, 10);
    SpawnList_add(&self->spawnableMonsterList, ENTITY_SPAWN_ZOMBIE, 10);
    SpawnList_add(&self->spawnableMonsterList, ENTITY_SPAWN_SKELETON, 10);
    SpawnList_add(&self->spawnableMonsterList, ENTITY_SPAWN_CREEPER, 10);
    SpawnList_add(&self->spawnableMonsterList, ENTITY_SPAWN_SLIME, 10);
    SpawnList_add(&self->spawnableCreatureList, ENTITY_SPAWN_SHEEP, 12);
    SpawnList_add(&self->spawnableCreatureList, ENTITY_SPAWN_PIG, 10);
    SpawnList_add(&self->spawnableCreatureList, ENTITY_SPAWN_CHICKEN, 10);
    SpawnList_add(&self->spawnableCreatureList, ENTITY_SPAWN_COW, 8);
    SpawnList_add(&self->spawnableWaterCreatureList, ENTITY_SPAWN_SQUID, 10);
}

struct WorldGenerator *BiomeGenBase_getRandomWorldGenForTrees(BiomeGenBase *self,
                                                              JavaRandom *var1) {
    return self->vtable->getRandomWorldGenForTrees(self, var1);
}

int BiomeGenBase_getSkyColorByTemp(BiomeGenBase *self, float var1) {
    return self->vtable->getSkyColorByTemp(self, var1);
}

void BiomeGenBase_generateBiomeLookup(void) {
    int var0, var1;
    for (var0 = 0; var0 < 64; ++var0) {
        for (var1 = 0; var1 < 64; ++var1) {
            BiomeGenBase_biomeLookupTable[var0 + var1 * 64] =
                BiomeGenBase_getBiome((float)var0 / 63.0f, (float)var1 / 63.0f);
        }
    }
    BiomeGenBase_desert->topBlock = BiomeGenBase_desert->fillerBlock = (int8_t)Block_sand->blockID;
    BiomeGenBase_iceDesert->topBlock = BiomeGenBase_iceDesert->fillerBlock =
        (int8_t)Block_sand->blockID;
}

BiomeGenBase *BiomeGenBase_getBiomeFromLookup(double var0, double var2) {
    int var4 = (int)(var0 * 63.0);
    int var5 = (int)(var2 * 63.0);
    return BiomeGenBase_biomeLookupTable[var4 + var5 * 64];
}

BiomeGenBase *BiomeGenBase_getBiome(float var0, float var1) {
    var1 *= var0;
    return var0 < 0.1f
               ? BiomeGenBase_tundra
               : (var1 < 0.2f
                      ? (var0 < 0.5f ? BiomeGenBase_tundra
                                     : (var0 < 0.95f ? BiomeGenBase_savanna : BiomeGenBase_desert))
                      : (var1 > 0.5f && var0 < 0.7f
                             ? BiomeGenBase_swampland
                             : (var0 < 0.5f
                                    ? BiomeGenBase_taiga
                                    : (var0 < 0.97f
                                           ? (var1 < 0.35f ? BiomeGenBase_shrubland
                                                           : BiomeGenBase_forest)
                                           : (var1 < 0.45f
                                                  ? BiomeGenBase_plains
                                                  : (var1 < 0.9f ? BiomeGenBase_seasonalForest
                                                                 : BiomeGenBase_rainforest))))));
}

SpawnList *BiomeGenBase_getSpawnableList(BiomeGenBase *self, EnumCreatureType var1) {
    return var1 == EnumCreatureType_monster
               ? &self->spawnableMonsterList
               : (var1 == EnumCreatureType_creature
                      ? &self->spawnableCreatureList
                      : (var1 == EnumCreatureType_waterCreature ? &self->spawnableWaterCreatureList
                                                                : NULL));
}

int BiomeGenBase_getEnableSnow(BiomeGenBase *self) { return self->enableSnow; }

int BiomeGenBase_canSpawnLightningBolt(BiomeGenBase *self) {
    return self->enableSnow ? 0 : self->enableRain;
}

void BiomeGenBase_init(void) {
    static int s_init = 0;
    if (s_init)
        return;
    s_init = 1;

    BiomeGenBase_rainforest = &BiomeGenBase_biomes[BIOME_RAINFOREST];
    BiomeGenRainforest_construct(BiomeGenBase_rainforest);
    BiomeGenBase_setColor(BiomeGenBase_rainforest, 588342);
    BiomeGenBase_setBiomeName(BiomeGenBase_rainforest, "Rainforest");
    BiomeGenBase_func_4124_a(BiomeGenBase_rainforest, 2094168);

    BiomeGenBase_swampland = &BiomeGenBase_biomes[BIOME_SWAMPLAND];
    BiomeGenSwamp_construct(BiomeGenBase_swampland);
    BiomeGenBase_setColor(BiomeGenBase_swampland, 522674);
    BiomeGenBase_setBiomeName(BiomeGenBase_swampland, "Swampland");
    BiomeGenBase_func_4124_a(BiomeGenBase_swampland, 9154376);

    BiomeGenBase_seasonalForest = &BiomeGenBase_biomes[BIOME_SEASONAL_FOREST];
    BiomeGenBase_construct(BiomeGenBase_seasonalForest);
    BiomeGenBase_setColor(BiomeGenBase_seasonalForest, 10215459);
    BiomeGenBase_setBiomeName(BiomeGenBase_seasonalForest, "Seasonal Forest");

    BiomeGenBase_forest = &BiomeGenBase_biomes[BIOME_FOREST];
    BiomeGenForest_construct((BiomeGenForest *)BiomeGenBase_forest);
    BiomeGenBase_setColor(BiomeGenBase_forest, 353825);
    BiomeGenBase_setBiomeName(BiomeGenBase_forest, "Forest");
    BiomeGenBase_func_4124_a(BiomeGenBase_forest, 5159473);

    BiomeGenBase_savanna = &BiomeGenBase_biomes[BIOME_SAVANNA];
    BiomeGenDesert_construct((BiomeGenDesert *)BiomeGenBase_savanna);
    BiomeGenBase_setColor(BiomeGenBase_savanna, 14278691);
    BiomeGenBase_setBiomeName(BiomeGenBase_savanna, "Savanna");

    BiomeGenBase_shrubland = &BiomeGenBase_biomes[BIOME_SHRUBLAND];
    BiomeGenBase_construct(BiomeGenBase_shrubland);
    BiomeGenBase_setColor(BiomeGenBase_shrubland, 10595616);
    BiomeGenBase_setBiomeName(BiomeGenBase_shrubland, "Shrubland");

    BiomeGenBase_taiga = &BiomeGenBase_biomes[BIOME_TAIGA];
    BiomeGenTaiga_construct(BiomeGenBase_taiga);
    BiomeGenBase_setColor(BiomeGenBase_taiga, 3060051);
    BiomeGenBase_setBiomeName(BiomeGenBase_taiga, "Taiga");
    BiomeGenBase_setEnableSnow(BiomeGenBase_taiga);
    BiomeGenBase_func_4124_a(BiomeGenBase_taiga, 8107825);

    BiomeGenBase_desert = &BiomeGenBase_biomes[BIOME_DESERT];
    BiomeGenDesert_construct((BiomeGenDesert *)BiomeGenBase_desert);
    BiomeGenBase_setColor(BiomeGenBase_desert, 16421912);
    BiomeGenBase_setBiomeName(BiomeGenBase_desert, "Desert");
    BiomeGenBase_setDisableRain(BiomeGenBase_desert);

    BiomeGenBase_plains = &BiomeGenBase_biomes[BIOME_PLAINS];
    BiomeGenDesert_construct((BiomeGenDesert *)BiomeGenBase_plains);
    BiomeGenBase_setColor(BiomeGenBase_plains, 16767248);
    BiomeGenBase_setBiomeName(BiomeGenBase_plains, "Plains");

    BiomeGenBase_iceDesert = &BiomeGenBase_biomes[BIOME_ICE_DESERT];
    BiomeGenDesert_construct((BiomeGenDesert *)BiomeGenBase_iceDesert);
    BiomeGenBase_setColor(BiomeGenBase_iceDesert, 16772499);
    BiomeGenBase_setBiomeName(BiomeGenBase_iceDesert, "Ice Desert");
    BiomeGenBase_setEnableSnow(BiomeGenBase_iceDesert);
    BiomeGenBase_setDisableRain(BiomeGenBase_iceDesert);
    BiomeGenBase_func_4124_a(BiomeGenBase_iceDesert, 12899129);

    BiomeGenBase_tundra = &BiomeGenBase_biomes[BIOME_TUNDRA];
    BiomeGenBase_construct(BiomeGenBase_tundra);
    BiomeGenBase_setColor(BiomeGenBase_tundra, 5762041);
    BiomeGenBase_setBiomeName(BiomeGenBase_tundra, "Tundra");
    BiomeGenBase_setEnableSnow(BiomeGenBase_tundra);
    BiomeGenBase_func_4124_a(BiomeGenBase_tundra, 12899129);

    BiomeGenBase_hell = &BiomeGenBase_biomes[BIOME_HELL];
    BiomeGenHell_construct(BiomeGenBase_hell);
    BiomeGenBase_setColor(BiomeGenBase_hell, 16711680);
    BiomeGenBase_setBiomeName(BiomeGenBase_hell, "Hell");
    BiomeGenBase_setDisableRain(BiomeGenBase_hell);

    BiomeGenBase_sky = &BiomeGenBase_biomes[BIOME_SKY];
    BiomeGenSky_construct(BiomeGenBase_sky);
    BiomeGenBase_setColor(BiomeGenBase_sky, 8421631);
    BiomeGenBase_setBiomeName(BiomeGenBase_sky, "Sky");
    BiomeGenBase_setDisableRain(BiomeGenBase_sky);

    BiomeGenBase_generateBiomeLookup();
}
