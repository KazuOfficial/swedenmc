#include "ScreenShotHelper.h"
#include "Renderer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "platform.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/glfw/deps/stb_image_write.h"

static uint8_t *s_buffer = NULL;
static int s_bufferCap = 0;

static uint8_t *s_pixelData = NULL;
static int *s_imageData = NULL;
static int s_imageDataCap = 0;

static char s_message[512];

const char *ScreenShotHelper_saveScreenshot(const char *gameDir, int var1, int var2) {

    char var3[4096];
    snprintf(var3, sizeof(var3), "%s/screenshots", gameDir);
    mc_mkdir(var3);

    int pixels = var1 * var2;

    if (s_buffer == NULL || s_bufferCap < pixels) {
        free(s_buffer);
        s_buffer = (uint8_t *)malloc(pixels * 3);
        if (!s_buffer)
            goto fail;
        s_bufferCap = pixels;
    }

    if (s_imageData == NULL || s_imageDataCap < pixels * 3) {
        free(s_pixelData);
        free(s_imageData);
        s_pixelData = (uint8_t *)malloc(pixels * 3);
        s_imageData = (int *)malloc(pixels * sizeof(int));
        if (!s_pixelData || !s_imageData)
            goto fail;
        s_imageDataCap = pixels;
    }

    R_pixelStorei(R_PACK_ALIGNMENT, 1);
    R_pixelStorei(R_UNPACK_ALIGNMENT, 1);

    R_readPixels(0, 0, var1, var2, R_RGB, R_UNSIGNED_BYTE, s_buffer);

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    char var4[64];
    strftime(var4, sizeof(var4), "%Y-%m-%d_%H.%M.%S", tm_now);

    int var6 = 1;

    while (1) {

        char var5[4096];
        if (var6 == 1) {
            snprintf(var5, sizeof(var5), "%s/%s.png", var3, var4);
        } else {
            snprintf(var5, sizeof(var5), "%s/%s_%d.png", var3, var4, var6);
        }

        struct stat st;
        if (stat(var5, &st) != 0) {

            memcpy(s_pixelData, s_buffer, pixels * 3);

            for (int var7 = 0; var7 < var1; ++var7) {
                for (int var8 = 0; var8 < var2; ++var8) {
                    int var9 = var7 + (var2 - var8 - 1) * var1;
                    int var10 = s_pixelData[var9 * 3 + 0] & 0xFF;
                    int var11 = s_pixelData[var9 * 3 + 1] & 0xFF;
                    int var12 = s_pixelData[var9 * 3 + 2] & 0xFF;
                    int var13 = (int)0xFF000000 | (var10 << 16) | (var11 << 8) | var12;
                    s_imageData[var7 + var8 * var1] = var13;
                }
            }

            uint8_t *rgba = (uint8_t *)malloc(pixels * 4);
            if (!rgba)
                goto fail;
            for (int i = 0; i < pixels; i++) {
                int argb = s_imageData[i];
                rgba[i * 4 + 0] = (uint8_t)((argb >> 16) & 0xFF);
                rgba[i * 4 + 1] = (uint8_t)((argb >> 8) & 0xFF);
                rgba[i * 4 + 2] = (uint8_t)(argb & 0xFF);
                rgba[i * 4 + 3] = (uint8_t)((argb >> 24) & 0xFF);
            }
            int wrote = stbi_write_png(var5, var1, var2, 4, rgba, var1 * 4);
            free(rgba);
            if (!wrote)
                goto fail;

            const char *basename = strrchr(var5, '/');
            basename = basename ? basename + 1 : var5;
            snprintf(s_message, sizeof(s_message), "Saved screenshot as %s", basename);
            return s_message;
        }

        ++var6;
    }

fail:
    snprintf(s_message, sizeof(s_message), "Failed to save: exception");
    return s_message;
}
