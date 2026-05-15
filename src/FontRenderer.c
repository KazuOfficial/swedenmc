#include "FontRenderer.h"
#include "Renderer.h"
#include "ChatAllowedCharacters.h"
#include "Tessellator.h"
#include "../lib/stb_image.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *str_sub(const char *str, int len) {
    char *r = (char *)malloc(len + 1);
    memcpy(r, str, len);
    r[len] = '\0';
    return r;
}

static char *str_cat(const char *a, const char *b) {
    int la = (int)strlen(a), lb = (int)strlen(b);
    char *r = (char *)malloc(la + lb + 1);
    memcpy(r, a, la);
    memcpy(r + la, b, lb + 1);
    return r;
}

static char *str_cat3(const char *a, const char *b, const char *c) {
    char *ab = str_cat(a, b);
    char *abc = str_cat(ab, c);
    free(ab);
    return abc;
}

static int str_trim_len(const char *str) {
    int len = (int)strlen(str);
    int s = 0, e = len;
    while (s < e && (unsigned char)str[s] <= ' ')
        s++;
    while (e > s && (unsigned char)str[e - 1] <= ' ')
        e--;
    return e - s;
}

static char **str_split(const char *str, char delim, int *out_count) {
    int n = 1;
    for (const char *p = str; *p; p++)
        if (*p == delim)
            n++;
    char **arr = (char **)malloc(n * sizeof(char *));
    int idx = 0;
    const char *start = str;
    for (const char *p = str;; p++) {
        if (*p == delim || *p == '\0') {
            arr[idx++] = str_sub(start, (int)(p - start));
            start = p + 1;
            if (*p == '\0')
                break;
        }
    }
    *out_count = idx;
    return arr;
}

static void str_split_free(char **arr, int count) {
    for (int i = 0; i < count; i++)
        free(arr[i]);
    free(arr);
}

FontRenderer *FontRenderer_create(GameSettings *var1, const char *var2, RenderEngine *var3) {

    ChatAllowedCharacters_init();

    FontRenderer *self = (FontRenderer *)calloc(1, sizeof(FontRenderer));
    if (!self)
        return NULL;

    const char *path = var2;
    if (path[0] == '/')
        path++;
    int var5, var6, comp;
    unsigned char *var4_rgba = stbi_load(path, &var5, &var6, &comp, 4);
    if (!var4_rgba) {

        for (int i = 0; i < 256; i++)
            self->charWidth[i] = 6;
        return self;
    }

    for (int var8 = 0; var8 < 256; ++var8) {
        int var9 = var8 % 16;
        int var10 = var8 / 16;
        int var11;
        for (var11 = 7; var11 >= 0; --var11) {
            int var12 = var9 * 8 + var11;
            int var13 = 1;
            for (int var14 = 0; var14 < 8 && var13; ++var14) {
                int var15 = (var10 * 8 + var14) * var5;

                int var16 = var4_rgba[(var12 + var15) * 4 + 2] & 0xFF;
                if (var16 > 0)
                    var13 = 0;
            }
            if (!var13)
                break;
        }
        if (var8 == 32)
            var11 = 2;
        self->charWidth[var8] = var11 + 2;
    }

    unsigned int tex = R_genTexture();
    RenderEngine_setupTextureRaw(var3, tex, var5, var6, var4_rgba);
    self->fontTextureName = (int)tex;
    stbi_image_free(var4_rgba);

    for (int var9 = 0; var9 < 32; ++var9) {
        int var10 = (var9 >> 3 & 1) * 85;
        int var11 = (var9 >> 2 & 1) * 170 + var10;
        int var12 = (var9 >> 1 & 1) * 170 + var10;
        int var22 = (var9 >> 0 & 1) * 170 + var10;
        if (var9 == 6)
            var11 += 85;
        int var24 = var9 >= 16;
        if (var1->anaglyph) {
            int var15 = (var11 * 30 + var12 * 59 + var22 * 11) / 100;
            int var16 = (var11 * 30 + var12 * 70) / 100;
            int var17 = (var11 * 30 + var22 * 70) / 100;
            var11 = var15;
            var12 = var16;
            var22 = var17;
        }
        if (var24) {
            var11 /= 4;
            var12 /= 4;
            var22 /= 4;
        }
        self->colorTable[var9][0] = (float)var11 / 255.0f;
        self->colorTable[var9][1] = (float)var12 / 255.0f;
        self->colorTable[var9][2] = (float)var22 / 255.0f;
    }

    return self;
}

