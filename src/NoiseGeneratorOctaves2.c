#include "NoiseGeneratorOctaves2.h"
#include <stdlib.h>
#include <string.h>

void NoiseGeneratorOctaves2_init(NoiseGeneratorOctaves2 *self, JavaRandom *rand, int octaves) {
    self->field_4233_b = octaves;
    self->field_4234_a = (NoiseGenerator2 *)malloc((size_t)octaves * sizeof(NoiseGenerator2));
    for (int var3 = 0; var3 < octaves; ++var3) {
        NoiseGenerator2_init(&self->field_4234_a[var3], rand);
    }
}

void NoiseGeneratorOctaves2_free(NoiseGeneratorOctaves2 *self) {
    free(self->field_4234_a);
    self->field_4234_a = NULL;
}

double *NoiseGeneratorOctaves2_func_4112_a(NoiseGeneratorOctaves2 *self, double *var1, double var2,
                                           double var4, int var6, int var7, double var8,
                                           double var10, double var12) {
    return NoiseGeneratorOctaves2_func_4111_a(self, var1, var2, var4, var6, var7, var8, var10,
                                              var12, 0.5);
}

double *NoiseGeneratorOctaves2_func_4111_a(NoiseGeneratorOctaves2 *self, double *var1, double var2,
                                           double var4, int var6, int var7, double var8,
                                           double var10, double var12, double var14) {
    var8 /= 1.5;
    var10 /= 1.5;
    int len = var6 * var7;
    if (var1 != NULL) {
        for (int var16 = 0; var16 < len; ++var16) {
            var1[var16] = 0.0;
        }
    } else {
        var1 = (double *)calloc((size_t)len, sizeof(double));
    }

    double var21 = 1.0;
    double var18 = 1.0;

    for (int var20 = 0; var20 < self->field_4233_b; ++var20) {
        NoiseGenerator2_func_4157_a(&self->field_4234_a[var20], var1, var2, var4, var6, var7,
                                    var8 * var18, var10 * var18, 0.55 / var21);
        var18 *= var12;
        var21 *= var14;
    }

    return var1;
}
