#ifndef MATH_HELPER_H
#define MATH_HELPER_H

void MathHelper_init(void);

float MathHelper_sin(float v);
float MathHelper_cos(float v);
float MathHelper_sqrt_float(float v);
float MathHelper_sqrt_double(double v);
int MathHelper_floor_float(float v);
int MathHelper_floor_double(double v);
float MathHelper_abs(float v);
double MathHelper_abs_max(double a, double b);
int MathHelper_bucketInt(int val, int bucket);
int MathHelper_stringNullOrLengthZero(const char *s);

#endif