void FontRenderer_destroy(FontRenderer *self) {
    if (!self)
        return;
    if (self->fontTextureName)
        R_deleteTexture((unsigned int)self->fontTextureName);
    free(self);
}

void FontRenderer_renderString(FontRenderer *self, const char *var1, int var2, int var3, int var4,
                               int var5) {
    if (var1 == NULL)
        return;

    if (var5) {
        int var6 = var4 & -16777216;
        var4 = (var4 & 16579836) >> 2;
        var4 += var6;
    }

    R_bindTexture((unsigned int)self->fontTextureName);
    float cr = (float)((var4 >> 16) & 255) / 255.0f;
    float cg = (float)((var4 >> 8) & 255) / 255.0f;
    float cb = (float)(var4 & 255) / 255.0f;
    float ca = (float)((var4 >> 24) & 255) / 255.0f;
    if (ca == 0.0f)
        ca = 1.0f;

    R_pushMatrix();
    R_translatef((float)var2, (float)var3, 0.0f);

    Tessellator *tess = &Tessellator_instance;
    Tessellator_startDrawingQuads(tess);
    Tessellator_setColorRGBA_F(tess, cr, cg, cb, ca);

    float cursor_x = 0.0f;
    int len = (int)strlen(var1);
    for (int var6 = 0; var6 < len; ++var6) {

        while (len > var6 + 1 && (unsigned char)var1[var6] == 167) {
            char c2 = var1[var6 + 1];
            if (c2 >= 'A' && c2 <= 'Z')
                c2 = (char)(c2 - 'A' + 'a');
            int ci = -1;
            const char *hex = "0123456789abcdef";
            for (int hi = 0; hi < 16; hi++)
                if (hex[hi] == c2) {
                    ci = hi;
                    break;
                }
            if (ci < 0 || ci > 15)
                ci = 15;
            ci += (var5 ? 16 : 0);
            Tessellator_setColorOpaque_F(tess, self->colorTable[ci][0], self->colorTable[ci][1],
                                         self->colorTable[ci][2]);
            var6 += 2;
        }

        if (var6 < len) {
            int var11 = -1;
            const char *allowed = ChatAllowedCharacters_allowedCharacters;
            if (allowed) {
                int alen = (int)strlen(allowed);
                for (int ai = 0; ai < alen; ai++)
                    if (allowed[ai] == var1[var6]) {
                        var11 = ai;
                        break;
                    }
            }
            if (var11 >= 0) {
                int charCode = var11 + 32;
                float tx = (float)(charCode % 16 * 8) / 128.0f;
                float ty = (float)(charCode / 16 * 8) / 128.0f;
                float w = 7.99f;
                float tw = w / 128.0f;
                Tessellator_addVertexWithUV(tess, cursor_x, w, 0.0, tx, ty + tw);
                Tessellator_addVertexWithUV(tess, cursor_x + w, w, 0.0, tx + tw, ty + tw);
                Tessellator_addVertexWithUV(tess, cursor_x + w, 0.0, 0.0, tx + tw, ty);
                Tessellator_addVertexWithUV(tess, cursor_x, 0.0, 0.0, tx, ty);
                cursor_x += (float)self->charWidth[charCode];
            }
        }
    }

    Tessellator_draw(tess);
    R_popMatrix();
}

void FontRenderer_drawStringWithShadow(FontRenderer *self, const char *var1, int var2, int var3,
                                       int var4) {
    FontRenderer_renderString(self, var1, var2 + 1, var3 + 1, var4, 1);
    FontRenderer_drawString(self, var1, var2, var3, var4);
}

void FontRenderer_drawString(FontRenderer *self, const char *var1, int var2, int var3, int var4) {
    FontRenderer_renderString(self, var1, var2, var3, var4, 0);
}

