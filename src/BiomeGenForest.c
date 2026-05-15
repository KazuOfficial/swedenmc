#include "BiomeGenForest.h"
#include "WorldGenerator.h"
#include "WorldGenTrees.h"
#include "WorldGenBigTree.h"
#include "WorldGenForest.h"
#include "JavaRandom.h"

static struct WorldGenerator *BiomeGenForest_getRandomWorldGenForTrees(BiomeGenBase *self,
                                                                       JavaRandom *var1) {
    (void)self;
    if (JavaRandom_nextInt(var1, 5) == 0)
        return (struct WorldGenerator *)WorldGenForest_create();
    if (JavaRandom_nextInt(var1, 3) == 0)
        return (struct WorldGenerator *)WorldGenBigTree_create();
    return (struct WorldGenerator *)WorldGenTrees_create();
}

static const BiomeGenBaseVtable BiomeGenForest_vtable = {BiomeGenForest_getRandomWorldGenForTrees,
                                                         BiomeGenBase_getSkyColorByTemp_impl};

void BiomeGenForest_construct(BiomeGenForest *self) {
    BiomeGenBase_construct(&self->base);
    self->base.vtable = &BiomeGenForest_vtable;
    SpawnList_add(&self->base.spawnableCreatureList, ENTITY_SPAWN_WOLF, 2);
}
