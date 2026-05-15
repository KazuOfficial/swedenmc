#include "JavaMath.h"
#include "TextureFlamesFX.h"
#include "Block.h"
#include "MathHelper.h"
#include <stdlib.h>

const TextureFXVtable TextureFlamesFX_vtable = {TextureFlamesFX_onTick};

TextureFlamesFX *TextureFlamesFX_create(int var1) {
    TextureFlamesFX *self = (TextureFlamesFX *)calloc(1, sizeof(TextureFlamesFX));
    self->base.vtable = &TextureFlamesFX_vtable;
    self->base.iconIndex = Block_fire->blockIndexInTexture + var1 * 16;
    self->base.tileSize = 1;
    self->field_1133_g = (float *)calloc(320, sizeof(float));
    self->field_1132_h = (float *)calloc(320, sizeof(float));
    return self;
}

void TextureFlamesFX_destroy(TextureFlamesFX *self) {
    free(self->field_1133_g);
    free(self->field_1132_h);
    free(self);
}

void TextureFlamesFX_onTick(TextureFX *base) {
    TextureFlamesFX *self = (TextureFlamesFX *)base;
    int var2;
    float var4;
    int var5;
    int var6;
    int var1;
    for (var1 = 0; var1 < 16; ++var1) {
        for (var2 = 0; var2 < 20; ++var2) {
            int var3 = 18;
            var4 = self->field_1133_g[var1 + (var2 + 1) % 20 * 16] * (float)var3;

            for (var5 = var1 - 1; var5 <= var1 + 1; ++var5) {
                for (var6 = var2; var6 <= var2 + 1; ++var6) {
                    if (var5 >= 0 && var6 >= 0 && var5 < 16 && var6 < 20) {
                        var4 += self->field_1133_g[var5 + var6 * 16];
                    }
                    ++var3;
                }
            }

            self->field_1132_h[var1 + var2 * 16] = var4 / ((float)var3 * 1.06F);
            if (var2 >= 19) {
                self->field_1132_h[var1 + var2 * 16] =
                    (float)((Math_random()) * (Math_random()) * (Math_random()) * 4.0 +
                            (Math_random()) * (double)0.1F + (double)0.2F);
            }
        }
    }

    {
        float *var12 = self->field_1132_h;
        self->field_1132_h = self->field_1133_g;
        self->field_1133_g = var12;
    }

    for (var2 = 0; var2 < 256; ++var2) {
        float var13 = self->field_1133_g[var2] * 1.8F;
        if (var13 > 1.0F) {
            var13 = 1.0F;
        }
        if (var13 < 0.0F) {
            var13 = 0.0F;
        }

        var5 = (int)(var13 * 155.0F + 100.0F);
        var6 = (int)(var13 * var13 * 255.0F);
        int var7 = (int)(var13 * var13 * var13 * var13 * var13 * var13 * var13 * var13 * var13 *
                         var13 * 255.0F);
        int var8 = 255;
        if (var13 < 0.5F) {
            var8 = 0;
        }

        var4 = (var13 - 0.5F) * 2.0F;
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
