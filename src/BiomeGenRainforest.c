#include "BiomeGenRainforest.h"
#include "BiomeGenBase.h"
#include "WorldGenerator.h"
#include "WorldGenTrees.h"
#include "WorldGenBigTree.h"
#include "JavaRandom.h"

static WorldGenerator *rainforest_getRandomWorldGenForTrees(BiomeGenBase *self, JavaRandom *rand) {
    (void)self;
    if (JavaRandom_nextInt(rand, 3) == 0)
        return &WorldGenBigTree_create()->base;
    return &WorldGenTrees_create()->base;
}

BiomeGenBaseVtable BiomeGenRainforest_vtable = {rainforest_getRandomWorldGenForTrees, NULL};

static void __attribute__((constructor)) BiomeGenRainforest_initVtable(void) {
    BiomeGenRainforest_vtable = BiomeGenBase_defaultVtable;
    BiomeGenRainforest_vtable.getRandomWorldGenForTrees = rainforest_getRandomWorldGenForTrees;
}
