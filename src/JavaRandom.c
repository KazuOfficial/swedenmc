#include "JavaRandom.h"
#include <math.h>
#include <time.h>

#define MULTIPLIER INT64_C(0x5DEECE66D)
#define ADDEND INT64_C(0xB)
#define MASK ((INT64_C(1) << 48) - 1)

static int64_t initialScramble(int64_t seed) { return (seed ^ MULTIPLIER) & MASK; }

void JavaRandom_init(JavaRandom *r, int64_t seed) { r->seed = initialScramble(seed); }

static int64_t java_seedUniquifier = INT64_C(8682522807148012);
void JavaRandom_new(JavaRandom *r) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    int64_t nanoTime = (int64_t)ts.tv_sec * INT64_C(1000000000) + ts.tv_nsec;
    JavaRandom_init(r, ++java_seedUniquifier + nanoTime);
}

int JavaRandom_next(JavaRandom *r, int bits) {
    r->seed =
        (int64_t)(((uint64_t)r->seed * (uint64_t)MULTIPLIER + (uint64_t)ADDEND) & (uint64_t)MASK);
    return (int)(r->seed >> (48 - bits));
}

int JavaRandom_nextIntRaw(JavaRandom *r) { return JavaRandom_next(r, 32); }

int JavaRandom_nextInt(JavaRandom *r, int bound) {
    int bits, val;
    if (bound <= 0)
        return 0;
    if ((bound & -bound) == bound)
        return (int)((bound * (int64_t)JavaRandom_next(r, 31)) >> 31);
    do {
        bits = JavaRandom_next(r, 31);
        val = bits % bound;
    } while ((int)((unsigned int)bits - (unsigned int)val + (unsigned int)(bound - 1)) < 0);
    return val;
}

int64_t JavaRandom_nextLong(JavaRandom *r) {
    return (int64_t)(((uint64_t)(uint32_t)JavaRandom_next(r, 32) << 32) |
                     (uint32_t)JavaRandom_next(r, 32));
}

float JavaRandom_nextFloat(JavaRandom *r) { return JavaRandom_next(r, 24) / (float)(1 << 24); }

double JavaRandom_nextDouble(JavaRandom *r) {
    return (((int64_t)JavaRandom_next(r, 26) << 27) + JavaRandom_next(r, 27)) /
           (double)(INT64_C(1) << 53);
}

int JavaRandom_nextBoolean(JavaRandom *r) { return JavaRandom_next(r, 1) != 0; }

double JavaRandom_nextGaussian(JavaRandom *r) {
    double v1, v2, s;
    do {
        v1 = 2.0 * JavaRandom_nextDouble(r) - 1.0;
        v2 = 2.0 * JavaRandom_nextDouble(r) - 1.0;
        s = v1 * v1 + v2 * v2;
    } while (s >= 1.0 || s == 0.0);
    double multiplier = sqrt(-2.0 * log(s) / s);
    return v1 * multiplier;
}
