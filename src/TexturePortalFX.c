#include "TexturePortalFX.h"
#include "Block.h"
#include "MathHelper.h"
#include "JavaRandom.h"
#include <stdlib.h>
#include <math.h>

const TextureFXVtable TexturePortalFX_vtable = {TexturePortalFX_onTick};

TexturePortalFX *TexturePortalFX_create(void) {
    TexturePortalFX *self = (TexturePortalFX *)calloc(1, sizeof(TexturePortalFX));
    self->base.vtable = &TexturePortalFX_vtable;
    self->base.iconIndex = Block_portal->blockIndexInTexture;
    self->base.tileSize = 1;

    JavaRandom var1;
    JavaRandom_init(&var1, 100L);

    int var2;
    for (var2 = 0; var2 < 32; ++var2) {
        int var3;
        for (var3 = 0; var3 < 16; ++var3) {
            int var4;
            for (var4 = 0; var4 < 16; ++var4) {
                float var5 = 0.0F;

                int var6;
                for (var6 = 0; var6 < 2; ++var6) {
                    float var7 = (float)(var6 * 8);
                    float var8 = (float)(var6 * 8);
                    float var9 = ((float)var3 - var7) / 16.0F * 2.0F;
                    float var10 = ((float)var4 - var8) / 16.0F * 2.0F;
                    if (var9 < -1.0F) {
                        var9 += 2.0F;
                    }
                    if (var9 >= 1.0F) {
                        var9 -= 2.0F;
                    }
                    if (var10 < -1.0F) {
                        var10 += 2.0F;
                    }
                    if (var10 >= 1.0F) {
                        var10 -= 2.0F;
                    }

                    float var11 = var9 * var9 + var10 * var10;
                    float var12 = (float)atan2((double)var10, (double)var9) +
                                  ((float)var2 / 32.0F * (float)M_PI * 2.0F - var11 * 10.0F +
                                   (float)(var6 * 2)) *
                                      (float)(var6 * 2 - 1);
                    var12 = (MathHelper_sin(var12) + 1.0F) / 2.0F;
                    var12 /= var11 + 1.0F;
                    var5 += var12 * 0.5F;
                }

                var5 += JavaRandom_nextFloat(&var1) * 0.1F;
                var6 = (int)(var5 * 100.0F + 155.0F);
                int var13 = (int)(var5 * var5 * 200.0F + 55.0F);
                int var14 = (int)(var5 * var5 * var5 * var5 * 255.0F);
                int var15 = (int)(var5 * 100.0F + 155.0F);
                int var16 = var4 * 16 + var3;
                self->portalTextureData[var2][var16 * 4 + 0] = (signed char)var13;
                self->portalTextureData[var2][var16 * 4 + 1] = (signed char)var14;
                self->portalTextureData[var2][var16 * 4 + 2] = (signed char)var6;
                self->portalTextureData[var2][var16 * 4 + 3] = (signed char)var15;
            }
        }
    }

    return self;
}

void TexturePortalFX_destroy(TexturePortalFX *self) { free(self); }

void TexturePortalFX_onTick(TextureFX *base) {
    TexturePortalFX *self = (TexturePortalFX *)base;
    ++self->portalTickCounter;
    signed char *var1 = self->portalTextureData[self->portalTickCounter & 31];

    int var2;
    for (var2 = 0; var2 < 256; ++var2) {
        int var3 = var1[var2 * 4 + 0] & 255;
        int var4 = var1[var2 * 4 + 1] & 255;
        int var5 = var1[var2 * 4 + 2] & 255;
        int var6 = var1[var2 * 4 + 3] & 255;
        if (self->base.anaglyphEnabled) {
            int var7 = (var3 * 30 + var4 * 59 + var5 * 11) / 100;
            int var8 = (var3 * 30 + var4 * 70) / 100;
            int var9 = (var3 * 30 + var5 * 70) / 100;
            var3 = var7;
            var4 = var8;
            var5 = var9;
        }
        self->base.imageData[var2 * 4 + 0] = (signed char)var3;
        self->base.imageData[var2 * 4 + 1] = (signed char)var4;
        self->base.imageData[var2 * 4 + 2] = (signed char)var5;
        self->base.imageData[var2 * 4 + 3] = (signed char)var6;
    }
}
