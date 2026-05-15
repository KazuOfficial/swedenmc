#include "JavaMath.h"
#include "TextureCompassFX.h"
#include "Minecraft.h"
#include "World.h"
#include "Entity.h"
#include "Item.h"
#include "MathHelper.h"
#include <stdlib.h>
#include <math.h>
#include "../lib/stb_image.h"

const TextureFXVtable TextureCompassFX_vtable = {TextureCompassFX_onTick};

TextureCompassFX *TextureCompassFX_create(struct Minecraft *var1) {
    TextureCompassFX *self = (TextureCompassFX *)calloc(1, sizeof(TextureCompassFX));

    self->base.vtable = &TextureCompassFX_vtable;
    self->base.iconIndex = Item_compass->vtable->getIconFromDamage(Item_compass, 0);
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
                    self->compassIconImageData[dy * 16 + dx] =
                        (int)(((unsigned int)a << 24) | ((unsigned int)r << 16) |
                              ((unsigned int)g << 8) | (unsigned int)b);
                }
            }
            stbi_image_free(var2);
        }
    }
    return self;
}

void TextureCompassFX_destroy(TextureCompassFX *self) { free(self); }

void TextureCompassFX_onTick(TextureFX *base) {
    TextureCompassFX *self = (TextureCompassFX *)base;
    int var1;
    for (var1 = 0; var1 < 256; ++var1) {
        int var2 = self->compassIconImageData[var1] >> 24 & 255;
        int var3 = self->compassIconImageData[var1] >> 16 & 255;
        int var4 = self->compassIconImageData[var1] >> 8 & 255;
        int var5 = self->compassIconImageData[var1] >> 0 & 255;
        if (self->base.anaglyphEnabled) {
            int var6 = (var3 * 30 + var4 * 59 + var5 * 11) / 100;
            int var7 = (var3 * 30 + var4 * 70) / 100;
            int var8 = (var3 * 30 + var5 * 70) / 100;
            var3 = var6;
            var4 = var7;
            var5 = var8;
        }
        self->base.imageData[var1 * 4 + 0] = (signed char)var3;
        self->base.imageData[var1 * 4 + 1] = (signed char)var4;
        self->base.imageData[var1 * 4 + 2] = (signed char)var5;
        self->base.imageData[var1 * 4 + 3] = (signed char)var2;
    }

    double var20 = 0.0;
    if (self->mc->theWorld != NULL && self->mc->thePlayer != NULL) {
        ChunkCoordinates var21 = World_getSpawnPoint(self->mc->theWorld);
        double var23 = (double)var21.x - ((Entity *)self->mc->thePlayer)->posX;
        double var25 = (double)var21.z - ((Entity *)self->mc->thePlayer)->posZ;
        var20 = (double)(((Entity *)self->mc->thePlayer)->rotationYaw - 90.0F) * M_PI / 180.0 -
                atan2(var25, var23);
        if (self->mc->theWorld->worldProvider->isNether) {
            var20 = (Math_random()) * (double)((float)M_PI) * 2.0;
        }
    }

    double var22;
    for (var22 = var20 - self->field_4229_i; var22 < -M_PI; var22 += M_PI * 2.0) {
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

    self->field_4228_j += var22 * 0.1;
    self->field_4228_j *= 0.8;
    self->field_4229_i += self->field_4228_j;
    double var24 = sin(self->field_4229_i);
    double var26 = cos(self->field_4229_i);

    int var9;
    int var10;
    int var11;
    int var12;
    int var13;
    int var14;
    int var15;
    int var16;
    int var17;
    int var18;
    int var19;
    for (var9 = -4; var9 <= 4; ++var9) {
        var10 = (int)(8.5 + var26 * (double)var9 * 0.3);
        var11 = (int)(7.5 - var24 * (double)var9 * 0.3 * 0.5);
        var12 = var11 * 16 + var10;
        var13 = 100;
        var14 = 100;
        var15 = 100;
        var16 = 255;
        if (self->base.anaglyphEnabled) {
            var17 = (var13 * 30 + var14 * 59 + var15 * 11) / 100;
            var18 = (var13 * 30 + var14 * 70) / 100;
            var19 = (var13 * 30 + var15 * 70) / 100;
            var13 = var17;
            var14 = var18;
            var15 = var19;
        }
        self->base.imageData[var12 * 4 + 0] = (signed char)var13;
        self->base.imageData[var12 * 4 + 1] = (signed char)var14;
        self->base.imageData[var12 * 4 + 2] = (signed char)var15;
        self->base.imageData[var12 * 4 + 3] = (signed char)var16;
    }

    for (var9 = -8; var9 <= 16; ++var9) {
        var10 = (int)(8.5 + var24 * (double)var9 * 0.3);
        var11 = (int)(7.5 + var26 * (double)var9 * 0.3 * 0.5);
        var12 = var11 * 16 + var10;
        var13 = var9 >= 0 ? 255 : 100;
        var14 = var9 >= 0 ? 20 : 100;
        var15 = var9 >= 0 ? 20 : 100;
        var16 = 255;
        if (self->base.anaglyphEnabled) {
            var17 = (var13 * 30 + var14 * 59 + var15 * 11) / 100;
            var18 = (var13 * 30 + var14 * 70) / 100;
            var19 = (var13 * 30 + var15 * 70) / 100;
            var13 = var17;
            var14 = var18;
            var15 = var19;
        }
        self->base.imageData[var12 * 4 + 0] = (signed char)var13;
        self->base.imageData[var12 * 4 + 1] = (signed char)var14;
        self->base.imageData[var12 * 4 + 2] = (signed char)var15;
        self->base.imageData[var12 * 4 + 3] = (signed char)var16;
    }
}
