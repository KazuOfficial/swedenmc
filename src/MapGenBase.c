#include "MapGenBase.h"

void MapGenBase_func_868_a_default(MapGenBase *self, World *world, int srcCX, int srcCZ, int tgtCX,
                                   int tgtCZ, uint8_t *chunkBlocks) {
    (void)self;
    (void)world;
    (void)srcCX;
    (void)srcCZ;
    (void)tgtCX;
    (void)tgtCZ;
    (void)chunkBlocks;
}

void MapGenBase_generate(MapGenBase *self, World *world, int chunkX, int chunkZ,
                         uint8_t *chunkBlocks) {
    int range = self->range;

    long seed = World_getRandomSeed(world);
    JavaRandom_setSeed(&self->rand, seed);
    long mult1 = (JavaRandom_nextLong(&self->rand) / 2L) * 2L + 1L;
    long mult2 = (JavaRandom_nextLong(&self->rand) / 2L) * 2L + 1L;

    for (int cx = chunkX - range; cx <= chunkX + range; cx++) {
        for (int cz = chunkZ - range; cz <= chunkZ + range; cz++) {
            long chunkSeed = (long)(((unsigned long)cx * (unsigned long)mult1 +
                                     (unsigned long)cz * (unsigned long)mult2) ^
                                    (unsigned long)seed);
            JavaRandom_setSeed(&self->rand, chunkSeed);
            self->vtable->func_868_a(self, world, cx, cz, chunkX, chunkZ, chunkBlocks);
        }
    }
}
