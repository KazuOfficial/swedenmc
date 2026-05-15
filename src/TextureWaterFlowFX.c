#include "JavaMath.h"
#include "TextureWaterFlowFX.h"
#include "Block.h"
#include "MathHelper.h"
#include <stdlib.h>

const TextureFXVtable TextureWaterFlowFX_vtable = {TextureWaterFlowFX_onTick};

TextureWaterFlowFX *TextureWaterFlowFX_create(void) {
    TextureWaterFlowFX *self = (TextureWaterFlowFX *)calloc(1, sizeof(TextureWaterFlowFX));
    self->base.vtable = &TextureWaterFlowFX_vtable;
    self->base.iconIndex = Block_waterMoving->blockIndexInTexture + 1;
    self->base.tileSize = 2;
    self->field_1138_g = (float *)calloc(256, sizeof(float));
    self->field_1137_h = (float *)calloc(256, sizeof(float));
    return self;
}

void TextureWaterFlowFX_destroy(TextureWaterFlowFX *self) {
    free(self->field_1138_g);
    free(self->field_1137_h);
    free(self);
}

void TextureWaterFlowFX_onTick(TextureFX *base) {
    TextureWaterFlowFX *self = (TextureWaterFlowFX *)base;
    ++self->field_1134_k;

    int var1;
    int var2;
    float var3;
    int var5;
    int var6;
    for (var1 = 0; var1 < 16; ++var1) {
        for (var2 = 0; var2 < 16; ++var2) {
            var3 = 0.0F;

            int var4;
            for (var4 = var2 - 2; var4 <= var2; ++var4) {
                var5 = var1 & 15;
                var6 = var4 & 15;
                var3 += self->field_1138_g[var5 + var6 * 16];
            }

            self->field_1137_h[var1 + var2 * 16] =
                var3 / 3.2F + self->field_1136_i[var1 + var2 * 16] * 0.8F;
        }
    }

    for (var1 = 0; var1 < 16; ++var1) {
        for (var2 = 0; var2 < 16; ++var2) {
            self->field_1136_i[var1 + var2 * 16] += self->field_1135_j[var1 + var2 * 16] * 0.05F;
            if (self->field_1136_i[var1 + var2 * 16] < 0.0F) {
                self->field_1136_i[var1 + var2 * 16] = 0.0F;
            }
            self->field_1135_j[var1 + var2 * 16] -= 0.3F;
            if ((Math_random()) < 0.2) {
                self->field_1135_j[var1 + var2 * 16] = 0.5F;
            }
        }
    }

    {
        float *var12 = self->field_1137_h;
        self->field_1137_h = self->field_1138_g;
        self->field_1138_g = var12;
    }

    for (var2 = 0; var2 < 256; ++var2) {
        var3 = self->field_1138_g[(var2 - self->field_1134_k * 16) & 255];
        if (var3 > 1.0F) {
            var3 = 1.0F;
        }
        if (var3 < 0.0F) {
            var3 = 0.0F;
        }

        float var13 = var3 * var3;
        var5 = (int)(32.0F + var13 * 32.0F);
        var6 = (int)(50.0F + var13 * 64.0F);
        int var7 = 255;
        int var8 = (int)(146.0F + var13 * 50.0F);
        if (self->base.anaglyphEnabled) {
            int var9 = (var5 * 30 + var6 * 59 + var7 * 11) / 100;
            int var10 = (var5 * 30 + var6 * 70) / 100;
            int var11 = (var5 * 30 + var7 * 70) / 100;
            var5 = var9;
            var6 = var10;
            var7 = var11;
        }

        self->base.imageData[var2 * 4 + 0] = (signed char)var5;
        self->base.imageData[var2 * 4 + 1] = (signed char)var6;
        self->base.imageData[var2 * 4 + 2] = (signed char)var7;
        self->base.imageData[var2 * 4 + 3] = (signed char)var8;
    }
}
