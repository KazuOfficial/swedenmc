#ifndef NOISE_GENERATOR2_H
#define NOISE_GENERATOR2_H

typedef struct NoiseGenerator2 {

    int field_4295_e[512];

    double field_4292_a;
    double field_4291_b;
    double field_4297_c;
} NoiseGenerator2;

#include "JavaRandom.h"

void NoiseGenerator2_init(NoiseGenerator2 *self, JavaRandom *rand);

void NoiseGenerator2_func_4157_a(NoiseGenerator2 *self, double *var1, double var2, double var4,
                                 int var6, int var7, double var8, double var10, double var12);

#endif
