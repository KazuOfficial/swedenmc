#include "BiomeGenTaiga.h"
#include "BiomeGenBase.h"
#include "WorldGenerator.h"
#include "WorldGenTaiga1.h"
#include "WorldGenTaiga2.h"
#include "JavaRandom.h"

static WorldGenerator *taiga_getRandomWorldGenForTrees(BiomeGenBase *self, JavaRandom *rand) {
    (void)self;
    if (JavaRandom_nextInt(rand, 3) == 0)
        return &WorldGenTaiga1_create()->base;
    return &WorldGenTaiga2_create()->base;
}

BiomeGenBaseVtable BiomeGenTaiga_vtable = {taiga_getRandomWorldGenForTrees, NULL};

static void __attribute__((constructor)) BiomeGenTaiga_initVtable(void) {
    BiomeGenTaiga_vtable = BiomeGenBase_defaultVtable;
    BiomeGenTaiga_vtable.getRandomWorldGenForTrees = taiga_getRandomWorldGenForTrees;
}
