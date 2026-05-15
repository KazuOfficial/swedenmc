#include "JavaMath.h"
#include "TextureWaterFX.h"
#include "Block.h"
#include "MathHelper.h"
#include <stdlib.h>

const TextureFXVtable TextureWaterFX_vtable = {TextureWaterFX_onTick};

TextureWaterFX *TextureWaterFX_create(void) {
    TextureWaterFX *self = (TextureWaterFX *)calloc(1, sizeof(TextureWaterFX));
    self->base.vtable = &TextureWaterFX_vtable;
    self->base.iconIndex = Block_waterMoving->blockIndexInTexture;
    self->base.tileSize = 1;
    self->field_1158_g = (float *)calloc(256, sizeof(float));
    self->field_1157_h = (float *)calloc(256, sizeof(float));
    return self;
}

void TextureWaterFX_destroy(TextureWaterFX *self) {
    free(self->field_1158_g);
    free(self->field_1157_h);
    free(self);
}

void TextureWaterFX_onTick(TextureFX *base) {
    TextureWaterFX *self = (TextureWaterFX *)base;
    ++self->tickCounter;

    int var1;
    int var2;
    float var3;
    int var5;
    int var6;
    for (var1 = 0; var1 < 16; ++var1) {
        for (var2 = 0; var2 < 16; ++var2) {
            var3 = 0.0F;

            int var4;
            for (var4 = var1 - 1; var4 <= var1 + 1; ++var4) {
                var5 = var4 & 15;
                var6 = var2 & 15;
                var3 += self->field_1158_g[var5 + var6 * 16];
            }

            self->field_1157_h[var1 + var2 * 16] =
                var3 / 3.3F + self->field_1156_i[var1 + var2 * 16] * 0.8F;
        }
    }

    for (var1 = 0; var1 < 16; ++var1) {
        for (var2 = 0; var2 < 16; ++var2) {
            self->field_1156_i[var1 + var2 * 16] += self->field_1155_j[var1 + var2 * 16] * 0.05F;
            if (self->field_1156_i[var1 + var2 * 16] < 0.0F) {
                self->field_1156_i[var1 + var2 * 16] = 0.0F;
            }
            self->field_1155_j[var1 + var2 * 16] -= 0.1F;
            if ((Math_random()) < 0.05) {
                self->field_1155_j[var1 + var2 * 16] = 0.5F;
            }
        }
    }

    {
        float *var12 = self->field_1157_h;
        self->field_1157_h = self->field_1158_g;
        self->field_1158_g = var12;
    }

    for (var2 = 0; var2 < 256; ++var2) {
        var3 = self->field_1158_g[var2];
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
