#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#include "GameSettings.h"
#include "RenderEngine.h"

#define FONT_HEIGHT 9

typedef struct FontRenderer {
    int charWidth[256];
    int fontTextureName;
    float colorTable[32][3];
} FontRenderer;

FontRenderer *FontRenderer_create(GameSettings *var1, const char *var2, RenderEngine *var3);
void FontRenderer_destroy(FontRenderer *self);

void FontRenderer_drawStringWithShadow(FontRenderer *self, const char *var1, int var2, int var3,
                                       int var4);

void FontRenderer_drawString(FontRenderer *self, const char *var1, int var2, int var3, int var4);

void FontRenderer_renderString(FontRenderer *self, const char *var1, int var2, int var3, int var4,
                               int var5);

int FontRenderer_getStringWidth(FontRenderer *self, const char *var1);

void FontRenderer_func_27278_a(FontRenderer *self, const char *var1, int var2, int var3, int var4,
                               int var5);

int FontRenderer_func_27277_a(FontRenderer *self, const char *var1, int var2);

#endif
