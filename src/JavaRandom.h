#ifndef JAVA_RANDOM_H
#define JAVA_RANDOM_H

#include <stdint.h>

typedef struct JavaRandom {
    int64_t seed;
} JavaRandom;

void JavaRandom_init(JavaRandom *r, int64_t seed);

void JavaRandom_new(JavaRandom *r);

static inline void JavaRandom_setSeed(JavaRandom *r, int64_t seed) { JavaRandom_init(r, seed); }
int JavaRandom_next(JavaRandom *r, int bits);
int JavaRandom_nextIntRaw(JavaRandom *r);
int JavaRandom_nextInt(JavaRandom *r, int bound);
int64_t JavaRandom_nextLong(JavaRandom *r);
float JavaRandom_nextFloat(JavaRandom *r);
double JavaRandom_nextDouble(JavaRandom *r);
int JavaRandom_nextBoolean(JavaRandom *r);
double JavaRandom_nextGaussian(JavaRandom *r);

#endif
