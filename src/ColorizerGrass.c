#include "ColorizerGrass.h"
#include <stdlib.h>

int *ColorizerGrass_grassBuffer = NULL;
static int *s_grass_init_buf = NULL;

void ColorizerGrass_func_28181_a(int *var0) {
    free(s_grass_init_buf);
    s_grass_init_buf = NULL;
    ColorizerGrass_grassBuffer = var0;
}

int ColorizerGrass_getGrassColor(double var0, double var2) {
    var2 *= var0;
    int var4 = (int)((1.0 - var0) * 255.0);
    int var5 = (int)((1.0 - var2) * 255.0);
    if (var4 < 0)
        var4 = 0;
    else if (var4 > 255)
        var4 = 255;
    if (var5 < 0)
        var5 = 0;
    else if (var5 > 255)
        var5 = 255;
    return ColorizerGrass_grassBuffer[var5 << 8 | var4];
}

void ColorizerGrass_init(void) {
    static int s_init = 0;
    if (s_init)
        return;
    s_init = 1;
    s_grass_init_buf = (int *)calloc(65536, sizeof(int));
    ColorizerGrass_grassBuffer = s_grass_init_buf;
}
