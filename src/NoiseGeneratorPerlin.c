#include "NoiseGeneratorPerlin.h"
#include <math.h>

static double lerp(double t, double a, double b) { return a + t * (b - a); }

static double grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

static double grad2(int hash, double x, double z) {
    int h = hash & 15;
    double u = (double)(1 - ((h & 8) >> 3)) * x;
    double v = h < 4 ? 0.0 : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

static double fade(double t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }

void NoiseGeneratorPerlin_init(NoiseGeneratorPerlin *self, JavaRandom *rand) {
    self->xCoord = JavaRandom_nextDouble(rand) * 256.0;
    self->yCoord = JavaRandom_nextDouble(rand) * 256.0;
    self->zCoord = JavaRandom_nextDouble(rand) * 256.0;

    for (int i = 0; i < 256; i++)
        self->permutations[i] = i;

    for (int i = 0; i < 256; i++) {
        int j = JavaRandom_nextInt(rand, 256 - i) + i;
        int tmp = self->permutations[i];
        self->permutations[i] = self->permutations[j];
        self->permutations[j] = tmp;
        self->permutations[i + 256] = self->permutations[i];
    }
}

double NoiseGeneratorPerlin_generateNoise(NoiseGeneratorPerlin *self, double x, double y,
                                          double z) {
    x += self->xCoord;
    y += self->yCoord;
    z += self->zCoord;
    int ix = (int)x;
    if (x < ix)
        --ix;
    int iy = (int)y;
    if (y < iy)
        --iy;
    int iz = (int)z;
    if (z < iz)
        --iz;
    int X = ix & 255, Y = iy & 255, Z = iz & 255;
    x -= ix;
    y -= iy;
    z -= iz;
    double u = fade(x), v = fade(y), w = fade(z);
    int A = self->permutations[X] + Y;
    int AA = self->permutations[A] + Z;
    int AB = self->permutations[A + 1] + Z;
    int B = self->permutations[X + 1] + Y;
    int BA = self->permutations[B] + Z;
    int BB = self->permutations[B + 1] + Z;
    return lerp(w,
                lerp(v,
                     lerp(u, grad(self->permutations[AA], x, y, z),
                          grad(self->permutations[BA], x - 1, y, z)),
                     lerp(u, grad(self->permutations[AB], x, y - 1, z),
                          grad(self->permutations[BB], x - 1, y - 1, z))),
                lerp(v,
                     lerp(u, grad(self->permutations[AA + 1], x, y, z - 1),
                          grad(self->permutations[BA + 1], x - 1, y, z - 1)),
                     lerp(u, grad(self->permutations[AB + 1], x, y - 1, z - 1),
                          grad(self->permutations[BB + 1], x - 1, y - 1, z - 1))));
}

double NoiseGeneratorPerlin_generateNoise2D(NoiseGeneratorPerlin *self, double x, double z) {
    return NoiseGeneratorPerlin_generateNoise(self, x, z, 0.0);
}

void NoiseGeneratorPerlin_populateNoiseArray(NoiseGeneratorPerlin *self, double *buf, double xOff,
                                             double yOff, double zOff, int sizeX, int sizeY,
                                             int sizeZ, double scaleX, double scaleY, double scaleZ,
                                             double amplitude) {

    if (sizeY == 1) {

        int idx = 0;
        double inv = 1.0 / amplitude;
        for (int xi = 0; xi < sizeX; xi++) {
            double px = (xOff + xi) * scaleX + self->xCoord;
            int ix = (int)px;
            if (px < ix)
                --ix;
            int X = ix & 255;
            px -= ix;
            double u = fade(px);

            for (int zi = 0; zi < sizeZ; zi++) {
                double pz = (zOff + zi) * scaleZ + self->zCoord;
                int iz = (int)pz;
                if (pz < iz)
                    --iz;
                int Z = iz & 255;
                pz -= iz;
                double w = fade(pz);

                int A = self->permutations[X] + 0;
                int AA = self->permutations[A] + Z;
                int B = self->permutations[X + 1] + 0;
                int BA = self->permutations[B] + Z;

                double n = lerp(u, grad2(self->permutations[AA], px, pz),
                                grad(self->permutations[BA], px - 1, 0, pz));
                double m = lerp(u, grad(self->permutations[AA + 1], px, 0, pz - 1),
                                grad(self->permutations[BA + 1], px - 1, 0, pz - 1));
                buf[idx++] += lerp(w, n, m) * inv;
            }
        }
    } else {

        int idx = 0;
        double inv = 1.0 / amplitude;
        int lastIY = -1;
        double A00 = 0, A01 = 0, A10 = 0, A11 = 0;

        for (int xi = 0; xi < sizeX; xi++) {
            double px = (xOff + xi) * scaleX + self->xCoord;
            int ix = (int)px;
            if (px < ix)
                --ix;
            int X = ix & 255;
            px -= ix;
            double u = fade(px);

            for (int zi = 0; zi < sizeZ; zi++) {
                double pz = (zOff + zi) * scaleZ + self->zCoord;
                int iz = (int)pz;
                if (pz < iz)
                    --iz;
                int Z = iz & 255;
                pz -= iz;
                double w = fade(pz);

                lastIY = -1;

                for (int yi = 0; yi < sizeY; yi++) {
                    double py = (yOff + yi) * scaleY + self->yCoord;
                    int iy = (int)py;
                    if (py < iy)
                        --iy;
                    int Y = iy & 255;
                    py -= iy;
                    double v = fade(py);

                    if (yi == 0 || Y != lastIY) {
                        lastIY = Y;
                        int Ap = self->permutations[X] + Y;
                        int AAp = self->permutations[Ap] + Z;
                        int ABp = self->permutations[Ap + 1] + Z;
                        int Bp = self->permutations[X + 1] + Y;
                        int BAp = self->permutations[Bp] + Z;
                        int BBp = self->permutations[Bp + 1] + Z;
                        A00 = lerp(u, grad(self->permutations[AAp], px, py, pz),
                                   grad(self->permutations[BAp], px - 1, py, pz));
                        A10 = lerp(u, grad(self->permutations[ABp], px, py - 1, pz),
                                   grad(self->permutations[BBp], px - 1, py - 1, pz));
                        A01 = lerp(u, grad(self->permutations[AAp + 1], px, py, pz - 1),
                                   grad(self->permutations[BAp + 1], px - 1, py, pz - 1));
                        A11 = lerp(u, grad(self->permutations[ABp + 1], px, py - 1, pz - 1),
                                   grad(self->permutations[BBp + 1], px - 1, py - 1, pz - 1));
                    }

                    double n = lerp(v, A00, A10);
                    double m = lerp(v, A01, A11);
                    buf[idx++] += lerp(w, n, m) * inv;
                }
            }
        }
    }
}