int FontRenderer_getStringWidth(FontRenderer *self, const char *var1) {
    if (var1 == NULL)
        return 0;
    int var2 = 0;
    int len = (int)strlen(var1);
    for (int var3 = 0; var3 < len; ++var3) {
        if ((unsigned char)var1[var3] == 167) {
            ++var3;
        } else {

            int var4 = -1;
            const char *allowed = ChatAllowedCharacters_allowedCharacters;
            if (allowed) {
                int alen = (int)strlen(allowed);
                for (int i = 0; i < alen; i++) {
                    if (allowed[i] == var1[var3]) {
                        var4 = i;
                        break;
                    }
                }
            }
            if (var4 >= 0) {
                var2 += self->charWidth[var4 + 32];
            }
        }
    }
    return var2;
}

void FontRenderer_func_27278_a(FontRenderer *self, const char *var1, int var2, int var3, int var4,
                               int var5) {

    int var6_count;
    char **var6 = str_split(var1, '\n', &var6_count);

    if (var6_count > 1) {
        for (int var11 = 0; var11 < var6_count; ++var11) {
            FontRenderer_func_27278_a(self, var6[var11], var2, var3, var4, var5);
            var3 += FontRenderer_func_27277_a(self, var6[var11], var4);
        }
    } else {

        int var7_count;
        char **var7 = str_split(var1, ' ', &var7_count);
        int var8 = 0;

        while (var8 < var7_count) {

            char *var9 = str_cat(var7[var8++], " ");

            while (var8 < var7_count) {
                char *test = str_cat(var9, var7[var8]);
                int w = FontRenderer_getStringWidth(self, test);
                free(test);
                if (w >= var4)
                    break;
                char *new_var9 = str_cat3(var9, var7[var8++], " ");
                free(var9);
                var9 = new_var9;
            }

            while (FontRenderer_getStringWidth(self, var9) > var4) {
                int var10 = 0;

                while (1) {
                    char *sub = str_sub(var9, var10 + 1);
                    int w = FontRenderer_getStringWidth(self, sub);
                    free(sub);
                    if (w > var4)
                        break;
                    ++var10;
                }

                char *prefix = str_sub(var9, var10);
                if (str_trim_len(prefix) > 0) {
                    FontRenderer_drawString(self, prefix, var2, var3, var5);
                    var3 += 8;
                }
                free(prefix);

                char *new_var9 = str_cat(var9 + var10, "");
                free(var9);
                var9 = new_var9;
            }

            if (str_trim_len(var9) > 0) {
                FontRenderer_drawString(self, var9, var2, var3, var5);
                var3 += 8;
            }
            free(var9);
        }

        str_split_free(var7, var7_count);
    }

    str_split_free(var6, var6_count);
}

int FontRenderer_func_27277_a(FontRenderer *self, const char *var1, int var2) {

    int var3_count;
    char **var3 = str_split(var1, '\n', &var3_count);
    int result;

    int var5;
    if (var3_count > 1) {
        int var9 = 0;
        for (var5 = 0; var5 < var3_count; ++var5) {
            var9 += FontRenderer_func_27277_a(self, var3[var5], var2);
        }
        result = var9;
    } else {

        int var4_count;
        char **var4 = str_split(var1, ' ', &var4_count);
        var5 = 0;
        int var6 = 0;

        while (var5 < var4_count) {

            char *var7 = str_cat(var4[var5++], " ");

            while (var5 < var4_count) {
                char *test = str_cat(var7, var4[var5]);
                int w = FontRenderer_getStringWidth(self, test);
                free(test);
                if (w >= var2)
                    break;
                char *new_var7 = str_cat3(var7, var4[var5++], " ");
                free(var7);
                var7 = new_var7;
            }

            while (FontRenderer_getStringWidth(self, var7) > var2) {
                int var8 = 0;
                while (1) {
                    char *sub = str_sub(var7, var8 + 1);
                    int w = FontRenderer_getStringWidth(self, sub);
                    free(sub);
                    if (w > var2)
                        break;
                    ++var8;
                }

                char *prefix = str_sub(var7, var8);
                if (str_trim_len(prefix) > 0)
                    var6 += 8;
                free(prefix);

                char *new_var7 = str_cat(var7 + var8, "");
                free(var7);
                var7 = new_var7;
            }

            if (str_trim_len(var7) > 0)
                var6 += 8;
            free(var7);
        }

        if (var6 < 8)
            var6 += 8;
        result = var6;
        str_split_free(var4, var4_count);
    }

    str_split_free(var3, var3_count);
    return result;
}
