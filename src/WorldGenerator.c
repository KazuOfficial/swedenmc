#include "WorldGenerator.h"
#include <stdlib.h>

void WorldGenerator_noop_func_517_a(WorldGenerator *self, double var1, double var3, double var5) {
    (void)self;
    (void)var1;
    (void)var3;
    (void)var5;
}

void WorldGenerator_simple_destroy(WorldGenerator *self) { free(self); }
