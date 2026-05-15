#include "ColorizerWater.h"
#include <stdlib.h>

int *ColorizerWater_waterBuffer = NULL;
static int *s_water_init_buf = NULL;

void ColorizerWater_func_28182_a(int *var0) {
    free(s_water_init_buf);
    s_water_init_buf = NULL;
    ColorizerWater_waterBuffer = var0;
}

void ColorizerWater_init(void) {
    static int s_init = 0;
    if (s_init)
        return;
    s_init = 1;
    s_water_init_buf = (int *)calloc(65536, sizeof(int));
    ColorizerWater_waterBuffer = s_water_init_buf;
}
