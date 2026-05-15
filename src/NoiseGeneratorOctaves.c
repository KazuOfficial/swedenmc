#include "NoiseGeneratorOctaves.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void NoiseGeneratorOctaves_init(NoiseGeneratorOctaves *self, JavaRandom *rand, int n) {
    self->numOctaves = n;
    self->octaves = (NoiseGeneratorPerlin *)malloc(n * sizeof(NoiseGeneratorPerlin));
    for (int i = 0; i < n; i++)
        NoiseGeneratorPerlin_init(&self->octaves[i], rand);
}

void NoiseGeneratorOctaves_free(NoiseGeneratorOctaves *self) {
    free(self->octaves);
    self->octaves = NULL;
}

double NoiseGeneratorOctaves_sample2D(NoiseGeneratorOctaves *self, double x, double z) {
    double sum = 0.0;
    double scale = 1.0;
    for (int i = 0; i < self->numOctaves; i++) {
        sum +=
            NoiseGeneratorPerlin_generateNoise2D(&self->octaves[i], x * scale, z * scale) / scale;
        scale /= 2.0;
    }
    return sum;
}

double *NoiseGeneratorOctaves_generateNoiseOctaves(NoiseGeneratorOctaves *self, double *buf,
                                                   double xOff, double yOff, double zOff, int sizeX,
                                                   int sizeY, int sizeZ, double scaleX,
                                                   double scaleY, double scaleZ) {
    int len = sizeX * sizeY * sizeZ;
    if (!buf) {
        buf = (double *)malloc(len * sizeof(double));
    }
    memset(buf, 0, len * sizeof(double));

    double amp = 1.0;
    for (int i = 0; i < self->numOctaves; i++) {
        NoiseGeneratorPerlin_populateNoiseArray(&self->octaves[i], buf, xOff, yOff, zOff, sizeX,
                                                sizeY, sizeZ, scaleX * amp, scaleY * amp,
                                                scaleZ * amp, amp);
        amp /= 2.0;
    }
    return buf;
}

double *NoiseGeneratorOctaves_generateNoiseOctaves2D(NoiseGeneratorOctaves *self, double *buf,
                                                     int x, int z, int sizeX, int sizeZ,
                                                     double scaleX, double scaleZ, double scaleY) {
    (void)scaleY;
    return NoiseGeneratorOctaves_generateNoiseOctaves(self, buf, (double)x, 10.0, (double)z, sizeX,
                                                      1, sizeZ, scaleX, 1.0, scaleZ);
}
