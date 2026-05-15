#ifndef NOISE_GENERATOR_OCTAVES2_H
#define NOISE_GENERATOR_OCTAVES2_H

#include "NoiseGenerator2.h"
#include "JavaRandom.h"

typedef struct NoiseGeneratorOctaves2 {
    NoiseGenerator2 *field_4234_a;
    int field_4233_b;
} NoiseGeneratorOctaves2;

void NoiseGeneratorOctaves2_init(NoiseGeneratorOctaves2 *self, JavaRandom *rand, int octaves);
void NoiseGeneratorOctaves2_free(NoiseGeneratorOctaves2 *self);

double *NoiseGeneratorOctaves2_func_4112_a(NoiseGeneratorOctaves2 *self, double *var1, double var2,
                                           double var4, int var6, int var7, double var8,
                                           double var10, double var12);

double *NoiseGeneratorOctaves2_func_4111_a(NoiseGeneratorOctaves2 *self, double *var1, double var2,
                                           double var4, int var6, int var7, double var8,
                                           double var10, double var12, double var14);

#endif
