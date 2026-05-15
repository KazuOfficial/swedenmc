#include "ImageBufferDownload.h"
#include <string.h>
#include <stdlib.h>

static void *ib_parseUserSkin(ImageBuffer *self, void *var1) {
    return ImageBufferDownload_parseUserSkin((ImageBufferDownload *)self, var1);
}

static const ImageBufferVtable ImageBufferDownload_vtable = {
    .parseUserSkin = ib_parseUserSkin,
};

void ImageBufferDownload_init(ImageBufferDownload *self) {
    memset(self, 0, sizeof(ImageBufferDownload));
    self->base.vtable = &ImageBufferDownload_vtable;
}

static int func_886_c(ImageBufferDownload *self, int var1, int var2, int var3, int var4) {
    for (int var5 = var1; var5 < var3; ++var5) {
        for (int var6 = var2; var6 < var4; ++var6) {
            int var7 = self->imageData[var5 + var6 * self->imageWidth];
            if ((var7 >> 24 & 255) < 128) {
                return 1;
            }
        }
    }
    return 0;
}

static void func_885_a(ImageBufferDownload *self, int var1, int var2, int var3, int var4) {
    if (!func_886_c(self, var1, var2, var3, var4)) {
        for (int var5 = var1; var5 < var3; ++var5) {
            for (int var6 = var2; var6 < var4; ++var6) {
                self->imageData[var5 + var6 * self->imageWidth] &= 16777215;
            }
        }
    }
}

static void func_884_b(ImageBufferDownload *self, int var1, int var2, int var3, int var4) {
    for (int var5 = var1; var5 < var3; ++var5) {
        for (int var6 = var2; var6 < var4; ++var6) {
            self->imageData[var5 + var6 * self->imageWidth] |= -16777216;
        }
    }
}

void *ImageBufferDownload_parseUserSkin(ImageBufferDownload *self, void *var1) {
    if (var1 == NULL) {
        return NULL;
    } else {
        self->imageWidth = 64;
        self->imageHeight = 32;

        free(self->imageData);
        self->imageData = (int *)malloc(64 * 32 * sizeof(int));
        unsigned char *rgba = (unsigned char *)var1;
        for (int i = 0; i < 64 * 32; i++) {
            int r = rgba[i * 4 + 0], g = rgba[i * 4 + 1], b = rgba[i * 4 + 2], a = rgba[i * 4 + 3];
            self->imageData[i] = (int)(((unsigned int)a << 24) | ((unsigned int)r << 16) |
                                       ((unsigned int)g << 8) | (unsigned int)b);
        }

        func_884_b(self, 0, 0, 32, 16);
        func_885_a(self, 32, 0, 64, 32);
        func_884_b(self, 0, 16, 64, 32);
        int var4 = 0;

        int var5;
        int var6;
        int var7;
        for (var5 = 32; var5 < 64; ++var5) {
            for (var6 = 0; var6 < 16; ++var6) {
                var7 = self->imageData[var5 + var6 * 64];
                if ((var7 >> 24 & 255) < 128) {
                    var4 = 1;
                }
            }
        }

        if (!var4) {
            for (var5 = 32; var5 < 64; ++var5) {
                for (var6 = 0; var6 < 16; ++var6) {
                    var7 = self->imageData[var5 + var6 * 64];
                    if ((var7 >> 24 & 255) < 128) {
                        var4 = 1;
                    }
                }
            }
        }

        for (int i = 0; i < 64 * 32; i++) {
            int argb = self->imageData[i];
            rgba[i * 4 + 0] = (argb >> 16) & 255;
            rgba[i * 4 + 1] = (argb >> 8) & 255;
            rgba[i * 4 + 2] = (argb >> 0) & 255;
            rgba[i * 4 + 3] = (argb >> 24) & 255;
        }
        return var1;
    }
}
