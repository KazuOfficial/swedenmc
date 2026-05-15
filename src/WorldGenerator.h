#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "World.h"
#include "JavaRandom.h"

typedef struct WorldGenerator WorldGenerator;

typedef struct WorldGeneratorVtable {
    int (*generate)(WorldGenerator *self, World *world, JavaRandom *rand, int x, int y, int z);

    void (*func_517_a)(WorldGenerator *self, double var1, double var3, double var5);

    void (*destroy)(WorldGenerator *self);
} WorldGeneratorVtable;

struct WorldGenerator {
    const WorldGeneratorVtable *vtable;
    double scaleX, scaleY, scaleZ;
};

void WorldGenerator_noop_func_517_a(WorldGenerator *self, double var1, double var3, double var5);
void WorldGenerator_simple_destroy(WorldGenerator *self);

static inline int WorldGenerator_generate(WorldGenerator *g, World *w, JavaRandom *r, int x, int y,
                                          int z) {
    return g->vtable->generate(g, w, r, x, y, z);
}
static inline void WorldGenerator_func_517_a(WorldGenerator *g, double v1, double v3, double v5) {
    g->vtable->func_517_a(g, v1, v3, v5);
}
static inline void WorldGenerator_destroy(WorldGenerator *g) { g->vtable->destroy(g); }

#endif
