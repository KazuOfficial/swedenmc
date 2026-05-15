#include "MathHelper.h"
#include <math.h>
#include <stddef.h>

static float SIN_TABLE[65536];

void MathHelper_init(void) {
    int i;
    for (i = 0; i < 65536; i++) {
        SIN_TABLE[i] = (float)sin((double)i * M_PI * 2.0 / 65536.0);
    }
}

float MathHelper_sin(float v) {
    return SIN_TABLE[(int)(fmodf(v * 10430.378f, 65536.0f) + 65536.0f) & 0xffff];
}

float MathHelper_cos(float v) {
    return SIN_TABLE[(int)(fmodf(v * 10430.378f + 16384.0f, 65536.0f) + 65536.0f) & 0xffff];
}

float MathHelper_sqrt_float(float v) { return (float)sqrt((double)v); }

float MathHelper_sqrt_double(double v) { return (float)sqrt(v); }

int MathHelper_floor_float(float v) {
    int i = (int)v;
    return v < (float)i ? i - 1 : i;
}

int MathHelper_floor_double(double v) {
    int i = (int)v;
    return v < (double)i ? i - 1 : i;
}

float MathHelper_abs(float v) { return v >= 0.0f ? v : -v; }

double MathHelper_abs_max(double a, double b) {
    if (a < 0.0)
        a = -a;
    if (b < 0.0)
        b = -b;
    return a > b ? a : b;
}

int MathHelper_bucketInt(int val, int bucket) {
    return val < 0 ? -((-val - 1) / bucket) - 1 : val / bucket;
}

int MathHelper_stringNullOrLengthZero(const char *s) { return s == NULL || s[0] == '\0'; }
