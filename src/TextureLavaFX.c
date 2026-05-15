#include "JavaMath.h"
#include "TextureLavaFX.h"
#include "Block.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <math.h>

const TextureFXVtable TextureLavaFX_vtable = {TextureLavaFX_onTick};

TextureLavaFX *TextureLavaFX_create(void) {
    TextureLavaFX *self = (TextureLavaFX *)calloc(1, sizeof(TextureLavaFX));
    self->base.vtable = &TextureLavaFX_vtable;
    self->base.iconIndex = Block_lavaMoving->blockIndexInTexture;
    self->base.tileSize = 1;
    self->field_1147_g = (float *)calloc(256, sizeof(float));
    self->field_1146_h = (float *)calloc(256, sizeof(float));
    return self;
}

void TextureLavaFX_destroy(TextureLavaFX *self) {
    free(self->field_1147_g);
    free(self->field_1146_h);
    free(self);
}

void TextureLavaFX_onTick(TextureFX *base) {
    TextureLavaFX *self = (TextureLavaFX *)base;
    int var2;
    float var3;
    int var5;
    int var6;
    int var7;
    int var8;
    int var9;
    int var1;
    for (var1 = 0; var1 < 16; ++var1) {
        for (var2 = 0; var2 < 16; ++var2) {
            var3 = 0.0F;
            int var4 = (int)(MathHelper_sin((float)var2 * (float)M_PI * 2.0F / 16.0F) * 1.2F);
            var5 = (int)(MathHelper_sin((float)var1 * (float)M_PI * 2.0F / 16.0F) * 1.2F);

            for (var6 = var1 - 1; var6 <= var1 + 1; ++var6) {
                for (var7 = var2 - 1; var7 <= var2 + 1; ++var7) {
                    var8 = (var6 + var4) & 15;
                    var9 = (var7 + var5) & 15;
                    var3 += self->field_1147_g[var8 + var9 * 16];
                }
            }

            self->field_1146_h[var1 + var2 * 16] =
                var3 / 10.0F + (self->field_1145_i[((var1 + 0) & 15) + ((var2 + 0) & 15) * 16] +
                                self->field_1145_i[((var1 + 1) & 15) + ((var2 + 0) & 15) * 16] +
                                self->field_1145_i[((var1 + 1) & 15) + ((var2 + 1) & 15) * 16] +
                                self->field_1145_i[((var1 + 0) & 15) + ((var2 + 1) & 15) * 16]) /
                                   4.0F * 0.8F;
            self->field_1145_i[var1 + var2 * 16] += self->field_1144_j[var1 + var2 * 16] * 0.01F;
            if (self->field_1145_i[var1 + var2 * 16] < 0.0F) {
                self->field_1145_i[var1 + var2 * 16] = 0.0F;
            }
            self->field_1144_j[var1 + var2 * 16] -= 0.06F;
            if ((Math_random()) < 0.005) {
                self->field_1144_j[var1 + var2 * 16] = 1.5F;
            }
        }
    }

    {
        float *var11 = self->field_1146_h;
        self->field_1146_h = self->field_1147_g;
        self->field_1147_g = var11;
    }

    for (var2 = 0; var2 < 256; ++var2) {
        var3 = self->field_1147_g[var2] * 2.0F;
        if (var3 > 1.0F) {
            var3 = 1.0F;
        }
        if (var3 < 0.0F) {
            var3 = 0.0F;
        }

        var5 = (int)(var3 * 100.0F + 155.0F);
        var6 = (int)(var3 * var3 * 255.0F);
        var7 = (int)(var3 * var3 * var3 * var3 * 128.0F);
        if (self->base.anaglyphEnabled) {
            var8 = (var5 * 30 + var6 * 59 + var7 * 11) / 100;
            var9 = (var5 * 30 + var6 * 70) / 100;
            int var10 = (var5 * 30 + var7 * 70) / 100;
            var5 = var8;
            var6 = var9;
            var7 = var10;
        }

        self->base.imageData[var2 * 4 + 0] = (signed char)var5;
        self->base.imageData[var2 * 4 + 1] = (signed char)var6;
        self->base.imageData[var2 * 4 + 2] = (signed char)var7;
        self->base.imageData[var2 * 4 + 3] = -1;
    }
}
