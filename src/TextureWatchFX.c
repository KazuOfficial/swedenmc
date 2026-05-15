#include "JavaMath.h"
#include "TextureWatchFX.h"
#include "Minecraft.h"
#include "World.h"
#include "Item.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <math.h>
#include "../lib/stb_image.h"

const TextureFXVtable TextureWatchFX_vtable = {TextureWatchFX_onTick};

TextureWatchFX *TextureWatchFX_create(struct Minecraft *var1) {
    TextureWatchFX *self = (TextureWatchFX *)calloc(1, sizeof(TextureWatchFX));
    self->base.vtable = &TextureWatchFX_vtable;
    self->base.iconIndex = Item_pocketSundial->vtable->getIconFromDamage(Item_pocketSundial, 0);
    self->base.tileSize = 1;
    self->base.tileImage = 1;
    self->mc = var1;

    {
        int w, h, comp;
        unsigned char *var2 = stbi_load("gui/items.png", &w, &h, &comp, 4);
        if (var2) {
            int var3 = self->base.iconIndex % 16 * 16;
            int var4 = self->base.iconIndex / 16 * 16;
            for (int dy = 0; dy < 16; dy++) {
                for (int dx = 0; dx < 16; dx++) {
                    int si = ((var4 + dy) * w + (var3 + dx)) * 4;
                    int a = (unsigned char)var2[si + 3];
                    int r = (unsigned char)var2[si + 0];
                    int g = (unsigned char)var2[si + 1];
                    int b = (unsigned char)var2[si + 2];
                    self->watchIconImageData[dy * 16 + dx] =
                        (int)(((unsigned int)a << 24) | ((unsigned int)r << 16) |
                              ((unsigned int)g << 8) | (unsigned int)b);
                }
            }
            stbi_image_free(var2);
        }
        var2 = stbi_load("misc/dial.png", &w, &h, &comp, 4);
        if (var2) {
            for (int dy = 0; dy < 16; dy++) {
                for (int dx = 0; dx < 16; dx++) {
                    int si = (dy * w + dx) * 4;
                    int a = (unsigned char)var2[si + 3];
                    int r = (unsigned char)var2[si + 0];
                    int g = (unsigned char)var2[si + 1];
                    int b = (unsigned char)var2[si + 2];
                    self->dialImageData[dy * 16 + dx] =
                        (int)(((unsigned int)a << 24) | ((unsigned int)r << 16) |
                              ((unsigned int)g << 8) | (unsigned int)b);
                }
            }
            stbi_image_free(var2);
        }
    }
    return self;
}

void TextureWatchFX_destroy(TextureWatchFX *self) { free(self); }

void TextureWatchFX_onTick(TextureFX *base) {
    TextureWatchFX *self = (TextureWatchFX *)base;
    double var1 = 0.0;
    if (self->mc->theWorld != NULL && self->mc->thePlayer != NULL) {
        float var3 = World_getCelestialAngle(self->mc->theWorld, 1.0F);
        var1 = (double)(-var3 * (float)M_PI * 2.0F);
        if (self->mc->theWorld->worldProvider->isNether) {
            var1 = (Math_random()) * (double)((float)M_PI) * 2.0;
        }
    }

    double var22;
    for (var22 = var1 - self->field_4222_j; var22 < -M_PI; var22 += M_PI * 2.0) {
    }

    while (var22 >= M_PI) {
        var22 -= M_PI * 2.0;
    }

    if (var22 < -1.0) {
        var22 = -1.0;
    }

    if (var22 > 1.0) {
        var22 = 1.0;
    }

    self->field_4221_k += var22 * 0.1;
    self->field_4221_k *= 0.8;
    self->field_4222_j += self->field_4221_k;
    double var5 = sin(self->field_4222_j);
    double var7 = cos(self->field_4222_j);

    int var9;
    for (var9 = 0; var9 < 256; ++var9) {
        int var10 = self->watchIconImageData[var9] >> 24 & 255;
        int var11 = self->watchIconImageData[var9] >> 16 & 255;
        int var12 = self->watchIconImageData[var9] >> 8 & 255;
        int var13 = self->watchIconImageData[var9] >> 0 & 255;
        if (var11 == var13 && var12 == 0 && var13 > 0) {
            double var14 = -((double)(var9 % 16) / 15.0 - 0.5);
            double var16 = (double)(var9 / 16) / 15.0 - 0.5;
            int var18 = var11;
            int var19 = (int)((var14 * var7 + var16 * var5 + 0.5) * 16.0);
            int var20 = (int)((var16 * var7 - var14 * var5 + 0.5) * 16.0);
            int var21 = (var19 & 15) + (var20 & 15) * 16;
            var10 = self->dialImageData[var21] >> 24 & 255;
            var11 = (self->dialImageData[var21] >> 16 & 255) * var11 / 255;
            var12 = (self->dialImageData[var21] >> 8 & 255) * var18 / 255;
            var13 = (self->dialImageData[var21] >> 0 & 255) * var18 / 255;
        }

        if (self->base.anaglyphEnabled) {
            int var23 = (var11 * 30 + var12 * 59 + var13 * 11) / 100;
            int var15 = (var11 * 30 + var12 * 70) / 100;
            int var24 = (var11 * 30 + var13 * 70) / 100;
            var11 = var23;
            var12 = var15;
            var13 = var24;
        }

        self->base.imageData[var9 * 4 + 0] = (signed char)var11;
        self->base.imageData[var9 * 4 + 1] = (signed char)var12;
        self->base.imageData[var9 * 4 + 2] = (signed char)var13;
        self->base.imageData[var9 * 4 + 3] = (signed char)var10;
    }
}
