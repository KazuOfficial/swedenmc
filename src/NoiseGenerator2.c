#include "NoiseGenerator2.h"
#include <math.h>

static const int field_4296_d[12][3] = {{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
                                        {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
                                        {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}};

#define FIELD_4294_F (0.5 * (1.7320508075688772 - 1.0))
#define FIELD_4293_G ((3.0 - 1.7320508075688772) / 6.0)

static int wrap(double var0) { return var0 > 0.0 ? (int)var0 : (int)var0 - 1; }

static double func_4156_a(const int var0[3], double var1, double var3) {
    return (double)var0[0] * var1 + (double)var0[1] * var3;
}

void NoiseGenerator2_init(NoiseGenerator2 *self, JavaRandom *rand) {
    self->field_4292_a = JavaRandom_nextDouble(rand) * 256.0;
    self->field_4291_b = JavaRandom_nextDouble(rand) * 256.0;
    self->field_4297_c = JavaRandom_nextDouble(rand) * 256.0;

    int var2;
    for (var2 = 0; var2 < 256; ++var2) {
        self->field_4295_e[var2] = var2;
    }

    for (var2 = 0; var2 < 256; ++var2) {
        int var3 = JavaRandom_nextInt(rand, 256 - var2) + var2;
        int var4 = self->field_4295_e[var2];
        self->field_4295_e[var2] = self->field_4295_e[var3];
        self->field_4295_e[var3] = var4;
        self->field_4295_e[var2 + 256] = self->field_4295_e[var2];
    }
}

void NoiseGenerator2_func_4157_a(NoiseGenerator2 *self, double *var1, double var2, double var4,
                                 int var6, int var7, double var8, double var10, double var12) {
    int var14 = 0;

    for (int var15 = 0; var15 < var6; ++var15) {
        double var16 = (var2 + (double)var15) * var8 + self->field_4292_a;

        for (int var18 = 0; var18 < var7; ++var18) {
            double var19 = (var4 + (double)var18) * var10 + self->field_4291_b;
            double var27 = (var16 + var19) * FIELD_4294_F;
            int var29 = wrap(var16 + var27);
            int var30 = wrap(var19 + var27);
            double var31 = (double)(var29 + var30) * FIELD_4293_G;
            double var33 = (double)var29 - var31;
            double var35 = (double)var30 - var31;
            double var37 = var16 - var33;
            double var39 = var19 - var35;
            int var41, var42;
            if (var37 > var39) {
                var41 = 1;
                var42 = 0;
            } else {
                var41 = 0;
                var42 = 1;
            }

            double var43 = var37 - (double)var41 + FIELD_4293_G;
            double var45 = var39 - (double)var42 + FIELD_4293_G;
            double var47 = var37 - 1.0 + 2.0 * FIELD_4293_G;
            double var49 = var39 - 1.0 + 2.0 * FIELD_4293_G;
            int var51 = var29 & 255;
            int var52 = var30 & 255;
            int var53 = self->field_4295_e[var51 + self->field_4295_e[var52]] % 12;
            int var54 = self->field_4295_e[var51 + var41 + self->field_4295_e[var52 + var42]] % 12;
            int var55 = self->field_4295_e[var51 + 1 + self->field_4295_e[var52 + 1]] % 12;

            double var56 = 0.5 - var37 * var37 - var39 * var39;
            double var21;
            if (var56 < 0.0) {
                var21 = 0.0;
            } else {
                var56 *= var56;
                var21 = var56 * var56 * func_4156_a(field_4296_d[var53], var37, var39);
            }

            double var58 = 0.5 - var43 * var43 - var45 * var45;
            double var23;
            if (var58 < 0.0) {
                var23 = 0.0;
            } else {
                var58 *= var58;
                var23 = var58 * var58 * func_4156_a(field_4296_d[var54], var43, var45);
            }

            double var60 = 0.5 - var47 * var47 - var49 * var49;
            double var25;
            if (var60 < 0.0) {
                var25 = 0.0;
            } else {
                var60 *= var60;
                var25 = var60 * var60 * func_4156_a(field_4296_d[var55], var47, var49);
            }

            var1[var14++] += 70.0 * (var21 + var23 + var25) * var12;
        }
    }
}
