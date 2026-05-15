#ifndef NOISE_GENERATOR_PERLIN_H
#define NOISE_GENERATOR_PERLIN_H

#include "JavaRandom.h"

typedef struct NoiseGeneratorPerlin {
    int permutations[512];
    double xCoord;
    double yCoord;
    double zCoord;
} NoiseGeneratorPerlin;

void NoiseGeneratorPerlin_init(NoiseGeneratorPerlin *self, JavaRandom *rand);
double NoiseGeneratorPerlin_generateNoise(NoiseGeneratorPerlin *self, double x, double y, double z);
double NoiseGeneratorPerlin_generateNoise2D(NoiseGeneratorPerlin *self, double x, double z);

void NoiseGeneratorPerlin_populateNoiseArray(NoiseGeneratorPerlin *self, double *buf, double xOff,
                                             double yOff, double zOff, int sizeX, int sizeY,
                                             int sizeZ, double scaleX, double scaleY, double scaleZ,
                                             double amplitude);

#endif
