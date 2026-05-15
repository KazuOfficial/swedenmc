#ifndef NOISE_GENERATOR_OCTAVES_H
#define NOISE_GENERATOR_OCTAVES_H

#include "NoiseGeneratorPerlin.h"

typedef struct NoiseGeneratorOctaves {
    NoiseGeneratorPerlin *octaves;
    int numOctaves;
} NoiseGeneratorOctaves;

void NoiseGeneratorOctaves_init(NoiseGeneratorOctaves *self, JavaRandom *rand, int numOctaves);
void NoiseGeneratorOctaves_free(NoiseGeneratorOctaves *self);

double NoiseGeneratorOctaves_sample2D(NoiseGeneratorOctaves *self, double x, double z);

double *NoiseGeneratorOctaves_generateNoiseOctaves(NoiseGeneratorOctaves *self, double *buf,
                                                   double xOff, double yOff, double zOff, int sizeX,
                                                   int sizeY, int sizeZ, double scaleX,
                                                   double scaleY, double scaleZ);

double *NoiseGeneratorOctaves_generateNoiseOctaves2D(NoiseGeneratorOctaves *self, double *buf,
                                                     int x, int z, int sizeX, int sizeZ,
                                                     double scaleX, double scaleZ, double scaleY);

#endif
