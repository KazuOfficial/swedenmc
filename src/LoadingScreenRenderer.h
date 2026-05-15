#ifndef LOADING_SCREEN_RENDERER_H
#define LOADING_SCREEN_RENDERER_H

#include "IProgressUpdate.h"
#include <stdint.h>

struct Minecraft;

typedef struct LoadingScreenRenderer {

    IProgressUpdate iProgressUpdate;

    const char *field_1004_a;

    struct Minecraft *mc;

    const char *field_1007_c;

    int64_t field_1006_d;

    int field_1005_e;
} LoadingScreenRenderer;

void LoadingScreenRenderer_init(LoadingScreenRenderer *self, struct Minecraft *var1);

void LoadingScreenRenderer_printText(LoadingScreenRenderer *self, const char *var1);

void LoadingScreenRenderer_func_594_b(LoadingScreenRenderer *self, const char *var1);

void LoadingScreenRenderer_func_597_c(LoadingScreenRenderer *self, const char *var1);

void LoadingScreenRenderer_displayLoadingString(LoadingScreenRenderer *self, const char *var1);

void LoadingScreenRenderer_setLoadingProgress(LoadingScreenRenderer *self, int var1);

#endif
