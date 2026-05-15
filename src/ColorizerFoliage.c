#include "ColorizerFoliage.h"
#include <stdlib.h>

int *ColorizerFoliage_foliageBuffer = NULL;
static int *s_foliage_init_buf = NULL;

void ColorizerFoliage_func_28152_a(int *var0) {
    free(s_foliage_init_buf);
    s_foliage_init_buf = NULL;
    ColorizerFoliage_foliageBuffer = var0;
}

int ColorizerFoliage_getFoliageColor(double var0, double var2) {
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
    return ColorizerFoliage_foliageBuffer[var5 << 8 | var4];
}

int ColorizerFoliage_getFoliageColorPine(void) { return 6396257; }

int ColorizerFoliage_getFoliageColorBirch(void) { return 8431445; }

int ColorizerFoliage_func_31073_c(void) { return 4764952; }

void ColorizerFoliage_init(void) {
    static int s_init = 0;
    if (s_init)
        return;
    s_init = 1;
    s_foliage_init_buf = (int *)calloc(65536, sizeof(int));
    ColorizerFoliage_foliageBuffer = s_foliage_init_buf;
}
